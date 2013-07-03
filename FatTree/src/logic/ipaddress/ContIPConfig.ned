//
// Author: Michael Klopf
// 

package fattree.logic.ipaddress;

//
// Assigns ip addresses to the controller and traffic generator part in the fat tree network.
// Uses a different sub network compared to the rest of the fat tree.
//
simple ContIPConfig
{
    string networkAddress = default("30.0.0.0"); // network part of the address (see netmask parameter)
    string netmask = default("255.0.0.0"); // host part of addresses are autoconfigured
    @display("i=block/cogwheel_s");
    @labels(node);
}
