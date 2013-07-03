//
// Author: Michael Klopf
// 

#include "AggRouterApp.h"
#include "DCTCPControllerApp.h"

Define_Module(AggRouterApp);

void AggRouterApp::initialize(int stage)
{
	if (stage == 2) {
	    DCTCPControllerApp *conApp = check_and_cast<DCTCPControllerApp *>(findControllerModule());

	    // emit SwitchPower - we assume it is always on therefore only positive value.
	    conApp->emitEnergyConsumption((int)par("powerConSwitch"));

	    // emit CablePower - we assume it's always on.
	    conApp->emitEnergyConsumption((int)par("k")/2*(int)par("powerConCable"));
	}
}

void AggRouterApp::handleMessage(cMessage *msg)
{
    // nothing to do yet
}

cModule *AggRouterApp::findControllerModule()
{
	return 	getParentModule()-> // get agg router
			getParentModule()-> // get pod
			getParentModule()-> // get FatTree
			getSubmodule("Controller",0)-> // get controller
			getSubmodule("TrafficController", 0)-> // get traffic controller
			getSubmodule("tcpApp", 0); // get tcp app
}
