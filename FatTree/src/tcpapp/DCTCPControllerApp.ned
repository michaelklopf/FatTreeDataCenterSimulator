//
// Author: Michael Klopf
// 

package fattree.tcpapp;

import inet.applications.tcpapp.TCPApp;
//
// TCP app handling messages arriving at the controller.
//
simple DCTCPControllerApp like TCPApp
{
    parameters:
        @class(DCTCPControllerApp);
        string address = default(""); // may be left empty ("")
        int port = default(1000); // port number to listen on
        string serverThreadClass; // class name of "thread" objects to launch on incoming connections
        @display("i=block/app");
        int HTTPPORT = default(80);
        int FTPPORT = default(21);
        int UPPERHTTPTH = default(30);
        int LOWERHTTPTH = default(10);
        int UPPERFTPTH = default(30);
        int LOWERFTPTH = default(10);
        int VMMIGTH = default(10);  //below it will do mig. if possible
        
        double alpha = default(0.5);
        double gamma = default(0.5);
        
        bool updateNumVMs = default(false);
        bool migrateVMs = default(false);
        
        bool loadAdaptation = default(false);
        
        double loadAdaptionInterval @unit("s") = default(7200s);
        
        double warmUpInterval @unit("s") = default(0s);
        
        // Signal for state of servers
        @signal[globalServerState1](type="long");
        @statistic[serverState1](title="Active Servers";source="globalServerState1";record=vector,stats,timeavg);
        
        @signal[globalServerState2](type="long");
        @statistic[serverState2](title="Active Servers";source="globalServerState2";record=vector,stats,timeavg);
        
        @signal[globalServerState3](type="long");
        @statistic[serverState3](title="Active Servers";source="globalServerState3";record=vector,stats,timeavg);
        
        @signal[globalServerState4](type="long");
        @statistic[serverState4](title="Active Servers";source="globalServerState4";record=vector,stats,timeavg);
        
        @signal[globalServerState5](type="long");
        @statistic[serverState5](title="Active Servers";source="globalServerState5";record=vector,stats,timeavg);
        
        @signal[globalServerState6](type="long");
        @statistic[serverState6](title="Active Servers";source="globalServerState6";record=vector,stats,timeavg);
        
        @signal[globalServerState7](type="long");
        @statistic[serverState7](title="Active Servers";source="globalServerState7";record=vector,stats,timeavg);
        
        @signal[globalServerState8](type="long");
        @statistic[serverState8](title="Active Servers";source="globalServerState8";record=vector,stats,timeavg);
        
        @signal[globalServerState9](type="long");
        @statistic[serverState9](title="Active Servers";source="globalServerState9";record=vector,stats,timeavg);
        
        @signal[globalServerState10](type="long");
        @statistic[serverState10](title="Active Servers";source="globalServerState10";record=vector,stats,timeavg);
        
        @signal[globalServerState11](type="long");
        @statistic[serverState11](title="Active Servers";source="globalServerState11";record=vector,stats,timeavg);
        
        @signal[globalServerState12](type="long");
        @statistic[serverState12](title="Active Servers";source="globalServerState12";record=vector,stats,timeavg);
        
        
        // Singals for number of free VMs.
        @signal[numFreeVMs](type="long");
        @statistic[numFreeVMs](title="Number of Free VMs - waiting for Job";source="numFreeVMs";record=vector);
        
        @signal[numFreeWebVMs](type="long");
        @statistic[numFreeWebVMs](title="Free VMs for webservice"; source="numFreeWebVMs";record=vector,timeavg,stats);
        
        @signal[numFreeFTPVMs](type="long");
        @statistic[numFreeFTPVMs](title="Free VMs for FTP service"; source="numFreeFTPVMs";record=vector,timeavg,stats);
        
        
        // Signals for number of busy VMs.
        @signal[numBusyVMs1](type="long");
        @statistic[numBusyVMs1](title="Number of busy vms - processing a job";source="numBusyVMs1";record=vector,stats,timeavg);
        
        @signal[numBusyVMs2](type="long");
        @statistic[numBusyVMs2](title="Number of busy vms - processing a job";source="numBusyVMs2";record=vector,stats,timeavg);
        
        @signal[numBusyVMs3](type="long");
        @statistic[numBusyVMs3](title="Number of busy vms - processing a job";source="numBusyVMs3";record=vector,stats,timeavg);
        
        @signal[numBusyVMs4](type="long");
        @statistic[numBusyVMs4](title="Number of busy vms - processing a job";source="numBusyVMs4";record=vector,stats,timeavg);
        
        @signal[numBusyVMs5](type="long");
        @statistic[numBusyVMs5](title="Number of busy vms - processing a job";source="numBusyVMs5";record=vector,stats,timeavg);
        
        @signal[numBusyVMs6](type="long");
        @statistic[numBusyVMs6](title="Number of busy vms - processing a job";source="numBusyVMs6";record=vector,stats,timeavg);
        
        @signal[numBusyVMs7](type="long");
        @statistic[numBusyVMs7](title="Number of busy vms - processing a job";source="numBusyVMs7";record=vector,stats,timeavg);
        
        @signal[numBusyVMs8](type="long");
        @statistic[numBusyVMs8](title="Number of busy vms - processing a job";source="numBusyVMs8";record=vector,stats,timeavg);
        
        @signal[numBusyVMs9](type="long");
        @statistic[numBusyVMs9](title="Number of busy vms - processing a job";source="numBusyVMs9";record=vector,stats,timeavg);
        
        @signal[numBusyVMs10](type="long");
        @statistic[numBusyVMs10](title="Number of busy vms - processing a job";source="numBusyVMs10";record=vector,stats,timeavg);
        
        @signal[numBusyVMs11](type="long");
        @statistic[numBusyVMs11](title="Number of busy vms - processing a job";source="numBusyVMs11";record=vector,stats,timeavg);
        
        @signal[numBusyVMs12](type="long");
        @statistic[numBusyVMs12](title="Number of busy vms - processing a job";source="numBusyVMs12";record=vector,stats,timeavg);
        
        
        // Signals for job queue lengths.
        @signal[jobQueueLength](type="long");
        @statistic[jobQueueLength](title="Length of Job Queue";source="jobQueueLength";record=vector);
        
        @signal[jobQueueWeb1](type="long");
        @statistic[jobQueueWeb1](title="Length of JQ for webservice"; source="jobQueueWeb1";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP1](type="long");
        @statistic[jobQueueFTP1](title="Length of JQ for ftp service"; source="jobQueueFTP1";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb2](type="long");
        @statistic[jobQueueWeb2](title="Length of JQ for webservice"; source="jobQueueWeb2";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP2](type="long");
        @statistic[jobQueueFTP2](title="Length of JQ for ftp service"; source="jobQueueFTP2";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb3](type="long");
        @statistic[jobQueueWeb3](title="Length of JQ for webservice"; source="jobQueueWeb3";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP3](type="long");
        @statistic[jobQueueFTP3](title="Length of JQ for ftp service"; source="jobQueueFTP3";record=vector,timeavg,stats);
        
    
        @signal[jobQueueWeb4](type="long");
        @statistic[jobQueueWeb4](title="Length of JQ for webservice"; source="jobQueueWeb4";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP4](type="long");
        @statistic[jobQueueFTP4](title="Length of JQ for ftp service"; source="jobQueueFTP4";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb5](type="long");
        @statistic[jobQueueWeb5](title="Length of JQ for webservice"; source="jobQueueWeb5";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP5](type="long");
        @statistic[jobQueueFTP5](title="Length of JQ for ftp service"; source="jobQueueFTP5";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb6](type="long");
        @statistic[jobQueueWeb6](title="Length of JQ for webservice"; source="jobQueueWeb6";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP6](type="long");
        @statistic[jobQueueFTP6](title="Length of JQ for ftp service"; source="jobQueueFTP6";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb7](type="long");
        @statistic[jobQueueWeb7](title="Length of JQ for webservice"; source="jobQueueWeb7";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP7](type="long");
        @statistic[jobQueueFTP7](title="Length of JQ for ftp service"; source="jobQueueFTP7";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb8](type="long");
        @statistic[jobQueueWeb8](title="Length of JQ for webservice"; source="jobQueueWeb8";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP8](type="long");
        @statistic[jobQueueFTP8](title="Length of JQ for ftp service"; source="jobQueueFTP8";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb9](type="long");
        @statistic[jobQueueWeb9](title="Length of JQ for webservice"; source="jobQueueWeb9";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP9](type="long");
        @statistic[jobQueueFTP9](title="Length of JQ for ftp service"; source="jobQueueFTP9";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb10](type="long");
        @statistic[jobQueueWeb10](title="Length of JQ for webservice"; source="jobQueueWeb10";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP10](type="long");
        @statistic[jobQueueFTP10](title="Length of JQ for ftp service"; source="jobQueueFTP10";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb11](type="long");
        @statistic[jobQueueWeb11](title="Length of JQ for webservice"; source="jobQueueWeb11";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP11](type="long");
        @statistic[jobQueueFTP11](title="Length of JQ for ftp service"; source="jobQueueFTP11";record=vector,timeavg,stats);
        
        
        @signal[jobQueueWeb12](type="long");
        @statistic[jobQueueWeb12](title="Length of JQ for webservice"; source="jobQueueWeb12";record=vector,timeavg,stats);
        
        @signal[jobQueueFTP12](type="long");
        @statistic[jobQueueFTP12](title="Length of JQ for ftp service"; source="jobQueueFTP12";record=vector,timeavg,stats);
        
        
        // Signals for number of active VMs.
        @signal[numActiveVMs1](type="long");
        @statistic[numActiveVMs1](title="Number of Active VMs";source="numActiveVMs1";record=vector,stats,timeavg);
        
        @signal[numActiveVMs2](type="long");
        @statistic[numActiveVMs2](title="Number of Active VMs";source="numActiveVMs2";record=vector,stats,timeavg);
        
        @signal[numActiveVMs3](type="long");
        @statistic[numActiveVMs3](title="Number of Active VMs";source="numActiveVMs3";record=vector,stats,timeavg);
        
        @signal[numActiveVMs4](type="long");
        @statistic[numActiveVMs4](title="Number of Active VMs";source="numActiveVMs4";record=vector,stats,timeavg);
        
        @signal[numActiveVMs5](type="long");
        @statistic[numActiveVMs5](title="Number of Active VMs";source="numActiveVMs5";record=vector,stats,timeavg);
        
        @signal[numActiveVMs6](type="long");
        @statistic[numActiveVMs6](title="Number of Active VMs";source="numActiveVMs6";record=vector,stats,timeavg);
        
        @signal[numActiveVMs7](type="long");
        @statistic[numActiveVMs7](title="Number of Active VMs";source="numActiveVMs7";record=vector,stats,timeavg);
        
        @signal[numActiveVMs8](type="long");
        @statistic[numActiveVMs8](title="Number of Active VMs";source="numActiveVMs8";record=vector,stats,timeavg);
        
        @signal[numActiveVMs9](type="long");
        @statistic[numActiveVMs9](title="Number of Active VMs";source="numActiveVMs9";record=vector,stats,timeavg);
        
        @signal[numActiveVMs10](type="long");
        @statistic[numActiveVMs10](title="Number of Active VMs";source="numActiveVMs10";record=vector,stats,timeavg);
        
        @signal[numActiveVMs11](type="long");
        @statistic[numActiveVMs11](title="Number of Active VMs";source="numActiveVMs11";record=vector,stats,timeavg);
        
        @signal[numActiveVMs12](type="long");
        @statistic[numActiveVMs12](title="Number of Active VMs";source="numActiveVMs12";record=vector,stats,timeavg);
        
        
        @signal[numActiveWebVMs](type="long");
        @statistic[numActiveWebVMs](title="Active VMs for webservice"; source="numActiveWebVMs";record=vector,timeavg,stats);
        
        @signal[numActiveFTPVMs](type="long");
        @statistic[numActiveFTPVMs](title="Active VMs for FTP service"; source="numActiveFTPVMs";record=vector,timeavg,stats);
        
        
        // Signal for number of all VMs.
        @signal[numAllVMs](type="long");
        @statistic[numAllVMs](title="Number of All VMs";source="numAllVMs";record=vector,last);
        
        
        // Singals for recording the moving averages
        // Exp Weigh Mov Avg        
        @signal[movavgWeb](type="long");
        @statistic[movavgWeb](title="ExpWeighMovAvg for webservice"; source="movavgWeb";record=vector);
        
        @signal[movavgFTP](type="long");
        @statistic[movavgFTP](title="ExpWeighMovAvg for ftp service"; source="movavgFTP";record=vector);
        
        
        // Time Exp Weigh Mov Avg        
        @signal[timemovavgWeb](type="long");
        @statistic[timemovavgWeb](title="TimeExpWeighMovAvg for webservice"; source="timemovavgWeb";record=vector,stats);
        
        @signal[timemovavgFTP](type="long");
        @statistic[timemovavgFTP](title="TimeExpWeighMovAvg for ftp service"; source="timemovavgFTP";record=vector,stats);
        
        
        // Signal for total power consumption of dc.
        @signal[totalPowerConsumption1](type="long");
        @statistic[totalPowerConsumption1](title="Total Power Consumption";source="totalPowerConsumption1";record=vector,timeavg,stats);
        
        @signal[totalPowerConsumption2](type="long");
        @statistic[totalPowerConsumption2](title="Total Power Consumption";source="totalPowerConsumption2";record=vector,timeavg,stats);
         
        @signal[totalPowerConsumption3](type="long");
        @statistic[totalPowerConsumption3](title="Total Power Consumption";source="totalPowerConsumption3";record=vector,timeavg,stats);
 
        @signal[totalPowerConsumption4](type="long");
        @statistic[totalPowerConsumption4](title="Total Power Consumption";source="totalPowerConsumption4";record=vector,timeavg,stats);
  
        @signal[totalPowerConsumption5](type="long");
        @statistic[totalPowerConsumption5](title="Total Power Consumption";source="totalPowerConsumption5";record=vector,timeavg,stats);
     
        @signal[totalPowerConsumption6](type="long");
        @statistic[totalPowerConsumption6](title="Total Power Consumption";source="totalPowerConsumption6";record=vector,timeavg,stats);
      
        @signal[totalPowerConsumption7](type="long");
        @statistic[totalPowerConsumption7](title="Total Power Consumption";source="totalPowerConsumption7";record=vector,timeavg,stats);
      
        @signal[totalPowerConsumption8](type="long");
        @statistic[totalPowerConsumption8](title="Total Power Consumption";source="totalPowerConsumption8";record=vector,timeavg,stats);
      
        @signal[totalPowerConsumption9](type="long");
        @statistic[totalPowerConsumption9](title="Total Power Consumption";source="totalPowerConsumption9";record=vector,timeavg,stats);
      
        @signal[totalPowerConsumption10](type="long");
        @statistic[totalPowerConsumption10](title="Total Power Consumption";source="totalPowerConsumption10";record=vector,timeavg,stats);
     
        @signal[totalPowerConsumption11](type="long");
        @statistic[totalPowerConsumption11](title="Total Power Consumption";source="totalPowerConsumption11";record=vector,timeavg,stats);
     
        @signal[totalPowerConsumption12](type="long");
        @statistic[totalPowerConsumption12](title="Total Power Consumption";source="totalPowerConsumption12";record=vector,timeavg,stats);
        
        
        // Signal for done vmmigrations.
        @signal[doneVmMigrations](type="long");
        @statistic[doneVmMigrations](title="Number of done VM Migrations";source="sum(doneVmMigrations)";record=vector,last,count);
        
        @signal[startFinishVMMig](type="long");
        @statistic[startFinishVMMig](title="Start and end of a VM Migration";source="startFinishVMMig";record=vector,stats);
        
        
        // Signal for clients.
        @signal[numberOfActiveWebClients](type="long");
        //@statistic[numberOfActiveWebClients](title="Number of active web clients"; source="numberOfActiveWebClients";record=vector,stats);
        
        @signal[numberOfActiveFtpClients](type="long");
        //@statistic[numberOfActiveFtpClients](title="Number of active ftp clients"; source="numberOfActiveFtpClients";record=vector,stats);
        
        
        // Signal for process job requests.
        @signal[processedWebJob1](type="long");
        @statistic[processedWebJobs1](title="Processed web job requests"; source="processedWebJob1";record=vector,count);
        
        @signal[processedFtpJob1](type="long");
        @statistic[processedFtpJobs1](title="Processed ftp job requests"; source="processedFtpJob1";record=vector,count);
        
        @signal[processedWebJob2](type="long");
        @statistic[processedWebJobs2](title="Processed web job requests"; source="processedWebJob2";record=vector,count);
        
        @signal[processedFtpJob2](type="long");
        @statistic[processedFtpJobs2](title="Processed ftp job requests"; source="processedFtpJob2";record=vector,count);
        
        @signal[processedWebJob3](type="long");
        @statistic[processedWebJobs3](title="Processed web job requests"; source="processedWebJob3";record=vector,count);
        
        @signal[processedFtpJob3](type="long");
        @statistic[processedFtpJobs3](title="Processed ftp job requests"; source="processedFtpJob3";record=vector,count);
        
        @signal[processedWebJob4](type="long");
        @statistic[processedWebJobs4](title="Processed web job requests"; source="processedWebJob4";record=vector,count);
        
        @signal[processedFtpJob4](type="long");
        @statistic[processedFtpJobs4](title="Processed ftp job requests"; source="processedFtpJob4";record=vector,count);
        
        @signal[processedWebJob5](type="long");
        @statistic[processedWebJobs5](title="Processed web job requests"; source="processedWebJob5";record=vector,count);
        
        @signal[processedFtpJob5](type="long");
        @statistic[processedFtpJobs5](title="Processed ftp job requests"; source="processedFtpJob5";record=vector,count);
        
        @signal[processedWebJob6](type="long");
        @statistic[processedWebJobs6](title="Processed web job requests"; source="processedWebJob6";record=vector,count);
        
        @signal[processedFtpJob6](type="long");
        @statistic[processedFtpJobs6](title="Processed ftp job requests"; source="processedFtpJob6";record=vector,count);
        
        @signal[processedWebJob7](type="long");
        @statistic[processedWebJobs7](title="Processed web job requests"; source="processedWebJob7";record=vector,count);
        
        @signal[processedFtpJob7](type="long");
        @statistic[processedFtpJobs7](title="Processed ftp job requests"; source="processedFtpJob7";record=vector,count);
        
        @signal[processedWebJob8](type="long");
        @statistic[processedWebJobs8](title="Processed web job requests"; source="processedWebJob8";record=vector,count);
        
        @signal[processedFtpJob8](type="long");
        @statistic[processedFtpJobs8](title="Processed ftp job requests"; source="processedFtpJob8";record=vector,count);
        
        @signal[processedWebJob9](type="long");
        @statistic[processedWebJobs9](title="Processed web job requests"; source="processedWebJob9";record=vector,count);
        
        @signal[processedFtpJob9](type="long");
        @statistic[processedFtpJobs9](title="Processed ftp job requests"; source="processedFtpJob9";record=vector,count);
        
        @signal[processedWebJob10](type="long");
        @statistic[processedWebJobs10](title="Processed web job requests"; source="processedWebJob10";record=vector,count);
        
        @signal[processedFtpJob10](type="long");
        @statistic[processedFtpJobs10](title="Processed ftp job requests"; source="processedFtpJob10";record=vector,count);
        
        @signal[processedWebJob11](type="long");
        @statistic[processedWebJobs11](title="Processed web job requests"; source="processedWebJob11";record=vector,count);
        
        @signal[processedFtpJob11](type="long");
        @statistic[processedFtpJobs11](title="Processed ftp job requests"; source="processedFtpJob11";record=vector,count);
        
        @signal[processedWebJob12](type="long");
        @statistic[processedWebJobs12](title="Processed web job requests"; source="processedWebJob12";record=vector,count);
        
        @signal[processedFtpJob12](type="long");
        @statistic[processedFtpJobs12](title="Processed ftp job requests"; source="processedFtpJob12";record=vector,count);
        
        // Count events, which lie in the area between two thresholds
        @signal[countDeadZoneHits](type="long");
        @statistic[countDeadZoneHits](title="Events which should not happen"; source = "countDeadZoneHits";record=vector,count);
        
    gates:
        input tcpIn @labels(TCPCommand/up);
        output tcpOut @labels(TCPCommand/down);
}
