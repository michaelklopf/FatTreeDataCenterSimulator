//
// Author: Michael Klopf
// 

#ifndef __FATTREEVMMIGRATIONCASE1_COREIPCONFIG_H_
#define __FATTREEVMMIGRATIONCASE1_COREIPCONFIG_H_

#include <omnetpp.h>
#include <IPAddress.h>

/**
 * Sets the IP address of the core router at a certain position
 * in dependence to the k-value of the fat tree.
 */
class CoreIPConfig : public cSimpleModule
{
    protected:
        /**
         * value, which is necessary for building fat tree.
         */
        int k;
        /**
         * gives the position among the other core routers.
         */
        int position;
    protected:
        virtual int numInitStages() const  {return 2;}
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);

        /**
         * Takes the k-value and the position of the core router in the
         * network and computes the address.
         */
        virtual IPAddress createAddress(int k, int position);
        /**
         * Finds the interface table and assigns the ip address
         * to all non-loopback interfaces.
         */
        virtual void setRouterIPAddress(IPAddress address);
        /**
         * Sets the ip address as display string in graphical environment.
         */
        virtual void setDisplayString(IPAddress address);
};

#endif
