//
// Author: Michael Klopf
// 

package fattree.tcpapp;

//
// Adds virtual machine migration capabilities to hyper visors.
//
simple HyperVisorApp extends DCTCPControllerApp
{
    parameters:
        @class(HyperVisorApp);
        bool hvON = default(true);
        volatile double hvStartUpTime @unit("s") = default(1s);
        volatile int copyDataSize @unit("B") = default(1GiB);
        volatile double hvPowerDownTime @unit("s") = default(1s);
        int idlePower @unit("W") = default(60W); 
        int startUpPower @unit("W") = default(100W);
        int shutDownPower @unit("W") = default(80W);
        
        @signal[hvState](type="long");
        @statistic[hvActivity](title="HV activity";source="hvState";record=vector, timeavg);
        
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
