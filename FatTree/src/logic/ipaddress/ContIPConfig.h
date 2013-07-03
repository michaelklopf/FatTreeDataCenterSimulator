//
// Author: Michael Klopf
// 

#ifndef __FATTREEVMMIGRATIONCASE1_CONTIPCONFIG_H_
#define __FATTREEVMMIGRATIONCASE1_CONTIPCONFIG_H_

#include <omnetpp.h>

/**
 * Sets the IP addresses of all (possible) traffic generators
 * and the controller in the network.
 */
class ContIPConfig : public cSimpleModule
{
    protected:
        virtual int numInitStages() const  {return 2;}
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);

        /**
         * Creates IP-Address for the controllers and traffic generators to allow
         * sending packages.
         */
        virtual void setConIPAddress(cTopology& topology);
};

#endif
