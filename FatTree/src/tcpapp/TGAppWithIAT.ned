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
simple TGAppWithIAT extends DCTCPControllerApp
{
    parameters:
        @class(TGAppWithIAT);
        volatile double replyDelay @unit("s") = default(1s);
        bool clientOn = default(false);
        //bool loadAdaptation = default(false);
        
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
        
        //volatile double iatMax @unit("s");
        //volatile double iatMin @unit("s");
        volatile double iatMean @unit("s");
        
        //int loadAdaptionInterval @unit("s") = default(7200s);
        //string serverThreadClass; // class name of "thread" objects to launch on incoming connections
        @display("i=block/app");
        
        @signal[processingTime1](type="double");
        @statistic[sojournTime1](title="Time to process a job.";source="processingTime1";record=vector,stats, timeavg);
        
        @signal[interArrivalTime1](type="double");
        @statistic[interArrivalTime1](title="IAT of http jobs";source="interArrivalTime1";record=vector,stats, timeavg);
        
        
        @signal[processingTime2](type="double");
        @statistic[sojournTime2](title="Time to process a job.";source="processingTime2";record=vector,stats, timeavg);
        
        @signal[interArrivalTime2](type="double");
        @statistic[interArrivalTime2](title="IAT of http jobs";source="interArrivalTime2";record=vector,stats, timeavg);
        
        
        @signal[processingTime3](type="double");
        @statistic[sojournTime3](title="Time to process a job.";source="processingTime3";record=vector,stats, timeavg);
        
        @signal[interArrivalTime3](type="double");
        @statistic[interArrivalTime3](title="IAT of http jobs";source="interArrivalTime3";record=vector,stats, timeavg);
        
        
        @signal[processingTime4](type="double");
        @statistic[sojournTime4](title="Time to process a job.";source="processingTime4";record=vector,stats, timeavg);
        
        @signal[interArrivalTime4](type="double");
        @statistic[interArrivalTime4](title="IAT of http jobs";source="interArrivalTime4";record=vector,stats, timeavg);
        
        
        @signal[processingTime5](type="double");
        @statistic[sojournTime5](title="Time to process a job.";source="processingTime5";record=vector,stats, timeavg);
        
        @signal[interArrivalTime5](type="double");
        @statistic[interArrivalTime5](title="IAT of http jobs";source="interArrivalTime5";record=vector,stats, timeavg);
        
        
        @signal[processingTime6](type="double");
        @statistic[sojournTime6](title="Time to process a job.";source="processingTime6";record=vector,stats, timeavg);
        
        @signal[interArrivalTime6](type="double");
        @statistic[interArrivalTime6](title="IAT of http jobs";source="interArrivalTime6";record=vector,stats, timeavg);
        
        
        @signal[processingTime7](type="double");
        @statistic[sojournTime7](title="Time to process a job.";source="processingTime7";record=vector,stats, timeavg);
        
        @signal[interArrivalTime7](type="double");
        @statistic[interArrivalTime7](title="IAT of http jobs";source="interArrivalTime7";record=vector,stats, timeavg);
        
        
        @signal[processingTime8](type="double");
        @statistic[sojournTime8](title="Time to process a job.";source="processingTime8";record=vector,stats, timeavg);
        
        @signal[interArrivalTime8](type="double");
        @statistic[interArrivalTime8](title="IAT of http jobs";source="interArrivalTime8";record=vector,stats, timeavg);
        
        
        @signal[processingTime9](type="double");
        @statistic[sojournTime9](title="Time to process a job.";source="processingTime9";record=vector,stats, timeavg);
        
        @signal[interArrivalTime9](type="double");
        @statistic[interArrivalTime9](title="IAT of http jobs";source="interArrivalTime9";record=vector,stats, timeavg);
        
        
        @signal[processingTime10](type="double");
        @statistic[sojournTime10](title="Time to process a job.";source="processingTime10";record=vector,stats, timeavg);
        
        @signal[interArrivalTime10](type="double");
        @statistic[interArrivalTime10](title="IAT of http jobs";source="interArrivalTime10";record=vector,stats, timeavg);
        
        
        @signal[processingTime11](type="double");
        @statistic[sojournTime11](title="Time to process a job.";source="processingTime11";record=vector,stats, timeavg);
        
        @signal[interArrivalTime11](type="double");
        @statistic[interArrivalTime11](title="IAT of http jobs";source="interArrivalTime11";record=vector,stats, timeavg);
        
        
        @signal[processingTime12](type="double");
        @statistic[sojournTime12](title="Time to process a job.";source="processingTime12";record=vector,stats, timeavg);
        
        @signal[interArrivalTime12](type="double");
        @statistic[interArrivalTime12](title="IAT of http jobs";source="interArrivalTime12";record=vector,stats, timeavg);
}
