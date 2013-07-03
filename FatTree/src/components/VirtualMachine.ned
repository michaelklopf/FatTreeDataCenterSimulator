//
// Virtual machine representation. Needed for ip assignment.
//
// Author: Michael Klopf
// 

package fattree.components;

//import inet.nodes.inet.StandardHost;
import fattree.ecmp.ECMPStandardHost;

module VirtualMachine extends ECMPStandardHost
{
    parameters:
    	@VM;
}
