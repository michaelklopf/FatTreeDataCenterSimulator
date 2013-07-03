//
// Network controller consists of traffic controller.
// Traffic controller creates jobs according to a distribution.
// The traffic controller is responsible for load balancing and other tasks.
//
// Author: Michael Klopf
// 

package fattree.components;

import ned.IdealChannel;
import ned.DatarateChannel;

import fattree.trafficgeneration.TrafficController;
import fattree.trafficgeneration.TrafficGenerator;

import fattree.logic.ipaddress.VMIPConfig;
import fattree.logic.ipaddress.ContIPConfig;
import fattree.logic.routing.ConfigRouting;
//import fattree.ecmp.ConfigECMPRouting;

//import fattree.router.ConNetRouter;

module Controller
{
    parameters:
        // Parameter handed over from fattree.ned.
        int k;
        int numtgs;
        int numtgsmatrix = numtgs/4;
        @display("bgb=800,800");
    gates:
        // As many gates as core routers.
        inout controllerg[(k/2)^2];
    types:
        channel C extends DatarateChannel {
            datarate = 1000Gbps;
            delay = 0us;            
        }
    submodules:
        trafficController: TrafficController {
            k = k;
            @display("p=400,740");
        }
        
        trafficGenerators[numtgs]: TrafficGenerator {
           @display("p=60,160,m,$numtgsmatrix,160,160"); 
        }
        
        //router: ConNetRouter {
        //   @display("p=150,160"); 
        //}
        
        // Setting up routing behaviour.
        routingsetup: ConfigRouting {
    		@display("p=220,40");
        }
        
        // Assigning ip addresses to the virtual machines.
        vmipconfig: VMIPConfig {
            @display("p=60,40");
        }
        
        // Assigning ip addresses to elements of controller.
        conipconfig: ContIPConfig {
            @display("p=60, 80");
        }
    connections:
        for i=0..sizeof(trafficGenerators)-1 {
            trafficGenerators[i].pppg++ <--> C <--> trafficController.pppg++;
        }
        
        //router.pppg++ <--> C <--> trafficGenerator.pppg++;
        
        //trafficController.pppg++ <--> C <--> router.pppg++;
        
        for i=0..sizeof(controllerg)-1 {
            controllerg++ <--> IdealChannel <--> trafficController.pppg++;
        }
}
