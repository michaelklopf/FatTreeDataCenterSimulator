//
// Network controller. Saves jobs coming from Traffic Generator and distributes them to servers
// and controls load balancing and flows for the messages. Also takes care of activating VMs.
//
// Author: Michael Klopf
// 

package fattree.trafficgeneration;

//import inet.nodes.inet.StandardHost;
import fattree.ecmp.ECMPStandardHost;

module TrafficController extends ECMPStandardHost
{
    parameters:
        @Controller;
        int k;
        IPForward = default(true);
    //gates:
        // As many gates as core routers.
        //inout tcgate[(k/2)^2];
        // One gate to connect with traffic generator.
        //input ingate;
}
