//
// Author: Michael Klopf
// 

#include "VirtualMachineApp.h"
#include <IInterfaceTable.h>
#include <IPAddressResolver.h>
#include <IPAddress.h>
#include <ConfigRouting.h>
#include <InterfaceEntry.h>
#include <IPv4InterfaceData.h>
#include "HyperVisorApp.h"

#define MINUSONE -1
//#define ACTIVE 1

Define_Module(VirtualMachineApp);

void VirtualMachineApp::initialize(int stage)
{
	if (stage == 1) {
	    TCPSrvHostApp::initialize();
	    this->HTTPPORT = par("HTTPPORT");
	    this->FTPPORT = par("FTPPORT");

	    this->vmON = par("vmON");

	    this->fsmVM = new cFSM();

	    this->fsmVM->setName("vmfsm");

	    this->vmOnSignal = registerSignal("vmState");
	    this->busyState = registerSignal("busyState");
	    this->idleState = registerSignal("idleState");
	    this->offState = registerSignal("offState");
	    this->startUpState = registerSignal("powerUpState");
	    this->shutDownState = registerSignal("powerDownState");

	    this->reqThreadClosed = true;
	    // This value is for the BUSY state and handling of incoming task.
	    this->vmMigrationRequest = false;

	    this->startUpRequested = false;

	    //this->isReqMsg = false;

	    if (ev.isGUI()) {
		    if (this->vmON) {
			    getParentModule()->getDisplayString().setTagArg("i2",0,"status/active");
		    } else {
			    getParentModule()->getDisplayString().setTagArg("i2",0,"status/asleep");
		    }
	    }

	    this->controller = check_and_cast<DCTCPControllerApp*>(findControllerModule());

	    if (this->vmON) {
            controller->emitEnergyConsumption((int)par("idlePower"));
	    	emit(vmOnSignal, (long)vmON);
	    	emit(idleState, true);
	    } else {
	        emit(offState, true);
	    }

	    // assign a picture showing which service is running
	    setServiceLogo();

	} else if (stage == 2) {
        // Create socket to hypervisor
        cModule *hv = getParentModule()->
                getParentModule()->getSubmodule("hypervisor",0);

        IPvXAddress addr = IPAddressResolver().addressOf(hv, IPAddressResolver::ADDR_IPv4);

        cModule*vm = getParentModule();
        IPvXAddress vmaddr = IPAddressResolver().addressOf(vm, IPAddressResolver::ADDR_IPv4);

        hvSocket = new TCPSocket();
        hvSocket->setOutputGate(gate("tcpOut"));

        const char *serverThreadClass = par("serverThreadClass");
        DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
        hvSocket->setCallbackObject(proc);
        proc->init(this, hvSocket);

        int vmport = (int)par("port");
        hvSocket->bind(vmaddr, vmport);
        int hvport = (int)par("connectToHvPort");
        hvSocket->connect(addr, hvport);
        SocketMapExt socketMap = getSocketMap();
        socketMap.addSocket(hvSocket);
	}
}

void VirtualMachineApp::handleMessage(cMessage *msg)
{
    DCTCPControllerApp::handleMessage(msg);
}

void VirtualMachineApp::finish()
{
    delete this->fsmVM;
    delete this->hvSocket;
}

void VirtualMachineApp::emitSignal()
{
	emit(vmOnSignal, (long)vmON);

    if (ev.isGUI()) {
	    if (this->vmON) {
		    getParentModule()->getDisplayString().setTagArg("i2",0,"status/active");
	    } else {
		    getParentModule()->getDisplayString().setTagArg("i2",0,"status/asleep");
	    }
    }

	if (this->vmON) {
		//controller->emitEnergyConsumption((int)par("idlePower")); // add power cons. of active vm
		//emit(offState, false);
		//emit(idleState, true);
	} else {
		//controller->emitEnergyConsumption(MINUSONE*(int)par("idlePower")); // subtract pc of inactive vm
		//emit(idleState, false);
		//emit(offState, true);
	}
}

void VirtualMachineApp::emitVMBusy()
{
	if (fsmVM->getState() == IDLE) {
		controller->emitEnergyConsumption(MINUSONE*(int)par("busyPower")); // subtract pc of now idle vm
		emit(busyState, false);
		emit(idleState, true);

	} else if (fsmVM->getState() == BUSY) {
		controller->emitEnergyConsumption((int)par("busyPower")); // add pc of busy vm
        emit(idleState, false);
        emit(busyState, true);

	}
}

void VirtualMachineApp::emitPoweringOn()
{
    cModule *vm = getParentModule();

    IPvXAddress addr = IPAddressResolver().addressOf(vm, IPAddressResolver::ADDR_IPv4);
    if (vmON) {
        //std::cerr << "DEBUG: Finishing start up vm " << addr << "\n";
        controller->emitEnergyConsumption(MINUSONE*(int)par("startUpPower"));
        emit(startUpState, false);
        emit(idleState, true);
        HyperVisorApp *hvapp = dynamic_cast<HyperVisorApp *>(getParentModule()->
                getParentModule()->getSubmodule("hypervisor",0)->getSubmodule("tcpApp",0));
        hvapp->emitHVBusy(true);
    } else {
        //std::cerr << "DEBUG: Beginning start up vm " << addr << "\n";
        controller->emitEnergyConsumption((int)par("idlePower")+(int)par("startUpPower"));
        emit(offState, false);
        emit(startUpState, true);
    }
}

void VirtualMachineApp::emitPoweringOff()
{
    cModule *vm = getParentModule();

    IPvXAddress addr = IPAddressResolver().addressOf(vm, IPAddressResolver::ADDR_IPv4);
    if (vmON) {
        //std::cerr << "DEBUG: Beginning shut down vm " << addr << "\n";
        // delete idle power, then add shut down power
        //controller->emitEnergyConsumption(MINUSONE*(int)par("idlePower"));
        controller->emitEnergyConsumption((int)par("shutDownPower"));
        emit(idleState, false);
        emit(shutDownState, true);
    } else {
        //std::cerr << "DEBUG: Finishing shut down of vm " << addr << "\n";
        controller->emitEnergyConsumption(MINUSONE*((int)par("shutDownPower")+(int)par("idlePower")));
        //controller->emitEnergyConsumption((int)par("idlePower"));
        emit(shutDownState, false);
        emit(offState, true);
        HyperVisorApp *hvapp = dynamic_cast<HyperVisorApp *>(getParentModule()->
                getParentModule()->getSubmodule("hypervisor",0)->getSubmodule("tcpApp",0));
        hvapp->emitHVBusy(false);
    }
}

void VirtualMachineApp::setServiceLogo()
{
	if (getServicePort() == HTTPPORT) {
		getParentModule()->getDisplayString().setTagArg("i",0,"block/browser");
	} else if (getServicePort() == FTPPORT) {
		getParentModule()->getDisplayString().setTagArg("i",0,"block/ftp");
	} else {
		getParentModule()->getDisplayString().setTagArg("i",0,"block/app2");
	}
}

void VirtualMachineApp::startUpVM()
{
	if (this->vmON) {
		opp_warning("VM is already on.");
	}
	if ((fsmVM->getState() != VMOFF) && (fsmVM->getState() != INIT)) { // 1,0 is from VMThread, solve enum problem.
		opp_error("VM start up initialized but fsm is not in OFF or INIT state.");
	}
	vmON = true;
	FSM_Goto(*fsmVM, IDLE);
	emitSignal();
}

void VirtualMachineApp::shutDownVM()
{
	if (!this->vmON) {
		opp_warning("VM is already off.");
	}
	vmON = false;
	FSM_Goto(*fsmVM, VMOFF);
	emitSignal();
}

void VirtualMachineApp::updateHVSocket()
{
    hvSocket->renewSocket();
    cModule *hv = getParentModule()->
            getParentModule()->getSubmodule("hypervisor",0);

    IPvXAddress addr = IPAddressResolver().addressOf(hv, IPAddressResolver::ADDR_IPv4);


    cModule*vm = getParentModule();
    IPvXAddress vmaddr = IPAddressResolver().addressOf(vm, IPAddressResolver::ADDR_IPv4);


    hvSocket->setOutputGate(gate("tcpOut"));

    const char *serverThreadClass = par("serverThreadClass");
    DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
    hvSocket->setCallbackObject(proc);
    proc->init(this, hvSocket);

    int vmport = (int)par("port");
    hvSocket->bind(vmaddr, vmport);
    int hvport = (int)par("connectToHvPort");
    hvSocket->connect(addr, hvport);
}
