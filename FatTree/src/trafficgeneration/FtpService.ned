//
// Module to bundle a certain amount of traffic generators for the purpose of
// emulating a numer of ftp clients.
//
// Author: Michael Klopf
// 

package fattree.trafficgeneration;

import ned.DatarateChannel;
import ned.IdealChannel;

module FtpService
{
    parameters:
        int numtg;
        int matrix = numtg/4;
    gates:
        inout servicegate[numtg];
    types:
        channel C extends DatarateChannel {
            datarate = 1000Gbps;
            delay = 0us;            
        }
    submodules:
        trafficGenerators[numtg]: TrafficGenerator {
            @display("p=160,160,m,$matrix,250,160");
            @display("i=block/ftp");
            @FTP;
        }
    connections:
        for i=0..sizeof(trafficGenerators)-1 {
            trafficGenerators[i].pppg++ <--> IdealChannel <--> servicegate++;
        }
}
