//
// Author: Michael Klopf
// 

package fattree.tcpapp;

import inet.applications.tcpapp.TCPApp;

//
// Core router tcp application.
//
simple CoreRouterApp like TCPApp
{
    parameters:
        int powerConSwitch @unit("W");
        double powerConCable @unit("W");
        int k;
        
    gates:
        input tcpIn @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}
