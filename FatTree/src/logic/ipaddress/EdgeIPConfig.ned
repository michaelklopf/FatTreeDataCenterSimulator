//
// Author: Michael Klopf
// 

package fattree.logic.ipaddress;

//
// Module assigns ip addresses to edge routers of the fat tree network.
//
simple EdgeIPConfig
{
    string networkAddress = default("10.0.0.0"); // network part of the address (see netmask parameter)
    string netmask = default("255.0.0.0"); // host part of addresses are autoconfigured
    @display("i=block/cogwheel_s");
    @labels(node);
}
