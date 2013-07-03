//
// Author: Michael Klopf
// 

package fattree.logic.ipaddress;

//
// Module to assign ip addresses to the aggregation routers in a fat tree.
//
simple AggIPConfig
{
    string networkAddress = default("10.0.0.0"); // network part of the address (see netmask parameter)
    string netmask = default("255.0.0.0"); // host part of addresses are autoconfigured
    @display("i=block/cogwheel_s");
    @labels(node);
}
