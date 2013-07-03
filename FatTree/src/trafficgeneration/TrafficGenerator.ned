//
// Creates messages according to a distribution.
//
// Author: Michael Klopf
// 

package fattree.trafficgeneration;

//import inet.nodes.inet.StandardHost;
import fattree.ecmp.ECMPStandardHost;

module TrafficGenerator extends ECMPStandardHost
{
    parameters:
        @Controller;
}
