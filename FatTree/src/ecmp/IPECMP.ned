
package fattree.ecmp;

simple IPECMP
{
    parameters:
        double procDelay @unit("s") = default(0s);
        int timeToLive = default(32);
        int multicastTimeToLive;
        string protocolMapping;
        double fragmentTimeout @unit("s") = default(60s);
        @display("i=block/routing");
    gates:
        input transportIn[] @labels(IPControlInfo/down,TCPSegment,UDPPacket);
        output transportOut[] @labels(IPControlInfo/up,TCPSegment,UDPPacket);
        input queueIn[] @labels(IPDatagram);
        output queueOut @labels(IPDatagram);
}

