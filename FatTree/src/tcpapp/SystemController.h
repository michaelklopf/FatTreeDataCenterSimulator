//
// TCP Application for ...
//
// Author: Michael Klopf
//

#ifndef __FATTREE_SYSTEMCONTROLLER_H_
#define __FATTREE_SYSTEMCONTROLLER_H_

#include <omnetpp.h>
#include <DCTCPControllerApp.h>
#include "VMMigMsg_m.h"
#include "ServiceMap.h"
#include "ClientTimeMap.h"
#include "StartClient_m.h"

/**
 * Starts/Deactivates servers and
 * virtual machines. Initiates virtual machine migrations.
 * Deactivates/Activates network components.
 */
class SystemController : public DCTCPControllerApp
{
protected:
	int currentStep;
	int numActHTTPClientsOld;
	int numActFTPClientsOld;
	ServiceMap *httpClients;
	ServiceMap *ftpClients;
	ServiceMap *activeHTTPClients;
	ServiceMap *activeFTPClients;

	ClientTimeMap *timePercentagePairs;

	//StartClient *timerMsg;
protected:
    virtual int numInitStages() const  {return 3;}
	virtual void initialize(int stage);
	virtual void finish();
	virtual void handleMessage(cMessage *msg);
	/* deprecated */
	virtual void updateNumberOfActiveClients();
	/* deprecated */
	virtual void fillInTimeAndPercentagePairs();
	/* deprecated */
	virtual void updateHTTPClients(int numClientsToChange);
	/*deprecated */
	virtual void updateFTPClients(int numClientsToChange);
public:
	virtual void startUpVM(IPvXAddress addr, int port);
	virtual void shutDownVM(IPvXAddress addr, int port);
	virtual void startVMMigration(VMMigMsg *vmmig);

	virtual cModule *findTrafficControllerModule();

	/* deprecated */
	virtual void updateActiveClients(IPvXAddress addr, int port);
	/* deprecated */
	virtual void updateDeactivatedClients(IPvXAddress addr, int port);
};

#endif
