//
// A rack is the basic building block of a FatTree.
// One edge router is allocated an certain amount of servers.
//
// Author: Michael Klopf
// 

package fattree.components;

//import inet.nodes.inet.StandardHost;
//import inet.nodes.inet.Router;
import fattree.ecmp.ECMPRouter;
import fattree.ecmp.ECMPStandardHost;

import ned.DatarateChannel;
import ned.IdealChannel;

import fattree.logic.ipaddress.EdgeIPConfig;

module Rack
{
    parameters:
        @Rack;
        // We get value from pod module.
        int k;
        int vmnum;
        int position;
        @display("bgb=600,250");

    gates:
        // As many ports as aggregation routers.
        inout rackg[k/2];

    types:
        channel ethernetline extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 1Gbps;
        }

    submodules:
        // k/2 servers are connected with one edge router.
        servers[k/2]: Server {
            // Reach value down.
            vmnum = vmnum;
            position = index;
            @display("p=80,160,r,80");
            @display("i=device/device");
        }
        // One edge or top of rack switch.
        edgeSwitch: ECMPStandardHost { //no from type EdgeRouter.ned, because position is saved in Rack
            @display("p=300,80");
            @display("i=abstract/router");
            IPForward = default(true);
        }

        ipconfig: EdgeIPConfig {
            @display("p=560,31");
        }
    connections allowunconnected:
        // Connect all servers with the one edge switch.
        for i=0..sizeof(servers)-1 {
            edgeSwitch.pppg++ <--> ethernetline <--> servers[i].serverport;
        }
        // Connect the ports of the edge switch with the gates of the compound module.
        for i=0..k/2-1 {
            edgeSwitch.pppg++ <--> IdealChannel <--> rackg++;
        }
}
