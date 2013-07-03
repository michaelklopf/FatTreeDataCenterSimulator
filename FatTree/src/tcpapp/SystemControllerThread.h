//
// Author: Michael Klopf
// 

#ifndef SYSTEMCONTROLLERTHREAD_H_
#define SYSTEMCONTROLLERTHREAD_H_

#include "DCTCPControllerApp.h"

class SystemControllerThread: public DCTCPControllerThreadBase {
public:
	SystemControllerThread() {};
	virtual ~SystemControllerThread() {};

	virtual void established();
    virtual void dataArrived(cMessage *msg, bool urgent);
    virtual void timerExpired(cMessage *timer);

    virtual void closed();
    virtual void peerClosed();
};

#endif /* SYSTEMCONTROLLERTHREAD_H_ */
