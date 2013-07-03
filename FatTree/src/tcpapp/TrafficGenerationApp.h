//
// Replacing TCPBasicClientApp and TCPGenericClientApp with this one.
// Put all the code into one file to make it easier to add new functionality.
//
// Author: Michael Klopf
// 

#ifndef __FATTREE_TRAFFICGENERATIONAPP_H_
#define __FATTREE_TRAFFICGENERATIONAPP_H_

#include <omnetpp.h>
#include "TCPSocket.h"

/**
 * An example request-reply based client application.
 */
class TrafficGenerationApp : public cSimpleModule, public TCPSocket::CallbackInterface
{
protected:
	TCPSocket socket;

	// statistics
	int numSessions;
	int numBroken;
	int packetsSent;
	int packetsRcvd;
	int bytesSent;
	int bytesRcvd;

	simsignal_t processingTime;
	simtime_t departureTime;
	simtime_t arrivalTime;

    cMessage *timeoutMsg;
    bool earlySend;  // if true, don't wait with sendRequest() until established()
    int numRequestsToSend; // requests to send in this session

    /** Utility: sends a request to the server */
    virtual void sendRequest();

public:
    TrafficGenerationApp();
    virtual ~TrafficGenerationApp();
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    virtual void connect();
    virtual void close();

    virtual void sendPacket(int numBytes, int expectedReplyBytes, bool serverClose=false);

    virtual void setStatusString(const char *s);

    virtual void handleTimer(cMessage *msg);

    virtual void socketEstablished(int connId, void *yourPtr);

    virtual void socketDataArrived(int connId, void *yourPtr, cPacket *msg, bool urgent);

    virtual void socketPeerClosed(int connId, void *yourPtr);

    virtual void socketClosed(int connId, void *yourPtr);

    virtual void socketFailure(int connId, void *yourPtr, int code);

    virtual void socketStatusArrived(int connId, void *yourPtr, TCPStatusInfo *status) {delete status;}
};

#endif
