//
// Author: Michael Klopf
// 

#include "ServerIPConfig.h"
#include <InterfaceTable.h>
#include <IPAddressResolver.h>
#include <IPv4InterfaceData.h>

Define_Module(ServerIPConfig);

void ServerIPConfig::initialize(int stage)
{
    if (stage == 1) {
        position = getParentModule()->par("position");
        connectedToRouter = getParentModule()->getParentModule()->par("position");
        podposition = getParentModule()->getParentModule()->getParentModule()->par("position");
        //EV << "Edge Router " << position << " lies in POD: " << podposition << ".\n";

        IPAddress address = createAddress(position, podposition, connectedToRouter);
        //EV << "Server " << position << " in POD " << podposition << " at Router " << connectedToRouter << " has IP " << address.str() << "\n";

        setRouterIPAddress(address);

        setDisplayString(address);
    }
}

IPAddress ServerIPConfig::createAddress(int position, int podposition, int connectedToRouter) {
    // First byte determined by ned file.
    int i0 = IPAddress((const char*)par("networkAddress")).getDByte(0);
    // Number of the POD the router lies in.
    int i1 = podposition;
    // Router the server is connected to.
    int i2 = connectedToRouter;
    // Number of the server.
    int i3 = position+2;

    return IPAddress(i0, i1, i2, i3);
}

void ServerIPConfig::setRouterIPAddress(IPAddress address) {
    // find interface table and assign address to all (non-loopback) interfaces
    IInterfaceTable *ift = IPAddressResolver().interfaceTableOf(getParentModule()->getSubmodule("hypervisor"));
    for (int i=0; i<ift->getNumInterfaces(); i++)
    {
        InterfaceEntry *ie = ift->getInterface(i);
        if (!ie->isLoopback())
        {
            ie->ipv4Data()->setIPAddress(IPAddress(address));
            ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS); // full address must match for local delivery
        }
    }
}

void ServerIPConfig::setDisplayString(IPAddress address) {
    if (ev.isGUI()) {
        // update display string
        getParentModule()->getSubmodule("hypervisor")->getDisplayString().setTagArg("t",0,address.str().c_str());
    }
}

void ServerIPConfig::handleMessage(cMessage *msg)
{
    error("this module doesn't handle messages, it runs only in initialize()");
}
