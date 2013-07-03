//
// Author: Michael Klopf
// 
//#define FSM_DEBUG
#include "VirtualMachineThread.h"
#include "ReplyMsg_m.h"
#include <IPAddressResolver.h>
#include <IPvXAddress.h>
#include <IInterfaceTable.h>
#include <IPAddress.h>
#include <ConfigRouting.h>
#include <InterfaceEntry.h>
#include <IPv4InterfaceData.h>
#include "HyperVisorApp.h"
#include "StartHV_m.h"
#include "ShutDownHV_m.h"

#define BUFFER 1	// delay vmstart a little bit

Register_Class(VirtualMachineThread);

void VirtualMachineThread::established()
{
    this->fsm = dynamic_cast<VirtualMachineApp*>(hostmod)->getFSMVM();
    //this->isReq = false;
    this->isVM1 = false;
    this->isVM2 = false;
    this->comingFromHV = false;
    this->waitingForStartTimeSet = false;
    this->waitingForHVStartUp = false;
    this->serverIsShuttingDown = false;
    socketMap = getHostModule()->getSocketMap();
    //this->socketMap = dynamic_cast<VirtualMachineApp*>(hostmod)->getSocketMap();
}

void VirtualMachineThread::dataArrived(cMessage *msg, bool)
{
    // Check if request msg from controller.
    RequestMsg *reqmsg = dynamic_cast<RequestMsg *>(msg);

    if (reqmsg) {
    	// check if request fits to offered service port
    	if (reqmsg->getServiceport() != dynamic_cast<VirtualMachineApp*>(getHostModule())->getServicePort()) {
    		opp_error("The virtual machine can't process this service request.");
    	}
        //dynamic_cast<VirtualMachineApp*>(hostmod)->setVMstate(true); // correct state?
        handleRequestMsg(reqmsg);
        return;
    }

    // Check if msg requests a start up of vm.
    StartVM *startvm = dynamic_cast<StartVM *>(msg);

    if (startvm) {
        isReq = true;
        //std::cerr << "DEBUG: Being in Start VM of " << startvm->getVmaddress() << " at " << simTime().dbl() << "\n";
        if (IPvXAddress(startvm->getVmaddress()) == IPvXAddress("20.0.0.7")) {
                    int a = 0;
                    a = a++;
                }
    	handleVMStartUp(startvm);
    	return;
    }

    // Check if msg requests to shut down the vm.
    ShutDownVM *stopvm = dynamic_cast<ShutDownVM *>(msg);

    if (stopvm) {
        isReq = true;
        //std::cerr << "DEBUG: Being in ShutDown VM of " << stopvm->getVmaddress() << " at " << simTime().dbl() << "\n";
    	handleVMShutDown(stopvm);
    	return;
    }

    // Check if msg is coming from hv to start up vm.
    StartHV *starthv = dynamic_cast<StartHV *>(msg);

    if (starthv) {
        if (IPvXAddress(starthv->getHvaddress()) == IPvXAddress("20.0.0.7")) {
            int a = 0;
            a = a++;
        }

    	this->comingFromHV = true;	// set flag to make correct decision later

    	// set flag, signaling that ini is over. HV is now on.
    	HyperVisorApp *hvapp = dynamic_cast<HyperVisorApp *>(getHostModule()->getParentModule()->
    				getParentModule()->getSubmodule("hypervisor",0)->getSubmodule("tcpApp",0));
    	hvapp->setStartUpIni(false);

    	// Get startvm msg and handle it accordingly
    	StartVM *startvmmsg = check_and_cast<StartVM *>(starthv->decapsulate());
    	startvmmsg->setServerState(starthv->getServerState());

        //std::cerr << "DEBUG: Being in Start HV of " << startvmmsg->getVmaddress() << " at " << simTime().dbl() << "\n";

    	handleVMStartUp(startvmmsg);
    	//getSocket()->close();	//close socket to hypervisor
    	delete starthv;
    	return;
    }

    // Check if msg is coming from hv which shut down server.
    ShutDownHV *stophv = dynamic_cast<ShutDownHV*>(msg);

    if (stophv) {
    	ShutDownVM *stoppedvm = check_and_cast<ShutDownVM*>(stophv->decapsulate());

        //std::cerr << "DEBUG: Being in ShutDown HV of " << stoppedvm->getVmaddress() << " at " << simTime().dbl() << "\n";
        if (IPvXAddress(stoppedvm->getVmaddress()) == IPvXAddress("20.0.0.6")) {
            int a = 0;
            a = a++;
        }
    	stoppedvm->setServerState(stophv->getServerState());

		TCPSocket *consock = getHostModule()->getSocketMap().findSocketForConnId(stoppedvm->getConnIDtoCon());
        if (!consock) {
        	opp_error("Could not find socket in socket map. HV data received.");
        }

    	stoppedvm->removeControlInfo();
		consock->send(stoppedvm);

		//getSocket()->close(); // hypervisor
		delete stophv;
		return;
    }

    // Check if msg does migration related task.
    VMMigMsg *vmmsg = dynamic_cast<VMMigMsg *>(msg);

    if (vmmsg) {
        handleVirtualMachineMigration(vmmsg);
        changeIPMsg = new VMMigMsg(*vmmsg);
        return;
    }

    if (!vmmsg)
        opp_error("Message (%s)%s is not a RequestMsg or VMMsg -- "
                  "probably wrong client app, or wrong setting of TCP's "
                  "sendQueueClass/receiveQueueClass parameters "
                  "(try \"TCPMsgBasedSendQueue\" and \"TCPMsgBasedRcvQueue\")",
                  msg->getClassName(), msg->getName());
}

void VirtualMachineThread::timerExpired(cMessage *timer)
{

	StartVM *startvm = dynamic_cast<StartVM *>(timer);

	if (startvm) {

        //std::cerr << "DEBUG: Being in Start VM timerexp of " << startvm->getVmaddress() << " at " << simTime().dbl() << "\n";
	    if (IPvXAddress(startvm->getVmaddress()) == IPvXAddress("20.0.0.7")) {
	        int a = 0;
	        a = a++;
	    }
	    if (serverIsShuttingDown) {
	        startvm->setServerState(false);
            // send back to controller.
            startvm->removeControlInfo();
            getSocket()->send(startvm);
            return;
	    } else if (comingFromHV) {
			dynamic_cast<VirtualMachineApp *>(getHostModule())->startUpVM();
	        check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOn();
	        //std::cerr << "DEBUG: coming from hv is on, starting " << startvm->getVmaddress() << " at " << simTime().dbl() << "\n";
			TCPSocket *consock = getHostModule()->getSocketMap().findSocketForConnId(startvm->getConnIDtoCon());
            if (!consock) {
            	opp_error("Could not find socket in socket map. HV data received.");
            }

			startvm->removeControlInfo();
			consock->send(startvm);
			comingFromHV = false;
		} else if (waitingForStartTimeSet) {
			HyperVisorApp *hvapp = dynamic_cast<HyperVisorApp *>(getHostModule()->getParentModule()->
					getParentModule()->getSubmodule("hypervisor",0)->getSubmodule("tcpApp",0));
			if (hvapp->getStartTime() < simTime()) {
				scheduleAt(simTime()+getHostModule()->par("delayMsg"), startvm);
				return;
			}
			//startvm->setServerState(true); // it's on now
			simtime_t starttime = hvapp->getStartTime(); //+ getHostModule()->par("vmPowerUpTime") + BUFFER;
	        //check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOn();
			scheduleAt(starttime, startvm);
			waitingForStartTimeSet = false;
			waitingForHVStartUp = true;
			return;
		} else if (waitingForHVStartUp) {   // notice: This case should be replaced by something smarter.
	        startvm->setServerState(true); // it's on
	        check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOn();
	        //std::cerr << "DEBUG: hv is now on, starting " << startvm->getVmaddress() << " at " << simTime().dbl() << "\n";
	        scheduleAt(simTime()+getHostModule()->par("vmPowerUpTime"), startvm);
            waitingForHVStartUp = false;
            return;
		} else {
			check_and_cast<VirtualMachineApp *>(getHostModule())->startUpVM();
			check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOn();
			// send back to controller.
			startvm->removeControlInfo();
			getSocket()->send(startvm);
		}
        //std::cerr << "DEBUG: Finishing startup vm " << startvm->getVmaddress() << " at " << simTime().dbl() << "\n";
		check_and_cast<VirtualMachineApp*>(getHostModule())->setStartUpRequested(false);
		return;
	}

	ShutDownVM *stopvm = dynamic_cast<ShutDownVM *>(timer);

	if (stopvm) {
        //std::cerr << "DEBUG: Being in ShutDown VM timerexp of " << stopvm->getVmaddress() << " at " << simTime().dbl() << "\n";
        if (IPvXAddress(stopvm->getVmaddress()) == IPvXAddress("20.0.0.6")) {
            int a = 0;
            a = a++;
        }
		// check if vm is in busy state.
		VirtualMachineApp *vmapp = check_and_cast<VirtualMachineApp *>(getHostModule());
		int fsmstate = vmapp->getFSMVM()->getState();

		if (fsmstate == BUSY) {
			opp_error("VM is busy, want to shut it down.");
		}

		vmapp->shutDownVM();
		check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOff();

		// check if last vm and shut down server if true. TODO beware of VMs which get a wake up msg during shutdown.

		//hvapp->shutDownServer();
        cModule *hv = getHostModule()->getParentModule()->
                getParentModule()->getSubmodule("hypervisor",0);

        IPvXAddress addr = IPAddressResolver().addressOf(hv, IPAddressResolver::ADDR_IPv4);

	    ShutDownHV *stophv = new ShutDownHV("StopServer");
	    stophv->setHvaddress(addr.str().c_str());
	    stophv->setByteLength(10);

	    stopvm->setConnIDtoCon(getSocket()->getConnectionId());
        TCPSocket *consock = getHostModule()->getSocketMap().findSocketForConnId(stopvm->getConnIDtoCon());
        if (!consock) {
            opp_error("Could not find socket in socket map. HV data received.");
        }

	    stophv->encapsulate(stopvm);

	    vmapp->getHvSocket()->send(stophv);
		return;
	}

    VMMigMsg *vmmsg = dynamic_cast<VMMigMsg *>(timer);

    if (vmmsg) {
    	// when vmmigration requested then proceed else directly handle vmmsg.
    	if (dynamic_cast<VirtualMachineApp*>(getHostModule())->getVMMigRequest()) {
        	// But check if socket of last job request is closed.
        	if (dynamic_cast<VirtualMachineApp*>(getHostModule())->isReqThreadClosed()) {
                handleVirtualMachineMigration(vmmsg);
                dynamic_cast<VirtualMachineApp*>(getHostModule())->setVMMigRequest(false);
                return;
        	} else {
        		// delay migration for some time
        		scheduleAt(simTime()+getHostModule()->par("delayMsg"), vmmsg);
        		EV << "I delayed the migration, because request socket is not closed.";
        		return;
        	}
    	} else {
            handleVirtualMachineMigration(vmmsg);
            return;
    	}
    }

    ReplyMsg *replyMsg = dynamic_cast<ReplyMsg*>(timer);

    if (replyMsg) {
        EV << "VM Setting back to IDLE \n";
        // Set back to IDLE state.
        FSM_Goto(*fsm, IDLE);
        //std::cerr << "Going idle in " << replyMsg->getVmaddress() << "\n";
        dynamic_cast<VirtualMachineApp*>(getHostModule())->emitVMBusy();

    	EV << "SCT: 5. Sending " << replyMsg->getClassName() << ".\n";
        // Send out Reply.
        getSocket()->send(timer);
    }
}

void VirtualMachineThread::handleRequestMsg(RequestMsg *reqmsg)
{
	isReq = true;
	// for socket closing procedure during vm ip-address change we need to observe
	// the socket of the last RequestMsg. As it has to be closed before address is changed.
	dynamic_cast<VirtualMachineApp*>(getHostModule())->setReqThreadClosed(false);
	EV << "SCT: 4. Receiving " << reqmsg->getClassName() << ".\n";
    // process message: send back requested number of bytes, then close
    // connection if that was requested too
    long requestedBytes = reqmsg->getExpectedReplyLength();
    double replyDelay = reqmsg->getReplyDelay();

    // Create new Message from type replyMsg
    ReplyMsg *replyMsg = new ReplyMsg("reply");
    replyMsg->setByteLength(requestedBytes);
    replyMsg->setServerClose(reqmsg->getServerClose());
    replyMsg->setConnID(reqmsg->getConnID());
    replyMsg->setServiceport(reqmsg->getServiceport());
    replyMsg->setDepTime(reqmsg->getDepTime());

    // Save ip address of vm to remove vm from active vm list.
    IInterfaceTable *ift = IPAddressResolver().interfaceTableOf(getHostModule()->getParentModule());
    int addrType = IPAddressResolver::ADDR_IPv4;
    IPvXAddress addr = IPAddressResolver().getAddressFrom(ift,addrType);
    replyMsg->setVmaddress(addr.str().c_str());

    delete reqmsg;

    // Virtual machine must be in IDLE or INIT state at this point.
    if (fsm->getState() != INIT && fsm->getState() != IDLE) {
        opp_error("invalid state, it's not IDLE or INIT (= %i), it's %i", INIT, fsm->getState());
    }
    EV << "VM Setting BUSY \n";
    // Set state to busy.
    FSM_Goto(*fsm,BUSY);
    //std::cerr << "Going busy in " << replyMsg->getVmaddress() << "\n";
    dynamic_cast<VirtualMachineApp*>(getHostModule())->emitVMBusy();
    // Send back with delay
    scheduleAt(simTime()+replyDelay, replyMsg);
}

void VirtualMachineThread::handleVirtualMachineMigration(VMMigMsg *vmmsg)
{
    FSM_Switch(*fsm) {
        // Initializing virtual machine. Either go to idle or off state.
        case FSM_Exit(INIT):
            // Check if vm active or inactive.
            if (dynamic_cast<VirtualMachineApp*>(hostmod)->getVMState()) {
                scheduleAt(simTime(), vmmsg);
                FSM_Goto(*fsm, IDLE);
            } else {
                scheduleAt(simTime(), vmmsg);
                FSM_Goto(*fsm, VMOFF);
            }
        break;

        // State when virtual machine has no work to do.
        case FSM_Exit(IDLE):
            // Turning off virtual machine when vm1.
            if (vmmsg->getMsgtype() == TURNOFFVM1) {
                check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOff();
                //check_and_cast<VirtualMachineApp*>(getHostModule())->getHvSocket()->close();
                scheduleAt(simTime()+getHostModule()->par("vmPowerDownTime"),vmmsg);
                vmmsg->setMsgtype(VM1TURNEDOFF);
                FSM_Goto(*fsm, VMOFF);
                break;
            }
            // Started up virtual machine when vm2.
            if (vmmsg->getMsgtype() == VM2TURNEDON) {
                // Set flag that vm2 is on. Emit signal to statistics.
                VirtualMachineApp *vm = dynamic_cast<VirtualMachineApp*>(getHostModule());
                vm->setVMstate(true);
                vm->emitPoweringOn();
                vm->emitSignal();
                vmmsg->removeControlInfo();
                getSocket()->send(vmmsg); // Send answer to hv1.
                //FSM_Goto(*fsm, INIT); // vm2 has finished fsm actions.
            } else {
                opp_error("Invalid state in IDLE-VM state.");
            }
        break;

        // State where virtual machine has work to do.
        case FSM_Exit(BUSY):
            if (vmmsg->getMsgtype() == TURNOFFVM1) {
                // Set flag, to show virtual machine migration request. Save mig msg.
            	dynamic_cast<VirtualMachineApp*>(getHostModule())->setVMMigRequest(true);
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Busy, but will turn off soon!");
                scheduleAt(simTime()+getHostModule()->par("delayMsg"), vmmsg);
            } else {
                opp_error("Invalid state in BUSY state.");
            }
        break;

        // State when virtual machine is off.
        case FSM_Exit(VMOFF):
            // Should get no turn off msg, when already off.
            if (vmmsg->getMsgtype() == TURNOFFVM1) {
                opp_error("invalid event in state VMOFF");
            }
            // Call back to hv1 that vm1 successfully turned off.
            if (vmmsg->getMsgtype() == VM1TURNEDOFF) {
                // Set flag, that vm1 is off. Emit signal to statistic.
                VirtualMachineApp *vm = dynamic_cast<VirtualMachineApp*>(getHostModule());
                vm->setVMstate(false);
                vm->emitPoweringOff();
                vm->emitSignal();
                vmmsg->removeControlInfo();
                getSocket()->send(vmmsg);
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Turned off VM1!");
            	isVM1 = true;
                check_and_cast<VirtualMachineApp*>(getHostModule())->getHvSocket()->close();
            	//FSM_Goto(*fsm, INIT); // vm1 has finished fsm actions.
                break;
            }
            // Start virtual machine.
            if (vmmsg->getMsgtype() == STARTVM2) {
            	this->isVM2 = true;
            	check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOn();
            	check_and_cast<VirtualMachineApp*>(getHostModule())->getHvSocket()->close();
                scheduleAt(simTime()+getHostModule()->par("vmPowerUpTime"), vmmsg);
                vmmsg->setMsgtype(VM2TURNEDON);
                FSM_Goto(*fsm, IDLE);
                if (ev.isGUI())
                    getHostModule()->getParentModule()->bubble("Turned on VM2!");
            } else {
                opp_error("Invalid state in VMOFF state.");
            }
        break;
    }

}

void VirtualMachineThread::closed()
{
    /*
    const char *addrchar1 = getSocket()->getLocalAddress().str().c_str();
    const char *addrchar = getSocket()->getRemoteAddress().str().c_str();
    std::cerr << "Removing socket vm " << addrchar1 << " and " << addrchar << "\n";
    */
	//EV << "SCTVM: I close the VM.\n";
	if (isVM1) {
		if (!changeIPMsg) {
			opp_error("VMMig msg has been deleted.");
		}
		setIPAddressForVM1(changeIPMsg);
		check_and_cast<VirtualMachineApp*>(getHostModule())->updateHVSocket();
		getHostModule()->removeThread(this);
	}
	if (isVM2) {
		if (!changeIPMsg) {
			opp_error("VMMig msg has been deleted.");
		}
		setIPAddressForVM2(changeIPMsg);
        check_and_cast<VirtualMachineApp*>(getHostModule())->updateHVSocket();
		initiateReRouting(changeIPMsg);
		delete changeIPMsg;
		getHostModule()->removeThread(this);
	}
	if (isReq) {
        dynamic_cast<VirtualMachineApp*>(getHostModule())->setReqThreadClosed(true);
		getHostModule()->removeThread(this);
	}

	//delete socketMap;
}

void VirtualMachineThread::peerClosed()
{
    /*
    const char *addrchar1 = getSocket()->getLocalAddress().str().c_str();
    const char *addrchar = getSocket()->getRemoteAddress().str().c_str();
    std::cerr << "Peer Closed vm" << addrchar1 << " and " << addrchar << "\n";
    */
    /*
    if (getSocket()->getLocalAddress() == IPvXAddress()) {
        return;
    } else if (getSocket()->getRemoteAddress() == IPvXAddress()) {
        return;
    }*/
	//EV << "SCTVM: Trying to hack VMThread.\n";
	if (isReq) {
		dynamic_cast<VirtualMachineApp*>(getHostModule())->setReqThreadClosed(true);
		getSocket()->close();
	}

	if (isVM1 || isVM2) {
		if (getSocket()->getState()==TCPSocket::PEER_CLOSED)
		{
			EV << "remote TCP closed, closing here as well\n";
			getSocket()->close();
		}
	}/*
	if (getSocket()->getState()==TCPSocket::PEER_CLOSED)
	{
	    EV << "remote TCP closed, closing here as well\n";
	    getSocket()->close();
	}*/
}

void VirtualMachineThread::setIPAddressForVM2(VMMigMsg *vmmsg)
{
    IPAddress *addr = new IPAddress(vmmsg->getVm1address());

    // Write Address in interface table.
    IInterfaceTable *ift = IPAddressResolver().interfaceTableOf(getHostModule()->getParentModule());
    for (int j=0; j<ift->getNumInterfaces(); j++)
    {
        InterfaceEntry *ie = ift->getInterface(j);
        if (!ie->isLoopback()) //check for loopback address
        {
            ie->ipv4Data()->setIPAddress(*addr);
            ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS);
            // full address must match for local delivery
        }
    }

	//EV << "Gotcha VM2 address is " << addr->str().c_str() << ".\n";
    // Set display string of virtual machine.
    if (ev.isGUI()) {
        // update display string
        getHostModule()->getParentModule()->getDisplayString().setTagArg("t",0,addr->str().c_str());
    }
}

void VirtualMachineThread::setIPAddressForVM1(VMMigMsg *vmmsg)
{
	IPAddress *addr = new IPAddress(vmmsg->getVm2address());
    //IPAddress addr = IPAddress(getNewVMIP());

    // Write Address in interface table.
    IInterfaceTable *ift = IPAddressResolver().interfaceTableOf(getHostModule()->getParentModule());
    for (int j=0; j<ift->getNumInterfaces(); j++)
    {
        InterfaceEntry *ie = ift->getInterface(j);
        if (!ie->isLoopback()) //check for loopback address
        {
            ie->ipv4Data()->setIPAddress(*addr);
            ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS);
            // full address must match for local delivery
        }
    }

    // Set display string of virtual machine.
    if (ev.isGUI()) {
        // update display string
        getHostModule()->getParentModule()->getDisplayString().setTagArg("t",0,addr->str().c_str());
    }
}

void VirtualMachineThread::initiateReRouting(VMMigMsg *vmmsg)
{
    // Rerouting
    ConfigRouting *router = new ConfigRouting();
    router->refreshRoutingTables(vmmsg->getVm1address(), vmmsg->getVm2address());
    EV << "Finished ReRouting.\n";
    delete router;
}

void VirtualMachineThread::handleVMStartUp(StartVM *startvm)
{
    if (IPvXAddress(startvm->getVmaddress()) == IPvXAddress("20.0.0.7")) {
        int a = 0;
        a = a++;
    }
    HyperVisorApp *hvapp = dynamic_cast<HyperVisorApp *>(getHostModule()->getParentModule()->
            getParentModule()->getSubmodule("hypervisor",0)->getSubmodule("tcpApp",0));

    // Server shutting down, don't start vm then.
    if (hvapp->getShutDownIni()) {
        serverIsShuttingDown = true;
        scheduleAt(simTime(), startvm);
        return;
    }

	check_and_cast<VirtualMachineApp*>(getHostModule())->setStartUpRequested(true);
	// If server is off and start up not yet requested, turn it on.
	if (!hvapp->getHVState() && !hvapp->getStartUpIni()) {	// wrong and wrong - off and no ini
		// set value, to show that start up requested.
		hvapp->setStartUpIni(true);

		cModule *hv = getHostModule()->getParentModule()->
				getParentModule()->getSubmodule("hypervisor",0);

		IPvXAddress addr = IPAddressResolver().addressOf(hv, IPAddressResolver::ADDR_IPv4);

	    StartHV *hvstart = new StartHV("StartServer");
	    hvstart->setHvaddress(addr.str().c_str());
	    hvstart->setByteLength(10);

	    startvm->setConnIDtoCon(getSocket()->getConnectionId());
	    // pack msg into hvstart to send it back later
	    hvstart->encapsulate(startvm);

	    check_and_cast<VirtualMachineApp*>(getHostModule())->getHvSocket()->send(hvstart);

        //std::cerr << "DEBUG: StartVM send msg to HV " << hvstart->getHvaddress() << " at " << simTime().dbl() << "\n";

	    if (comingFromHV) {
	    	opp_error("Starting HV up again, shouldn't happen.");
	    }
	} else if (!hvapp->getHVState() && hvapp->getStartUpIni()) {	// wrong and true - off and ini sent
		if (hvapp->getStartTime() < simTime()) {
			// delay the msg, because other vm did not send msg to hv yet.
			waitingForStartTimeSet = true;
			scheduleAt(simTime()+getHostModule()->par("delayMsg"), startvm);
			return;
		}
		waitingForHVStartUp = true;
		//startvm->setServerState(true); // it's on now
		simtime_t starttime = hvapp->getStartTime(); //+ getHostModule()->par("vmPowerUpTime") + BUFFER;
        //check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOn();
        //std::cerr << "DEBUG: Hv starting, got starttime, delay msg at " << startvm->getVmaddress() << " at " << simTime().dbl() << "\n";
		scheduleAt(starttime, startvm);
	} else {	// true and wrong - it's on - or true and true (shouldn't happen)
		// Else, start up the VM.
        //std::cerr << "DEBUG: hv is on, starting " << startvm->getVmaddress() << " at " << simTime().dbl() << "\n";
		startvm->setServerState(true); // it's on
		check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOn();
		scheduleAt(simTime()+getHostModule()->par("vmPowerUpTime"), startvm);
	}
}

void VirtualMachineThread::handleVMShutDown(ShutDownVM *stopvm)
{
    if (IPvXAddress(stopvm->getVmaddress()) == IPvXAddress("20.0.0.7")) {
           double zeit = simTime().dbl();
           int a = 0;
           a = a++;
    }
	// Delay powering down.
    check_and_cast<VirtualMachineApp*>(getHostModule())->emitPoweringOff();
	scheduleAt(simTime()+getHostModule()->par("vmPowerDownTime"), stopvm);
}
