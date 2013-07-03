//
// Author: Michael Klopf
// 

#include "CoreIPConfig.h"
#include <InterfaceTable.h>
#include <IPAddressResolver.h>
#include <IPv4InterfaceData.h>

Define_Module(CoreIPConfig);

void CoreIPConfig::initialize(int stage)
{
    if (stage == 1) {
        k = getParentModule()->par("k");
        position = getParentModule()->par("position");
        //EV << "Core Router " << position << " knows his k value: " << k << ".\n";

        IPAddress address = createAddress(k, position);
        //EV << "Core Router " << position << " with IP " << address.str() << "\n";

        setRouterIPAddress(address);

        setDisplayString(address);
    }
}

IPAddress CoreIPConfig::createAddress(int k, int position) {
    // First byte determined by ned file.
    int i0 = IPAddress((const char*)par("networkAddress")).getDByte(0);
    // Row value.
    int i2 = (int)(position/(k/2))+1;
    // Column value.
    int i3 = (int)(position%(k/2))+1;

    return IPAddress(i0, k, i2, i3);
}

void CoreIPConfig::setRouterIPAddress(IPAddress address) {
    // find interface table and assign address to all (non-loopback) interfaces
    IInterfaceTable *ift = IPAddressResolver().interfaceTableOf(getParentModule());
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

void CoreIPConfig::setDisplayString(IPAddress address) {
    if (ev.isGUI()) {
        // update display string
        getParentModule()->getDisplayString().setTagArg("t",0,address.str().c_str());
    }
}

void CoreIPConfig::handleMessage(cMessage *msg)
{
    error("this module doesn't handle messages, it runs only in initialize()");
}
