//
// Author: Michael Klopf
// 

#include "VMIPConfig.h"
#include <IPAddress.h>
#include <InterfaceTable.h>
#include <IPAddressResolver.h>
#include <IPv4InterfaceData.h>

Define_Module(VMIPConfig);

void VMIPConfig::initialize(int stage)
{
    if (stage == 1) {
        // Set ips for the virtual machines.
        cTopology vmTopology("VMTopology");
        vmTopology.extractByProperty("VM");
        EV << "We found " << vmTopology.getNumNodes() << " virtual machines.\n";

        setVMIPAddress(vmTopology);
    }
}

void VMIPConfig::setVMIPAddress(cTopology& topology) {
    // Get network address/netmask and check if address space is big enough.
    uint32 networkAddress = IPAddress(par("networkAddress").stringValue()).getInt();
    uint32 netmask = IPAddress(par("netmask").stringValue()).getInt();
    int maxNodes = (~netmask)-1;
    if (topology.getNumNodes()>maxNodes)
            error("netmask too large, not enough addresses for all %d nodes", topology.getNumNodes());

    // Count VMs for creation of ip addresses.
    int numVMs = 0;
    // Assign every virtual machine an address and set it.
    for (int i = 0; i < topology.getNumNodes(); i++) {
        // ip is disjunction of network address and current number of vms.
        uint32 addr = networkAddress | uint32(++numVMs);

        // Write Address in interface table.
        IInterfaceTable *ift = IPAddressResolver().interfaceTableOf(topology.getNode(i)->getModule());
        for (int j=0; j<ift->getNumInterfaces(); j++)
        {
            InterfaceEntry *ie = ift->getInterface(j);
            if (!ie->isLoopback()) //check for loopback address
            {
                ie->ipv4Data()->setIPAddress(IPAddress(addr));
                ie->ipv4Data()->setNetmask(IPAddress::ALLONES_ADDRESS); // full address must match for local delivery
            }
        }
        // Set display string of virtual machine.
        if (ev.isGUI()) {
            // update display string
            topology.getNode(i)->getModule()->getDisplayString().setTagArg("t",0,IPAddress(addr).str().c_str());
        }
    }
}

void VMIPConfig::handleMessage(cMessage *msg)
{
    error("this module doesn't handle messages, it runs only in initialize()");
}
