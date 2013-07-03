//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __FATTREE_TGTESTAPP_H_
#define __FATTREE_TGTESTAPP_H_

#include <omnetpp.h>
#include "DCTCPControllerApp.h"
#include "StopClient_m.h"
#include "SocketMapExt.h"
#include "TTimer_m.h"
/**
 * TODO - Generated class
 */
class TGTestApp : public DCTCPControllerApp
{
protected:
    bool clientOn;

    simsignal_t processingTime;
    simsignal_t interArrivalTime;

    /*
     * Socket to controller.
     */
    TCPSocket *conSock;

    bool firstSend;

    double iatMax;
    double iatMin;
    double currentLoadLevel;

    // time interval when current load level is changing
    int loadAdaptionInterval;
    // current step through load array
    int step;
    // the current inter-arrival time
    double currentIAT;

    // deque with load levels
    std::deque<double> *loadLevels;

protected:
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

public:
    TGTestApp() {
        firstSend = true;
    }

    virtual ~TGTestApp() {}

    void emitProcessingTime(simtime_t processingTime);

    void emitIAT(double iatValue);

    void computeIAT();

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
    */

    simsignal_t getProcessingTime() const
    {
        return processingTime;
    }

    void setProcessingTime(simsignal_t processingTime)
    {
        this->processingTime = processingTime;
    }
};

#endif
