//
// The pod connects the aggregation layer routers with the racks.
//
// Author: Michael Klopf
// 

package fattree.components;

//import inet.nodes.inet.Router;

import ned.DatarateChannel;
import ned.IdealChannel;

import fattree.router.AggRouter;

module Pod
{
    parameters:
        @Pod;
        // We get value from FatTree module.
        int k;
        int vmnum;
        int position;

        @display("bgb=600,250");
    gates:
        // As many gates as core routers.
        inout podg[(k/2)^2];

    types:
        channel ethernetline extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 1Gbps;
        }

    submodules:
        // k/2 racks are in one pod.
        racks[k/2]: Rack {
            // Reach values down.
            k = k;
            vmnum = vmnum;
            position = index;
            @display("p=80,160,r,120");
            @display("i=device/server2");
        }

		// Also k/2 aggregation routers in one pod.
        aggRouters[k/2]: AggRouter {
            k = k;
            position = index;
            @display("p=80,80,r,120");
        }
    connections allowunconnected:
        // Connect a rack with all aggregation routers. Do it for all racks.
        for i=0..sizeof(racks)-1, for j=0..sizeof(aggRouters)-1 {
            racks[i].rackg++ <--> ethernetline <--> aggRouters[j].pppg++;
        }

		// Connect the aggregation routers with the compound ports.
        for i=0..sizeof(aggRouters)-1 {
            aggRouters[i].pppg++ <--> IdealChannel <--> podg++;
        }
}
