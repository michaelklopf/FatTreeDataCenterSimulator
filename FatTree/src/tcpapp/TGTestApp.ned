//
// 
//
// Author: Michael Klopf
// 

package fattree.tcpapp;

//import inet.applications.tcpapp.TCPApp;

//
// Application which supports the TrafficControllerThread.
//
simple TGTestApp extends DCTCPControllerApp
{
    parameters:
        @class(TGTestApp);
        volatile double replyDelay @unit("s") = default(1s);
        bool clientOn = default(false);
        
        @signal[processingTime](type="double");
        @statistic[processingTime](title="Time to process a job.";source="processingTime";record=vector,timeavg);
        
        @signal[interArrivalTime](type="double");
        @statistic[interArrivalTime](title="IAT of http jobs";source="interArrivalTime";record=vector,stats);
        
        
        //string address = default(""); // may be left empty ("")
        //int port = default(1000); // port number to listen on
        string connectAddress = default("");  // server address (may be symbolic)
        int connectPort = default(1000); // port number to connect to
        
        volatile double startTime @unit("s") = default(1s); // time first session begins
        volatile int numRequestsPerSession = default(1);  // number of requests sent per session
        volatile int requestLength @unit(B) = default(200B); // length of a request
        volatile int replyLength @unit("B") = default(1MiB); // length of a reply
        volatile double thinkTime @unit("s") = default(1s);// time gap between requests
        volatile double idleInterval @unit(s) = default(1s);// time gap between sessions
        volatile double reconnectInterval @unit("s") = default(30s);  // if connection breaks, waits this much before trying to reconnect
        
        volatile double iatMax @unit("s");
        volatile double iatMin @unit("s");
        
        //int loadAdaptionInterval @unit("s") = default(7200s);
        //string serverThreadClass; // class name of "thread" objects to launch on incoming connections
        @display("i=block/app");
}
