//
// Author: Michael Klopf
// 

package fattree.tcpapp;

import inet.applications.tcpapp.TCPApp;

//
// Aggregation router tcp application.
//
simple AggRouterApp like TCPApp
{
    parameters:
        int powerConSwitch @unit("W");
        double powerConCable @unit("W");
        int k;
        
    gates:
        input tcpIn @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}
