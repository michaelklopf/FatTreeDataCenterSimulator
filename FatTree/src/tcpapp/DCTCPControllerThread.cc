//
// Author: Michael Klopf
// 
//#define FSM_DEBUG
#include "DCTCPControllerThread.h"
#include <IPAddressResolver.h>
#include <TCPSocket.h>

Register_Class(DCTCPControllerThread);

void DCTCPControllerThread::established()
{
    this->fsm = dynamic_cast<DCTCPControllerApp *>(hostmod)->getFSMCON();
}

void DCTCPControllerThread::dataArrived(cMessage *msg, bool)
{
    // Check if msg of type RequestMsg
    RequestMsg *reqmsg = dynamic_cast<RequestMsg *>(msg);
    // Fetch socketMap to add the new socket
    socketMap = getHostModule()->getSocketMap();

    // Set with active vms.
    activeVMs = getHostModule()->getActiveVMs();

    freeVMs = getHostModule()->getFreeVMs();

    // Set with all vms.
    allVMs = getHostModule()->getAllVMs();

    // Set job queues.
    jobQueue = getHostModule()->getJobQueue();

    webJobQueue = getHostModule()->getWebJobQueue();

    ftpJobQueue = getHostModule()->getFTPJobQueue();

    if (reqmsg) {
        handleRequestMsg(reqmsg);
        return;
    }
    // Check if msg of type ReplyMsg.
    ReplyMsg *repmsg = dynamic_cast<ReplyMsg *>(msg);

    if (repmsg) {
        handleReplyMsg(repmsg);
        return;
    }

    // Check if msg does migration related task.
    VMMigMsg *vmmsg = dynamic_cast<VMMigMsg *>(msg);

    if (vmmsg) {
        handleVirtualMachineMigration(vmmsg);
        return;
    }

    opp_error("Message (%s)%s is not a RequestMsg, nor ReplyMsg, nor VMMsg -- "
              "probably wrong client app, or wrong setting of TCP's "
              "sendQueueClass/receiveQueueClass parameters "
              "(try \"TCPMsgBasedSendQueue\" and \"TCPMsgBasedRcvQueue\")",
              msg->getClassName(), msg->getName());
}

void DCTCPControllerThread::timerExpired(cMessage *timer)
{
    VMMigMsg *vmmsg = dynamic_cast<VMMigMsg *>(timer);

    if (vmmsg) {
        handleVirtualMachineMigration(vmmsg);
        return;
    }

    RequestMsg *reqmsg = dynamic_cast<RequestMsg *>(timer);

    if (reqmsg) {
    	//handleRequestMsg(reqmsg);
        processRequestMsg(reqmsg);
    	return;
    }
}

IPvXAddress DCTCPControllerThread::chooseVMAddr(int port)
{
	//ServiceMap *freeVMsforService = freeVMs->getVMsWithPort(port);
	std::auto_ptr<ServiceMap> freeVMsforService(freeVMs->getVMsWithPort(port));

	if (freeVMsforService.get() == NULL || freeVMsforService->getSize() == 0) {
		opp_error("Could not find a free VM.");
	}
	int numOfFreeVMs = freeVMsforService->getSize();
    //std::srand(time(0));
    int k = (rand() % numOfFreeVMs);

    IPvXAddress addr = freeVMsforService->getIPAddress(k);

    //delete freeVMsforService;
    //EV << "Connect address is called " << addr.str().c_str() << "\n";
    //EV << "Is address unspecified? " << addr.isUnspecified() << "\n";
    return addr;
}

void DCTCPControllerThread::handleRequestMsg(RequestMsg *reqmsg)
{
	// Save socket id to traffic generator.
	reqmsg->setConnID(getSocket()->getConnectionId());
	reqmsg->setContextPointer(this);

	if (reqmsg->getServiceport() == getHostModule()->HTTPPORT) {
	    if (getHostModule()->isUpdateNumVMS()) {
	        getHostModule()->updateNumOfActiveVMsAndServers(getHostModule()->HTTPPORT);
	    }
	    if (webJobQueue->size() > 0) {
	        webJobQueue->push_back(*reqmsg);

	        emitJobQueueLengthForService(getHostModule()->HTTPPORT);

	        delete reqmsg;
	    } else if (freeVMs->isThereFreeVMForService(getHostModule()->HTTPPORT)) {
	        processRequestMsg(reqmsg);
	    } else {
	        webJobQueue->push_back(*reqmsg);

	        emitJobQueueLengthForService(getHostModule()->HTTPPORT);

	        delete reqmsg;
	    }
	} else if (reqmsg->getServiceport() == getHostModule()->FTPPORT) {
	    if (getHostModule()->isUpdateNumVMS()) {
	        getHostModule()->updateNumOfActiveVMsAndServers(getHostModule()->FTPPORT);
	    }
	    if (ftpJobQueue->size() > 0) {
	        ftpJobQueue->push_back(*reqmsg);

	        emitJobQueueLengthForService(getHostModule()->FTPPORT);

	        delete reqmsg;
	    } else if (freeVMs->isThereFreeVMForService(getHostModule()->FTPPORT)) {
	        processRequestMsg(reqmsg);
	    } else {
	        ftpJobQueue->push_back(*reqmsg);

	        emitJobQueueLengthForService(getHostModule()->FTPPORT);

	        delete reqmsg;
	    }
	}
}

void DCTCPControllerThread::processRequestMsg(RequestMsg *reqmsg)
{
    // emit signal
    getHostModule()->emitProcessedJobRequest(reqmsg->getServiceport());

    // Choose one VM to send to
    IPvXAddress addr = chooseVMAddr(reqmsg->getServiceport());

    freeVMs->removeVM(addr); // make vm unreachable
    getHostModule()->emitFreeVMsSignal(freeVMs->getSize());
    //std::cerr << "DEBUG : Num busy vms req " << activeVMs->getVMsNotInOtherMap(freeVMs)->getSize() << "\n";
    getHostModule()->emitNumberOfBusyVMs(activeVMs->getVMsNotInOtherMap(freeVMs)->getSize());
    getHostModule()->emitFreeVMsSignal(freeVMs->getVMsWithPort(reqmsg->getServiceport())->getSize(), reqmsg->getServiceport());

    // Create new socket for connection
    TCPSocket nsock;
    nsock.setOutputGate(getHostModule()->gate("tcpOut"));
    nsock.bind((int)(hostmod->getPortList()->front()));
    hostmod->getPortList()->pop_front();
    nsock.connect(addr, reqmsg->getServiceport()); // remote addr and port

    socketMap.addSocket(&nsock);

    // reset control info and give it a new name.
    reqmsg->removeControlInfo();
    reqmsg->setName("requestCon");

    nsock.send(reqmsg);
}

void DCTCPControllerThread::handleReplyMsg(ReplyMsg *repmsg)
{
	EV << "Gotcha: 7. Receiving " << repmsg->getClassName() << "\n.";
    TCPSocket *oldsock;
    oldsock = socketMap.findSocketForConnId(repmsg->getConnID());
    if (!oldsock) {
     	opp_error("Could not find socket in socket map. CT");
    }

    const char *addrstr = repmsg->getVmaddress();
    IPvXAddress addr = IPvXAddress(addrstr);

    if (addr != getHostModule()->getMigratedVM()) {
        freeVMs->addVM(addr, allVMs->getPortForAddress(addr)); // make vm visible again, but not when it is the migrated vm.
        getHostModule()->emitFreeVMsSignal(freeVMs->getSize());
        //std::cerr << "DEBUG : Num busy vms reply " << activeVMs->getVMsNotInOtherMap(freeVMs)->getSize() << "\n";
        getHostModule()->emitNumberOfBusyVMs(activeVMs->getVMsNotInOtherMap(freeVMs)->getSize());
        getHostModule()->emitFreeVMsSignal(freeVMs->getVMsWithPort(repmsg->getServiceport())->getSize(), repmsg->getServiceport());

        if (repmsg->getServiceport() == getHostModule()->HTTPPORT) {
            if (webJobQueue->size() > 0) {
                RequestMsg *reqmsg = new RequestMsg(webJobQueue->front());
                webJobQueue->pop_front();
                processRequestMsg(reqmsg);

                emitJobQueueLengthForService(getHostModule()->HTTPPORT);
                if (getHostModule()->isUpdateNumVMS()) {
                    getHostModule()->updateNumOfActiveVMsAndServers(getHostModule()->HTTPPORT);
                }
            }
        } else if (repmsg->getServiceport() == getHostModule()->FTPPORT) {
            if (ftpJobQueue->size() > 0) {
                RequestMsg *reqmsg = new RequestMsg(ftpJobQueue->front());
                ftpJobQueue->pop_front();
                processRequestMsg(reqmsg);

                emitJobQueueLengthForService(getHostModule()->FTPPORT);
                if (getHostModule()->isUpdateNumVMS()) {
                    getHostModule()->updateNumOfActiveVMsAndServers(getHostModule()->FTPPORT);
                }
            }
        }
    }

    ReplyMsg *replyMessage = new ReplyMsg(*repmsg);
    replyMessage->setExpectedReplyLength(repmsg->getExpectedReplyLength());
    replyMessage->setServerClose(repmsg->getServerClose());

    delete repmsg;

    if (replyMessage->getServerClose()) {
        //oldsock->close();
    	hostmod->getPortList()->push_back(getSocket()->getLocalPort());
    	getSocket()->close(); // close socket to vms
    }
    oldsock->send(replyMessage);
}

void DCTCPControllerThread::handleVirtualMachineMigration(VMMigMsg *vmmsg)
{
    FSM_Switch(*fsm) {
        // Initializing the controller fsm. Go to IDLE state.
        case FSM_Exit(INIT):
        	// save the address for migrated vm for use with handleReplyMsg.
        //getHostModule()->setMigratedVM(IPvXAddress(vmmsg->getVm1address()));
        	// save socket to migration start up module.
            vmmsg->setConnIdToMigS(getSocket()->getConnectionId());
            scheduleAt(simTime(), vmmsg);
            FSM_Goto(*fsm, IDLE);
        break;

        // Idle state where waiting for migration start.
        case FSM_Exit(IDLE):
           // Init starts the migration process with starting hv2.
           if (vmmsg->getMsgtype() == INIT_MIG) {
               if (vmmsg->getTurnOnHV2()) {
                   scheduleAt(simTime(), vmmsg);
                   FSM_Goto(*fsm, STARTINGHOST);
               } else {
                   scheduleAt(simTime(), vmmsg);
                   vmmsg->setMsgtype(HV2ENABLED);
                   FSM_Goto(*fsm, COPYING);
               }
           } else {
               opp_error("Invalid state in IDLE state.");
           }
        break;

        // Starting hv2.
        case FSM_Exit(STARTINGHOST):
            // HV2 has started initiate copying process.
            if (vmmsg->getMsgtype() == HV2ENABLED) {
                scheduleAt(simTime(), vmmsg);
                FSM_Goto(*fsm, COPYING);
                break;
            }
            // Start HV2 or start copying process.
            if (vmmsg->getMsgtype() == INIT_MIG) {
                // Choose between starting up hv2 or directly go to copy state.
                // Connect to hv2.
                IPvXAddress *hv2address = new IPvXAddress(vmmsg->getHv2address());
                // Create new socket for connection
                TCPSocket nsock;
                nsock.setOutputGate(getHostModule()->gate("tcpOut"));
                nsock.connect(*hv2address, 1000);
                socketMap.addSocket(&nsock);

                vmmsg->removeControlInfo();
                vmmsg->setMsgtype(ENABLEHV2);
                //servers->addVM(*hv2address);
                nsock.send(vmmsg);
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Start HV2!");
            } else {
                opp_error("Invalid state in STARTINGHOST state.");
            }
        break;

        // Copying data from hv1 to hv2.
        case FSM_Exit(COPYING):
            // Coming from STARTINGHOST to start copy process.
            if (vmmsg->getMsgtype() == HV2ENABLED) {
            	if (vmmsg->getTurnOnHV2()) {
            		getHostModule()->getServerStates()->changeState(IPvXAddress(vmmsg->getHv2address()), true);
            		getSocket()->close(); // Close socket to HV2.
            	}
                // Connect to hv1.
                IPvXAddress *hv1address = new IPvXAddress(vmmsg->getHv1address());

                // Create new socket for connection
                TCPSocket nsock;
                nsock.setOutputGate(getHostModule()->gate("tcpOut"));
                nsock.connect(*hv1address, 1000);
                socketMap.addSocket(&nsock);

                vmmsg->removeControlInfo();
                vmmsg->setMsgtype(COPYINGDATA);
                nsock.send(vmmsg);
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Initiate Copy process!");
                break;
            }
            // After copying was successful start migration.
            if (vmmsg->getMsgtype() == COPYSUCCESS) {
                scheduleAt(simTime(), vmmsg);
                FSM_Goto(*fsm, MIGRATION);
            } else {
                opp_error("Invalid state in COPYING state.");
            }
        break;

        // Migrating vms, i.e., turning off vm1, starting vm2, turning off hv1.
        case FSM_Exit(MIGRATION):
            // Start migration process with turning off vm1.
            if (vmmsg->getMsgtype() == COPYSUCCESS) {
                //Take VM1 out of the active VMs set, to stop job requests.
                IPvXAddress *vm1address = new IPvXAddress(vmmsg->getVm1address());
                freeVMs->removeVM(*vm1address);
                getHostModule()->emitFreeVMsSignal(freeVMs->getSize());
                getHostModule()->emitNumberOfBusyVMs(activeVMs->getVMsNotInOtherMap(freeVMs)->getSize());
                getHostModule()->emitFreeVMsSignal(freeVMs->getVMsWithPort(vmmsg->getServiceport())->getSize(), vmmsg->getServiceport());

                vmmsg->removeControlInfo();
                vmmsg->setMsgtype(TURNOFFVM1);
                getSocket()->send(vmmsg); // send via current socket back to HV1.
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Turn off VM1!");
                break;
            }
            // Successful start up of vm2, vm1 is turned off. Turn off hv1.
            if (vmmsg->getMsgtype() == STARTSUCCESS) {
            	// Change the hv of the vms.
                getHostModule()->getServers()->removeVM(IPvXAddress(vmmsg->getVm1address()));
                getHostModule()->getServers()->removeVM(IPvXAddress(vmmsg->getVm2address()));
            	getHostModule()->getServers()->changeHV(IPvXAddress(vmmsg->getVm1address()), IPvXAddress(vmmsg->getHv2address()));
            	getHostModule()->getServers()->changeHV(IPvXAddress(vmmsg->getVm2address()), IPvXAddress(vmmsg->getHv1address()));
                IPvXAddress hv1 = getHostModule()->getServers()->getServerAddr(IPvXAddress(vmmsg->getVm1address()));
                IPvXAddress hv2 = getHostModule()->getServers()->getServerAddr(IPvXAddress(vmmsg->getVm2address()));
                std::string addrstr = hv1.str();
                const char *addrchar = addrstr.c_str();

                std::string addrstr2 = hv2.str();
                const char *addrchar2 = addrstr2.c_str();
            	// Add vm1 to active vms list again, after it is successful migrated.
                IPvXAddress *vm1address = new IPvXAddress(vmmsg->getVm1address());
                freeVMs->addVM(*vm1address, allVMs->getPortForAddress(*vm1address));
                getHostModule()->emitFreeVMsSignal(freeVMs->getSize());
                getHostModule()->emitNumberOfBusyVMs(activeVMs->getVMsNotInOtherMap(freeVMs)->getSize());
                getHostModule()->emitFreeVMsSignal(freeVMs->getVMsWithPort(vmmsg->getServiceport())->getSize(), vmmsg->getServiceport());
                // Here code to add new ip of vm1 and remove old ip of vm2. (or switch?)

                if (vmmsg->getTurnOffHV1()) {
                	// Check for active VMs and take them out of active VM list.
                	//checkAndSetVMsInactive(vmmsg);
                    vmmsg->removeControlInfo();
                    vmmsg->setMsgtype(TURNOFFHV1);
                    //servers->removeVM(getSocket()->getRemoteAddress());
                	getSocket()->send(vmmsg); // send via current socket back to HV1.
                    if (ev.isGUI())
                        getHostModule()->getParentModule()->bubble("Initiate turning off HV1!");
                    break;
                } else {
                    FSM_Goto(*fsm, INIT);
                    // Send msg back to migration starter module.
                    SocketMapExt socketMap = getHostModule()->getSocketMap();
                    TCPSocket *migStartSock = socketMap.findSocketForConnId(vmmsg->getConnIdToMigS());
                    if (!migStartSock) {
                    	opp_error("Could not find socket in socket map. TC StartSuccess.");
                    }
                    vmmsg->removeControlInfo();
                    migStartSock->send(vmmsg);
                    getSocket()->close(); // Close socket to hv1.
                    if (ev.isGUI())
                        getHostModule()->getParentModule()->bubble("Send msg back to manager module!");
                    break;
                }
            }
            // Successfully turned off hv1. Delete vmmsg, go back to idle.
            if (vmmsg->getMsgtype() == HV1TURNEDOFF) {
                FSM_Goto(*fsm, INIT);
                getHostModule()->getServerStates()->changeState(IPvXAddress(vmmsg->getHv1address()), false);
            	// Send msg back to migration starter module.
                SocketMapExt socketMap = getHostModule()->getSocketMap();
                TCPSocket *migStartSock = socketMap.findSocketForConnId(vmmsg->getConnIdToMigS());
                if (!migStartSock) {
                	opp_error("Could not find socket in socket map. TC HV1TurnedOff.");
                }
                vmmsg->removeControlInfo();
                migStartSock->send(vmmsg);
                getSocket()->close(); // Close socket to hv1.
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Send msg back to manager module!");
            } else {
                opp_error("Invalid state in MIGRATION state.");
            }
        break;
    }
}

void DCTCPControllerThread::checkAndSetVMsInactive(VMMigMsg *vmmsg)
{

}

void DCTCPControllerThread::peerClosed()
{
    /*
	if (getSocket()->getState()==TCPSocket::PEER_CLOSED)
	{
	    EV << "remote TCP closed, closing here as well\n";
	    getSocket()->close();
	}*/
	EV << "Hack the ConThread.\n";
}

void DCTCPControllerThread::emitJobQueueLengthForService(int serviceport)
{
	// Get the numbers of messages in job queue, per service.
    if (serviceport == getHostModule()->HTTPPORT) {
        getHostModule()->emitJobQueueLength(webJobQueue->size(), serviceport);
    } else if (serviceport == getHostModule()->FTPPORT) {
        getHostModule()->emitJobQueueLength(ftpJobQueue->size(), serviceport);
    }
}
