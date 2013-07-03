//
// Author: Michael Klopf
// 

package fattree.tcpapp;

import inet.applications.tcpapp.TCPApp;

//
// Replacing TCPBasicClientApp and TCPGenericClientApp with this one.
// Put all the code into one file to make it easier to add new functionality.
//
simple TrafficGenerationApp like TCPApp
{
    parameters:
        volatile double replyDelay @unit("s") = default(1s);
        
        @signal[processingTime](type="double");
        @statistic[processingTime](title="Time to process a job.";source="processingTime";record=vector);
        
        string address = default(""); // may be left empty ("")
        int port = default(-1); // port number to listen on
        string connectAddress = default("");  // server address (may be symbolic)
        int connectPort = default(1000); // port number to connect to
        double startTime @unit("s") = default(1s); // time first session begins
        volatile int numRequestsPerSession = default(1);  // number of requests sent per session
        volatile int requestLength @unit(B) = default(200B); // length of a request
        volatile int replyLength @unit("B") = default(1MiB); // length of a reply
        volatile double thinkTime @unit("s"); // time gap between requests
        volatile double idleInterval @unit(s); // time gap between sessions
        volatile double reconnectInterval @unit("s") = default(30s);  // if connection breaks, waits this much before trying to reconnect
        @display("i=block/app");
    gates:
        input tcpIn @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}
