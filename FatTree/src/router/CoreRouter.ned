//
// Author: Michael Klopf
// 

package fattree.router;
import fattree.ecmp.ECMPStandardHost;

//import inet.nodes.inet.Router;
import fattree.ecmp.ECMPRouter;

import fattree.logic.ipaddress.CoreIPConfig;
//
// Represents a core router in the fat tree.
//
module CoreRouter extends ECMPStandardHost
{
    parameters:
		@CoreRouter;
        @display("i=abstract/router");
		int k;
        int position; // position among other core routers
        IPForward = default(true);
    submodules:
        ipconfig: CoreIPConfig;
}
