//
// Author: Michael Klopf
//

#ifndef __FATTREE_TGAPPWITHIAT_H_
#define __FATTREE_TGAPPWITHIAT_H_

#include <omnetpp.h>
#include "DCTCPControllerApp.h"
#include "StopClient_m.h"
#include "SocketMapExt.h"
#include "TTimer_m.h"

/**
 * Class extends the generic controllerapp to give a foundation
 * for the TrafficControllerThread.
 */
class TGAppWithIAT : public DCTCPControllerApp
{
protected:
    bool clientOn;

    simsignal_t decoy;

    simsignal_t processingTime1;
    simsignal_t interArrivalTime1;

    simsignal_t processingTime2;
    simsignal_t interArrivalTime2;

    simsignal_t processingTime3;
    simsignal_t interArrivalTime3;

    simsignal_t processingTime4;
    simsignal_t interArrivalTime4;

    simsignal_t processingTime5;
    simsignal_t interArrivalTime5;

    simsignal_t processingTime6;
    simsignal_t interArrivalTime6;

    simsignal_t processingTime7;
    simsignal_t interArrivalTime7;

    simsignal_t processingTime8;
    simsignal_t interArrivalTime8;

    simsignal_t processingTime9;
    simsignal_t interArrivalTime9;

    simsignal_t processingTime10;
    simsignal_t interArrivalTime10;

    simsignal_t processingTime11;
    simsignal_t interArrivalTime11;

    simsignal_t processingTime12;
    simsignal_t interArrivalTime12;

    /*
     * Socket to controller.
     */
    TCPSocket *conSock;

    bool firstSend;

    bool loadAdaptation;

    double iatMean;

    // time interval when current load level is changing
    double loadAdaptionInterval;
    double warmUpInterval;

    // current step through load array
    int step;
    // the current inter-arrival time
    double currentIAT;

    // deque with load levels
    std::deque<double> *loadLevels;

    simsignal_t processSigs[12];
    simsignal_t iatSigs[12];

protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    void computeIAT(double currentLoadLevel);

    void initializeSignals();

public:
    TGAppWithIAT() {
        firstSend = true;
    }

    virtual ~TGAppWithIAT() {}

    void emitProcessingTime(simtime_t processingTime);

    void emitIAT(double iatValue);

    bool isClientOn() const
    {
        return clientOn;
    }

    void setClientOn(bool clientOn)
    {
        this->clientOn = clientOn;
    }

    double getCurrentIat() const
    {
        return currentIAT;
    }

    bool isFirstSend() const
    {
        return firstSend;
    }

    void setFirstSend(bool firstSend)
    {
        this->firstSend = firstSend;
    }

    TCPSocket *getConSock() const
    {
        return conSock;
    }

    /*
    void setConSock(TCPSocket *conSock)
    {
        this->conSock = conSock;
    }

    simsignal_t getProcessingTime() const
    {
        return processingTime1;
    }

    void setProcessingTime(simsignal_t processingTime)
    {
        this->processingTime1 = processingTime;
    }
    */
};


#endif
