//
// A server consits of a hypervisor and several virtual machines.
// This is represented as a router with several standardhosts.
//
// Author: Michael Klopf
// 

package fattree.components;

//import inet.nodes.inet.StandardHost;
//import inet.nodes.inet.Router;

import ned.IdealChannel;
import ned.DatarateChannel;

import fattree.logic.ipaddress.ServerIPConfig;

module Server
{
    parameters:
        @Server;
        // Number of Virtual Machines on one server.
        // Default value set in FatTree file.
        int vmnum;
        int position;
        @display("bgb=400,250");

    gates:
        // Outgoing port for connection with edge switch.
        inout serverport;

    types:
        // Necessary, because can't interconnect with IdealChannel.
        channel C extends DatarateChannel
        {
            datarate = 1000Gbps;
            delay = 0us;
        }

    submodules:
        // The server consists of a number of virtual machines.
        vms[vmnum]: VirtualMachine {
            @display("p=80,200,r,80");
            @display("i=block/app2");
        }

        // A hypervisor, manages virtual machines migration.
        hypervisor: Hypervisor {	//before that a StandardHost
            @display("p=200,80");//@display("p=80,160");
            @display("i=block/join");
        }

        // The server is build of the virtual machines and the hypervisor,
        // represented with a router connecting those components.
        //serverswitch: Router {
        //    @display("p=300,80");
        //}

        ipconfig: ServerIPConfig {

            @display("p=568,31");
        }
    connections:
        // Connection of virtual machines with the hypervisor.
        for i=0..sizeof(vms)-1 {
            vms[i].pppg++ <--> C <--> hypervisor.pppg++;
        }
        // Connection of hypervisor with switch.
        //hypervisor.pppg++ <--> C <--> serverswitch.pppg++;

        // Connection of hypervisor with compound module gates.
        hypervisor.pppg++ <--> IdealChannel <--> serverport;
}
