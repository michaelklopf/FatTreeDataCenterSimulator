//
// Hypervisor for virtual machine observation. It's forwarding the traffic
// to the vms.
//
// Author: Michael Klopf
// 

package fattree.components;

//import inet.nodes.inet.StandardHost;
import fattree.ecmp.ECMPStandardHost;

module Hypervisor extends ECMPStandardHost
{
    parameters:
        @Hypervisor;
        IPForward = default(true);
}
