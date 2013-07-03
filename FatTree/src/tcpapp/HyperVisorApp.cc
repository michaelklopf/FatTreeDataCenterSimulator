//
// Author: Michael Klopf
// 

#include "HyperVisorApp.h"
#include "VirtualMachineApp.h"
#include "IPAddressResolver.h"

#define SERVEROFF  -1
#define SERVERON    1
#define MINUSONE   -1

Define_Module(HyperVisorApp);

void HyperVisorApp::initialize(int stage)
{
    if (stage == 1) {
        this->hvOnSignal = registerSignal("hvState");
        this->busyState = registerSignal("busyState");
        this->idleState = registerSignal("idleState");
        this->offState = registerSignal("offState");
        this->startUpState = registerSignal("powerUpState");
        this->shutDownState = registerSignal("powerDownState");
    }
	if (stage == 2) {
	    TCPSrvHostApp::initialize();

	    this->hvON = par("hvON");

	    this->startUpIni = false;

	    this->shutDownIni = false;

	    this->startTime = 0;

	    this->fsmHV = new cFSM();

	    this->fsmHV->setName("hvfsm");

	    if (ev.isGUI()) {
		    if (this->hvON) {
			    getParentModule()->getDisplayString().setTagArg("i2",0,"status/active");
		    } else {
			    getParentModule()->getDisplayString().setTagArg("i2",0,"status/asleep");
		    }
	    }
	    // Get controller and initialize the statistic there.
	    this->controller = check_and_cast<DCTCPControllerApp*>(findControllerModule());

	    if (this->hvON) {
		    controller->emitActiveServerSignal(SERVERON);
		    controller->emitEnergyConsumption((int)par("idlePower"));
		    emit(idleState, true);
	    } else {
		    //controller->emitActiveServerSignal(SERVEROFF);
	        emit(offState, true);
	    }
	    // Initialize on states.
	    if (this->hvON) {
	    	emit(hvOnSignal, (long)hvON);
	    }
	}
}

void HyperVisorApp::finish()
{
	//emitSignal();
    //emit(idleState, false);
    //emit(offState, true);
    delete this->fsmHV;
}

void HyperVisorApp::emitSignal()
{
	// set statistic local
	emit(hvOnSignal, (long)hvON);

    if (ev.isGUI()) {
	    if (this->hvON) {
		    getParentModule()->getDisplayString().setTagArg("i2",0,"status/active");
	    } else {
		    getParentModule()->getDisplayString().setTagArg("i2",0,"status/asleep");
	    }
    }
    // set global statistic
    if (this->hvON) {
    	controller->emitActiveServerSignal(SERVERON);
    	//controller->emitEnergyConsumption((int)par("idlePower"));
    } else {
    	controller->emitActiveServerSignal(SERVEROFF);
    	//controller->emitEnergyConsumption(MINUSONE*(int)par("idlePower"));
    }
}

/*
void HyperVisorApp::emitPoweringOnAndOff(bool processing)
{
    // when beginning shut down or start up, choose first branch.
    if (processing) {
        // start shut down
        if (this->hvON) {
            //std::cerr << "DEBUG: Shutting down hv " << "\n";
            // delete idle power, then add shut down power
            controller->emitEnergyConsumption(MINUSONE*(int)par("idlePower"));
            controller->emitEnergyConsumption((int)par("shutDownPower"));
            emit(idleState, false);
            emit(shutDownState, true);
        } else {    // begin start up
            //std::cerr << "DEBUG: Starting up hv " << "\n";
            controller->emitEnergyConsumption((int)par("startUpPower"));
            emit(offState, false);
            emit(startUpState, true);
        }
    } else {
        // finish start up
        if (this->hvON) {
            //std::cerr << "DEBUG: Finishing start up hv " << "\n";
            controller->emitEnergyConsumption(MINUSONE * (int)par("startUpPower"));
            emit(startUpState, false);
            emit(idleState, true);
        } else {    //finish shut down
            //std::cerr << "DEBUG: Finishing  shutting up hv " << "\n";
            controller->emitEnergyConsumption(MINUSONE * (int)par("shutDownPower"));
            controller->emitEnergyConsumption((int)par("idlePower"));
            emit(shutDownState, false);
            emit(offState, true);
        }
    }
}
*/

void HyperVisorApp::emitPoweringOn()
{
    cModule *hv = getParentModule();

    IPvXAddress addr = IPAddressResolver().addressOf(hv, IPAddressResolver::ADDR_IPv4);
    if (hvON) {
        // if on, finish start up
        //std::cerr << "DEBUG: Finishing start up hv " << addr << "\n";
        controller->emitEnergyConsumption(MINUSONE * (int)par("startUpPower"));
        emit(startUpState, false);
        emit(idleState, true);
    } else {
        // off, start it up
        //std::cerr << "DEBUG: Starting up hv " << addr << "\n";
        controller->emitEnergyConsumption((int)par("idlePower")+(int)par("startUpPower"));
        emit(offState, false);
        emit(startUpState, true);
    }
}

void HyperVisorApp::emitPoweringOff()
{
    cModule *hv = getParentModule();

    IPvXAddress addr = IPAddressResolver().addressOf(hv, IPAddressResolver::ADDR_IPv4);
    if (hvON) {
        shutDownIni = true;
        // begin powering off
        //std::cerr << "DEBUG: Starting  shutting down hv " << addr << "\n";
        //controller->emitEnergyConsumption(MINUSONE*(int)par("idlePower"));
        controller->emitEnergyConsumption((int)par("shutDownPower"));
        emit(idleState, false);
        emit(shutDownState, true);
    } else {
        shutDownIni = false;
        // powered off
        //std::cerr << "DEBUG: Finishing  shutting down hv " << addr << "\n";
        controller->emitEnergyConsumption(MINUSONE * ((int)par("shutDownPower")+(int)par("idlePower")));
        //controller->emitEnergyConsumption((int)par("idlePower"));
        emit(shutDownState, false);
        emit(offState, true);
    }
}

void HyperVisorApp::emitHVBusy(bool isBusy)
{
    if (isBusy) {
        emit(idleState, false);
        emit(busyState, true);
    } else {
        if (areAllVMsOff()) {
            emit(busyState, false);
            emit(idleState, true);
        }
    }
}

void HyperVisorApp::startUpServer()
{
	if (this->hvON) {
		opp_warning("Server is already on.");
	}
	if ((fsmHV->getState() != HVOFF) && (fsmHV->getState() != INIT)) {
		opp_error("HV start up initialized but fsm is not in OFF or INIT state.");
	}
	hvON = true;
	FSM_Goto(*fsmHV, ACTIVE);
	emitSignal();
}

void HyperVisorApp::shutDownServer()
{
	// Go back if a vm is still on or gets started up.
	if (!areAllVMsOff()) {
		//opp_warning("Still VM on.");
		return;
	}
	if (!this->hvON) {
		//opp_warning("Server is already off.");
		return;
	}
	hvON = false;
	FSM_Goto(*fsmHV, HVOFF);
	emitSignal();
}

bool HyperVisorApp::areAllVMsOff()
{
	bool vmsoff = true;
	for (cSubModIterator iter(*getParentModule()->getParentModule()); !iter.end(); iter++) {
		if (iter()->isName("vms")) {
			VirtualMachineApp *vmapp = dynamic_cast<VirtualMachineApp*>(iter()->getSubmodule("tcpApp",0));
			// In case one vm is still on return false
			if (vmapp->getVMState()) {
				vmsoff = false;
			}
			if (vmapp->getStartUpRequested()) {
			    vmsoff = false;
			}
		}
	}
	return vmsoff; // return true, all vms are off
}
