//
// Author: Michael Klopf
//



package fattree.ecmp;

import inet.networklayer.arp.ARP;
import inet.networklayer.ipv4.ErrorHandling;
import inet.networklayer.ipv4.ICMP;
import inet.networklayer.ipv4.IGMP;
//import inet.networklayer.ipv4.IP;


//
// Network layer of an \IP node. Now ECMP IP node.
//
// Interfaces to transport layer: TCP, UDP, echo/ping, RSVP
//
module ECMPNetworkLayer
{
    parameters:
        bool proxyARP = default(true);
        @display("i=block/fork");
    gates:
        input ifIn[] @labels(IPDatagram);
        input tcpIn @labels(TCPSegment,IPControlInfo/down);
        input udpIn @labels(UDPPacket,IPControlInfo/down);
        input sctpIn @labels(IPControlInfo/down,SCTPPacket);   //I.R.
        input rsvpIn @labels(IPControlInfo/down);
        input ospfIn @labels(IPControlInfo/down);
        input pingIn;
        output ifOut[];
        output tcpOut @labels(TCPSegment,IPControlInfo/up);
        output udpOut @labels(UDPPacket,IPControlInfo/up);
        output sctpOut @labels(IPControlInfo/up,SCTPPacket); //I.R.
        output rsvpOut @labels(IPControlInfo/up);
        output ospfOut @labels(IPControlInfo/up);
        output pingOut;

    submodules:
        // Adapted submodule from IP to ecmpIP.
        ip: IPECMP {
            parameters:
                timeToLive = 32;
                multicastTimeToLive = 32;
                fragmentTimeout = 60s;
                protocolMapping = "6:0,17:1,1:2,2:3,46:4,89:5,132:6";   //I.R.
                @display("p=85,95;q=queue");
            gates:
                transportIn[7]; //I.R.
                transportOut[7];
                queueIn[sizeof(ifIn)];
        }
        arp: ARP {
            parameters:
                proxyARP = proxyARP;
                @display("p=163,206;q=pendingQueue");
            gates:
                nicOut[sizeof(ifOut)];
        }
        icmp: ICMP {
            parameters:
                @display("p=160,63");
        }
        igmp: IGMP {
            parameters:
                @display("p=160,122");
        }
        errorHandling: ErrorHandling {
            parameters:
                @display("p=239,63");
        }
    connections allowunconnected:
        // transport Layer
        ip.transportOut[0] --> tcpOut;
        ip.transportIn[0] <-- tcpIn;

        ip.transportOut[1] --> udpOut;
        ip.transportIn[1] <-- udpIn;

        ip.transportOut[2] --> icmp.localIn;
        ip.transportIn[2] <-- icmp.sendOut;

        ip.transportOut[3] --> igmp.localIn;
        ip.transportIn[3] <-- igmp.sendOut;

        ip.transportOut[4] --> rsvpOut;
        ip.transportIn[4] <-- rsvpIn;

        ip.transportOut[5] --> ospfOut;
        ip.transportIn[5] <-- ospfIn;

        ip.transportOut[6] --> sctpOut;     //I.R.
        ip.transportIn[6] <-- sctpIn;

        icmp.pingOut --> pingOut;
        icmp.pingIn <-- pingIn;

        icmp.errorOut --> errorHandling.in;

        ip.queueOut --> arp.ipIn;

        // L2 interfaces to IP and from ARP
        for i=0..sizeof(ifOut)-1 {
            ifIn[i] --> { @display("m=s"); } --> ip.queueIn[i];
            ifOut[i] <-- { @display("m=s"); } <-- arp.nicOut[i];
        }
}

