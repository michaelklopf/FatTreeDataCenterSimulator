//
// Thread replacing the Traffic Generation App to include functionality for
// communicate with the scheduling module which starts the application, also
// with the TrafficController.
//
// Author: Michael Klopf
// 

#ifndef TRAFFICGENERATIONTHREAD_H_
#define TRAFFICGENERATIONTHREAD_H_

#include "TCPSrvHostApp.h"
#include "TGAppWithIAT.h"

/*
 * Module to emulate different client applications like Web, FTP etc.
 * It allows to be activated by a scheduling module to allow emulating
 * traffic patterns like night/day usage.
 */
class TrafficGenerationThread : public DCTCPControllerThreadBase {
protected:
    cMessage *scheduleMsg;
    TGAppWithIAT *hostmodule;
    bool startstop;
public:
	TrafficGenerationThread() {
		scheduleMsg = NULL;
	};
	virtual ~TrafficGenerationThread() {getHostModule()->cancelAndDelete(scheduleMsg);};

    virtual void established();
    virtual void dataArrived(cMessage *msg, bool urgent);
    virtual void timerExpired(cMessage *timer);

    virtual void closed();
    virtual void failure();
    virtual void peerClosed();
};

#endif /* TRAFFICGENERATIONTHREAD_H_ */
