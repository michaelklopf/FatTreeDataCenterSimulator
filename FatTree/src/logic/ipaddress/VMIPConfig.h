//
// Author: Michael Klopf
// 

#ifndef __FATTREEVMMIGRATIONCASE1_VMIPCONFIG_H_
#define __FATTREEVMMIGRATIONCASE1_VMIPCONFIG_H_

#include <omnetpp.h>

/**
 * Sets the IP addresses of all the virtual machines in the network.
 * It also sets the IP address of the traffic controller. He's part
 * of the same subnet.
 */
class VMIPConfig : public cSimpleModule
{
    protected:
        virtual int numInitStages() const  {return 2;}
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);

        /**
         * Creates the IP-Address, sets it and creates a
         * display string showing the IP Address.
         */
        virtual void setVMIPAddress(cTopology& topology);
};

#endif
