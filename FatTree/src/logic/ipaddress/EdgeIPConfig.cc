//
// Author: Michael Klopf
// 

#include "EdgeIPConfig.h"
#include <InterfaceTable.h>
#include <IPAddressResolver.h>
#include <IPv4InterfaceData.h>

Define_Module(EdgeIPConfig);

void EdgeIPConfig::initialize(int stage)
{
    if (stage == 1) {
        position = getParentModule()->par("position");
        podposition = getParentModule()->getParentModule()->par("position");
        //EV << "Edge Router " << position << " lies in POD: " << podposition << ".\n";

        IPAddress address = createAddress(position, podposition);
        //EV << "Edge Router " << position << " in POD " << podposition << " has IP " << address.str() << "\n";

        setRouterIPAddress(address);

        setDisplayString(address);
    }
}

IPAddress EdgeIPConfig::createAddress(int position, int podposition) {
    // First byte determined by ned file.
    int i0 = IPAddress((const char*)par("networkAddress")).getDByte(0);
    // Number of the POD the router lies in.
    int i1 = podposition;
    // Position of switch in POD.
    int i2 = position;

    return IPAddress(i0, i1, i2, 1);    //i3 = 1, according to paper.
}

void EdgeIPConfig::setRouterIPAddress(IPAddress address) {
    // find interface table and assign address to all (non-loopback) interfaces
    IInterfaceTable *ift = IPAddressResolver().interfaceTableOf(getParentModule()->getSubmodule("edgeSwitch"));
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

void EdgeIPConfig::setDisplayString(IPAddress address) {
    if (ev.isGUI()) {
        // update display string
        getParentModule()->getSubmodule("edgeSwitch")->getDisplayString().setTagArg("t",0,address.str().c_str());
    }
}

void EdgeIPConfig::handleMessage(cMessage *msg)
{
    error("this module doesn't handle messages, it runs only in initialize()");
}
