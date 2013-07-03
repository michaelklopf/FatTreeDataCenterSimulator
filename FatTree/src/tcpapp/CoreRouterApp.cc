//
// Author: Michael Klopf
// 

#include "CoreRouterApp.h"
#include "DCTCPControllerApp.h"

Define_Module(CoreRouterApp);

void CoreRouterApp::initialize(int stage)
{
	if (stage == 2) {
	    DCTCPControllerApp *conApp = check_and_cast<DCTCPControllerApp *>(findControllerModule());

	    // emit SwitchPower - we assume it is always on therefore only positive value.
	    conApp->emitEnergyConsumption((int)par("powerConSwitch"));

	    // emit CablePower - we assume it's always on.
	    conApp->emitEnergyConsumption((int)par("k")*(int)par("powerConCable"));
	}
}

void CoreRouterApp::handleMessage(cMessage *msg)
{
    // nothing to do yet
}

cModule *CoreRouterApp::findControllerModule()
{
	return	getParentModule()->	// get core router
			getParentModule()-> // get FatTree
			getSubmodule("Controller",0)-> // get controller
			getSubmodule("TrafficController", 0)-> // get traffic controller
			getSubmodule("tcpApp", 0); // get tcp app
}
