//
// Author: Michael Klopf
// 

package fattree.tcpapp;

//
// Adds virtual machine migration capabilities to virtual machines.
//
simple VirtualMachineApp extends DCTCPControllerApp
{
    parameters:
        //@VM defined in components/VM
        @class(VirtualMachineApp);
    	string networkAddress = default("20.0.0.0"); // network part of the address (see netmask parameter)
    	string netmask = default("255.0.0.0"); // host part of addresses are autoconfigured
    	int connectToHvPort = default(1000);
        bool vmON = default(true);
        volatile double vmPowerDownTime @unit("s") = default(60s);
        volatile double vmPowerUpTime @unit("s") = default(60s);
        double delayMsg @unit("s") = default(1s);
        
        int idlePower @unit("W") = default(5W);
        int busyPower @unit("W") = default(10W);
        int startUpPower @unit("W") = default(15W);
        int shutDownPower @unit("W") = default(15W);
        
        //int serviceport = default(80);
        
        @signal[vmState](type="long");
        @statistic[vmActivity](title="VM activity";source="vmState";record=vector,count, timeavg);
        
        @signal[busyState](type="bool");
        @statistic[busyState](title="Time in busy state";source="busyState";record=vector, mean, timeavg);
        
        @signal[idleState](type="bool");
        @statistic[idleState](title="Time in idle state";source="idleState";record=vector, mean, timeavg);
        
        @signal[powerUpState](type="bool");
        @statistic[powerUpState](title="Time in state when starting up";source="powerUpState";record=vector, mean, timeavg);
        
        @signal[powerDownState](type="bool");
        @statistic[powerDownState](title="Time in state when shutting down";source="powerDownState";record=vector, mean, timeavg);
        
        @signal[offState](type="bool");
        @statistic[offState](title="Time in off state";source="offState";record=vector, mean, timeavg);
}
