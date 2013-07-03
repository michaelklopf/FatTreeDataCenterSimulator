//
// Author: Michael Klopf
// 

package fattree.router;

import inet.nodes.inet.Router;

//
// Router to connect the elements in the controller subnetwork for
// traffic generation purposes.
//
module ConNetRouter extends Router
{
    parameters:
        @Controller;
}
