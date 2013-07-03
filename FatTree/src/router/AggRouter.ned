//
// Author: Michael Klopf
// 

package fattree.router;

//import inet.nodes.inet.Router;
import fattree.ecmp.ECMPRouter;
import fattree.ecmp.ECMPStandardHost;

import fattree.logic.ipaddress.AggIPConfig;

//
// Representation of an aggregation router in the fat tree network.
//
module AggRouter extends ECMPStandardHost
{
    parameters:
        @AggRouter;
        @display("i=abstract/router");
        int k;
        int position;	// saves position of router in pod.
        IPForward = default(true);
    submodules:
        ipconfig: AggIPConfig;
}
