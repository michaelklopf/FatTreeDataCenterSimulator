//
// Author: Michael Klopf
// 
//#define FSM_DEBUG
#include "HyperVisorThread.h"
#include "HyperVisorApp.h"
#include <IPAddressResolver.h>

Register_Class(HyperVisorThread);

void HyperVisorThread::established()
{
    this->fsm = dynamic_cast<HyperVisorApp *>(hostmod)->getFSMHV();
    connIdVMSock = 0;
    /*
    const char *addrchar1 = getSocket()->getLocalAddress().str().c_str();
    const char *addrchar = getSocket()->getRemoteAddress().str().c_str();
    getHostModule()->getSocketMaps()->printSockets();
    */
    //this->socketMap = dynamic_cast<HyperVisorApp *>(hostmod)->getSocketMap();
}

void HyperVisorThread::dataArrived(cMessage *msg, bool)
{
    sockets = getHostModule()->getSocketMaps();
    //DEBUG
    /*
    IPvXAddress vmAddr = getSocket()->getRemoteAddress();
    std::string addrstr = vmAddr.str();
    const char *addrchar = addrstr.c_str();
    IPvXAddress hvAddr = getSocket()->getLocalAddress();
    std::string addrstr2 = hvAddr.str();
    const char *addrchar2 = addrstr2.c_str();
    */

    // Check if msg does migration related task.
    VMMigMsg *vmmsg = dynamic_cast<VMMigMsg *>(msg);

    if (vmmsg) {
        handleVirtualMachineMigration(vmmsg);
        return;
    }

    StartHV *hvstart = dynamic_cast<StartHV *>(msg);

    if (hvstart) {
        //std::cerr << "DEBUG: Being in Start HV of " << hvstart->getHvaddress() << " at " << simTime().dbl() << "\n";
    	handleHyperVisorStart(hvstart);
    	return;
    }

    ShutDownHV *hvstop = dynamic_cast<ShutDownHV*>(msg);

    if (hvstop) {
        //std::cerr << "DEBUG: Being in ShutDown HV of " << hvstop->getHvaddress() << " at " << simTime().dbl() << "\n";
    	handleHyperVisorShutDown(hvstop);
    	return;
    }

    if (!hvstop)
        opp_error("Message (%s)%s is not a VMMsg at HV -- "
                  "probably wrong client app, or wrong setting of TCP's "
                  "sendQueueClass/receiveQueueClass parameters "
                  "(try \"TCPMsgBasedSendQueue\" and \"TCPMsgBasedRcvQueue\")",
                  msg->getClassName(), msg->getName());

}

void HyperVisorThread::timerExpired(cMessage *timer)
{
    sockets = getHostModule()->getSocketMaps();

    VMMigMsg *vmmsg = dynamic_cast<VMMigMsg *>(timer);

    if (vmmsg) {
        handleVirtualMachineMigration(vmmsg);
        return;
    }

    StartHV *hvstart = dynamic_cast<StartHV *>(timer);

    if (hvstart) {
        //std::cerr << "DEBUG: Being in Start HV timerexp of " << hvstart->getHvaddress() << " at " << simTime().dbl() << "\n";
        if (IPvXAddress(hvstart->getHvaddress()) == IPvXAddress("10.1.0.2")) {
            int a = 0;
            a = a++;
        }
    	HyperVisorApp *hvapp = dynamic_cast<HyperVisorApp *>(getHostModule());
    	hvapp->startUpServer();
        //check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOnAndOff(false);
    	check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOn();
    	//std::cerr << "DEBUG: Finishing startup server " << hvstart->getHvaddress() << " at " << simTime().dbl() << "\n";

    	// Set state of server
    	if (check_and_cast<HyperVisorApp*>(getHostModule())->getHVState()) {
    		hvstart->setServerState(true);
    	} else {
    		hvstart->setServerState(false);
    	}

    	hvstart->removeControlInfo();
    	getSocket()->send(hvstart);
    	/*
    	if (hvstart->getSource() == VM) {
    	} else if (hvstart->getSource() == CONTROLLER) {

    	} else {
    		opp_error("Got a start up msg from an unknown source.");
    	}*/
    }

    ShutDownHV *hvstop = dynamic_cast<ShutDownHV*>(timer);

    if (hvstop) {
        //std::cerr << "DEBUG: Being in ShutDonw HV timerexp of " << hvstop->getHvaddress() << " at " << simTime().dbl() << "\n";
        if (IPvXAddress(hvstop->getHvaddress()) == IPvXAddress("10.1.0.2")) {
            int a = 0;
            a = a++;
        }
    	HyperVisorApp *hvapp = dynamic_cast<HyperVisorApp *>(getHostModule());
    	hvapp->shutDownServer();

    	// Set state of server
    	if (check_and_cast<HyperVisorApp*>(getHostModule())->getHVState()) {
    		hvstop->setServerState(true);
    	} else {
    		hvstop->setServerState(false);
    	    //check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOnAndOff(false);
    	    check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOff();
    	    //std::cerr << "DEBUG: Being in stop HV timerexp of " << hvstop->getHvaddress() << " at " << simTime().dbl() << "\n";
    	}

    	hvstop->removeControlInfo();

    	/*
        SocketMapExt socketMap = getHostModule()->getSocketMap();
        TCPSocket *conSock = socketMap.findSocketForConnId(connIdVMSock);
        if (!conSock) {
            opp_error("Could not find socket in socket map. HV data received.");
        }
        conSock->send(hvstop);*/
    	getSocket()->send(hvstop);
    }
}

void HyperVisorThread::handleVirtualMachineMigration(VMMigMsg *vmmsg)
{
    FSM_Switch(*fsm) {
        // Branch for initializing hyper visor FSM.
        case FSM_Exit(INIT):
            // When hv is on, go to active state, else go to off state.
            if (dynamic_cast<HyperVisorApp*>(hostmod)->getHVState()) {
                scheduleAt(simTime(), vmmsg);
                FSM_Goto(*fsm, ACTIVE);
            } else {
                scheduleAt(simTime(), vmmsg);
                FSM_Goto(*fsm, HVOFF);
            }
        break;

        // Active state, in which hyper visor is when on.
        case FSM_Exit(ACTIVE):
            // State, when hyper visor 2 should be started.
            if (vmmsg->getMsgtype() == ENABLEHV2) {
            	// Set correct state of hv and emitSignal for statistics.
            	HyperVisorApp *hv = dynamic_cast<HyperVisorApp*>(getHostModule());
                hv->setHVState(true);
                hv->emitSignal();
                hv->emitPoweringOn();

                vmmsg->removeControlInfo();
                vmmsg->setMsgtype(HV2ENABLED);
                getSocket()->send(vmmsg);
                break;
            }
            // State, when to start copying process.
            if (vmmsg->getMsgtype() == COPYINGDATA) {
            	vmmsg->setConnIdfromHV1ToCon(getSocket()->getConnectionId());
                // Self-Msg to simulate copying time.
                scheduleAt(simTime(), vmmsg);
                FSM_Goto(*fsm, COPYING);
                break;
            }
            // State, when data is copied from hv1 to hv2.
            if (vmmsg->getMsgtype() == COPYSTART) {
                vmmsg->removeControlInfo();
                vmmsg->setMsgtype(DATARECEIVED);
                getSocket()->send(vmmsg);
                FSM_Goto(*fsm, INIT); // set back to init, hv2 fsm has finished.
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Received copied data!");
                break;
            }
            // State, when vm1 has to be turned off.
            if (vmmsg->getMsgtype() == TURNOFFVM1) {
                // Connect to vm1
                IPvXAddress *vm1address = new IPvXAddress(vmmsg->getVm1address());
                // Create new socket for connection
                TCPSocket nsock;
                nsock.setOutputGate(getHostModule()->gate("tcpOut"));
                nsock.connect(*vm1address, vmmsg->getServiceport());
                socketMap.addSocket(&nsock);

                vmmsg->removeControlInfo();
                nsock.send(vmmsg);
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Initiate VM1 shut down!");
                break;
            }
            // HV1 tells VM2 to start.
            if (vmmsg->getMsgtype() == VM1TURNEDOFF) {
                // Connect to vm2
                IPvXAddress *vm2address = new IPvXAddress(vmmsg->getVm2address());
                // Create new socket for connection
                TCPSocket nsock;
                nsock.setOutputGate(getHostModule()->gate("tcpOut"));
                nsock.connect(*vm2address, vmmsg->getServiceport());
                socketMap.addSocket(&nsock);

                vmmsg->removeControlInfo();
                vmmsg->setMsgtype(STARTVM2);
                nsock.send(vmmsg);
                getSocket()->close(); // Close socket to VM1.
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Initiate VM2 start!");
                break;
            }

            // VM2 start was successful, report to controller.
            if (vmmsg->getMsgtype() == VM2TURNEDON) {
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("VM2 start successful!");
                //SocketMapExt socketMap = getHostModule()->getSocketMap();
                //TCPSocket *conSock = socketMap.findSocketForConnId(vmmsg->getConnIdfromHV1ToCon());
                TCPSocket *conSock = sockets->findSocketForConnId(vmmsg->getConnIdfromHV1ToCon());
                if (!conSock) {
                	opp_error("Could not find socket in socket map. HV VM2TurnedOn");
                }
                vmmsg->removeControlInfo();
                vmmsg->setMsgtype(STARTSUCCESS);
                conSock->send(vmmsg);
                getSocket()->close(); // Close socket to vm2.
                break;
            }
            // Msg from Controller to turn off HV1.
            if (vmmsg->getMsgtype() == TURNOFFHV1) {
            	// Set correct state of hv and emitSignal for statistics.
                checkAndPowerDownVMs();
                check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOff();
                scheduleAt(simTime()+getHostModule()->par("hvPowerDownTime"), vmmsg);
                vmmsg->setMsgtype(HV1TURNEDOFF);
                FSM_Goto(*fsm, HVOFF);
            } else {
                opp_error("Invalid state in ACTIVE state.");
            }
        break;

        // Off state, when hyper visor is turned off.
        case FSM_Exit(HVOFF):
             // Action to start hyper visor.
             if (vmmsg->getMsgtype() == ENABLEHV2) {
                 // Self-Msg to simulate start up of hv.
                 check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOn();
                 scheduleAt(simTime()+getHostModule()->par("hvStartUpTime"),vmmsg);
                 FSM_Goto(*fsm, ACTIVE);
                 if (ev.isGUI())
                     getHostModule()->getParentModule()->bubble("Starting up HV2!");
                 break;
             }
             // Action to finalize hv1 turn off.
             if (vmmsg->getMsgtype() == HV1TURNEDOFF) {
                 HyperVisorApp *hv = dynamic_cast<HyperVisorApp*>(getHostModule());
                 hv->setHVState(false);
                 hv->emitPoweringOff();
                 hv->emitSignal();

                 vmmsg->removeControlInfo();
                 getSocket()->send(vmmsg); // send back to controller with current socket.
                 if (ev.isGUI())
                     getHostModule()->getParentModule()->bubble("HV1 turned off!");
                 FSM_Goto(*fsm, INIT); // set back to INIT, hv1 fsm has finished.
             } else {
                 opp_error("Invalid state in HVOFF state.");
             }
        break;

        // Copying state, when data is moved to other hyper visor / vm.
        case FSM_Exit(COPYING):
            // Connecting to HV2 and sending data.
            if (vmmsg->getMsgtype() == COPYINGDATA) {
                // Connect to hv2.
                IPvXAddress *hv2address = new IPvXAddress(vmmsg->getHv2address());
                // Create new socket for connection
                TCPSocket nsock;
                nsock.setOutputGate(getHostModule()->gate("tcpOut"));
                nsock.connect(*hv2address, 1000);
                socketMap.addSocket(&nsock);

                vmmsg->removeControlInfo();
                // Set length for moving vm content.
                vmmsg->setByteLength((int)getHostModule()->par("copyDataSize"));
                vmmsg->setMsgtype(COPYSTART);
                nsock.send(vmmsg);
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Copy data to HV2!");
                break;
            }
            // Receive msg for successful completion of data transfer.
            if (vmmsg->getMsgtype() == DATARECEIVED) {
                vmmsg->removeControlInfo();
                vmmsg->setMsgtype(COPYSUCCESS);
                // Find socket for conn to TC.
                //SocketMapExt socketMap = getHostModule()->getSocketMap();
                // Signal TC successful data transfer.
                //TCPSocket *conSock = socketMap.findSocketForConnId(vmmsg->getConnIdfromHV1ToCon());
                TCPSocket *conSock = sockets->findSocketForConnId(vmmsg->getConnIdfromHV1ToCon());
                if (!conSock) {
                	opp_error("Could not find socket in socket map. HV data received.");
                }
                conSock->send(vmmsg);
                getSocket()->close(); // Close socket to HV2
                FSM_Goto(*fsm, ACTIVE);
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Copying was a success!");
            } else {
                opp_error("Invalid state in COPYING-HV state.");
            }
        break;
    }

}

void HyperVisorThread::checkAndPowerDownVMs()
{
	for (cSubModIterator iter(*getHostModule()->getParentModule()->getParentModule()); !iter.end(); iter++) {
		if (iter()->isName("vms")) {
			VirtualMachineApp *vmapp = dynamic_cast<VirtualMachineApp *>(iter()->getSubmodule("tcpApp",0));
			if (vmapp->getVMState()) {
				opp_error("Warning: Powering down VM manually.");
				//vmapp->setVMstate(false);
				//vmapp->emitSignal();
			}
		}
	}
}

void HyperVisorThread::handleHyperVisorStart(StartHV *hvstart)
{
    if (IPvXAddress(hvstart->getHvaddress()) == IPvXAddress("10.1.0.2")) {
        int a = 0;
        a = a++;
    }
    connIdVMSock = getSocket()->getConnectionId();
	simtime_t starttime = simTime() + getHostModule()->par("hvStartUpTime");
	if (starttime < simTime()) {
		opp_error("Trying to set an impossible start time.");
	}
	check_and_cast<HyperVisorApp *>(getHostModule())->setStartTime(starttime);
	//check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOnAndOff(true);
	check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOn();
	//std::cerr << "DEBUG: Being in Start HV of " << hvstart->getHvaddress() << " at " << simTime().dbl() << "\n";
    //std::cerr << "DEBUG: Beginning startup server " << hvstart->getHvaddress() << " at " << simTime().dbl() << "\n";
	scheduleAt(starttime, hvstart);
	//scheduleAt(simTime()+getHostModule()->par("hvStartUpTime"),hvstart);
}

void HyperVisorThread::handleHyperVisorShutDown(ShutDownHV *hvstop)
{
    if (IPvXAddress(hvstop->getHvaddress()) == IPvXAddress("10.1.0.2")) {
        int a = 0;
        a = a++;
    }
    connIdVMSock = getSocket()->getConnectionId();
	// in case one vm is still on or getting started, don't delay msg.
	if (!check_and_cast<HyperVisorApp*>(getHostModule())->areAllVMsOff()) {
		scheduleAt(simTime(),hvstop);
        //std::cerr << "DEBUG: StillVms on in " << hvstop->getHvaddress() << " at " << simTime().dbl() << "\n";
	} else {
	    //check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOnAndOff(true);
	    check_and_cast<HyperVisorApp*>(getHostModule())->emitPoweringOff();
	    //std::cerr << "DEBUG: Being in Stop HV of " << hvstop->getHvaddress() << " at " << simTime().dbl() << "\n";
		scheduleAt(simTime()+getHostModule()->par("hvPowerDownTime"), hvstop);
	}
}
