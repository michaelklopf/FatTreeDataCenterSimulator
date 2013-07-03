//
// This network represents a so called (or k-ary) fat tree architecture.
// The architecture consists of three layers. The core, aggregation and edge layer.
//
// Core routers are connected with the pods.
// Each pod consists of two layers of routers (aggregation and edge).
// The servers connected to an edge router are together called a rack.
//
// Author: Michael Klopf
//

package fattree.simulations;

import inet.nodes.inet.Router;
//import inet.networklayer.autorouting.FatTreeConfigurator;

import ned.DatarateChannel;
import ned.IdealChannel;

import fattree.components.Pod;
//import fattree.components.Controller;
import fattree.components.ECMPController;
import fattree.router.CoreRouter;

import inet.world.NAMTrace;

network FatTree
{
    parameters:
        // Only even values are allowed.
        int k = default(4);

        // Number of virtual machines created for each server.
        int vmnum = default(4);
        
        // Number of traffic generators.
        //int numtgs = default(1);
        // Number of http service clients
        int httpclients = default(6);
        // Number of ftp service clients
        int ftpclients = default(6);
        
        // Value for display string needed.
        int khalf = k/2;

		@display("bgb=500,500");
		//@display("bgb=800,800"); // For k = 8
    types:
        channel ethernetline extends DatarateChannel
        {
            delay = 0.1us;
            datarate = 1Gbps;
        }

    submodules:
        // A k-ary fattree consists of k pods.
        Pod[k]: Pod {
            // Reach values down.
            k = k;
            vmnum = vmnum;
            position = index;
            //@display("p=100,160,c,80");
            @display("p=70,450,r,120");
            @display("i=device/server");
        }

		// (k/2)^2 core routers.
        CoreRouter[(k/2)^2]: CoreRouter {
            k = k;
            position = index;
            //@display("p=280,200,m,$khalf,120,120");
            @display("p=150,150,m,$khalf,200,150");
        }

        // A network controller. Creates messages and gives them
        // out to the network.
        Controller: ECMPController {
            k = k;
            //numtgs = numtgs;
            numHttpClients = httpclients;
            numFtpClients = ftpclients;
            @display("p=250,30");
            @display("i=block/control");
        }

        //configurator: FatTreeConfigurator;
        nam: NAMTrace {
            @display("p=-50,-50");
        }
    connections:
        // Each pod is connected with all core routers.
        // Two aggregation routers are not connected to the same core routers.
        // We assume omnet takes care of this issue when wiring the routers, so
        // that it uses the gates consecutively.
        for i=0..k-1, for j=0..sizeof(CoreRouter)-1 {
            Pod[i].podg++ <--> ethernetline <--> CoreRouter[j].pppg++;
        }

        // Core routers are connected with the Network Controller.
        for i=0..sizeof(CoreRouter)-1 {
            CoreRouter[i].pppg++ <--> ethernetline <--> Controller.controllerg++;
            CoreRouter[i].pppg++ <--> ethernetline <--> Controller.syscong++;
        }
}
