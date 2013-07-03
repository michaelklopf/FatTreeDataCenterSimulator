//
// Author: Michael Klopf
// 

#include "DCTrafGenApp.h"
#include "RequestMsg_m.h"
#include "IPAddressResolver.h"
#include "ReplyMsg_m.h"

Define_Module(DCTrafGenApp);

void DCTrafGenApp::initialize()
{
	TCPBasicClientApp::initialize();

	this->processingTime = registerSignal("processingTime");

	this->departureTime = 0;
	this->arrivalTime = 0;
}

void DCTrafGenApp::socketDataArrived(int connId, void *ptr, cPacket *msg, bool urgent)
{
	arrivalTime = simTime();
	EV << "SCT: 10. Job finished.\n";
    TCPBasicClientApp::socketDataArrived(connId, ptr, msg, urgent);

    simtime_t cycleTime = arrivalTime - departureTime;
    emit(processingTime, cycleTime.dbl());
    // Application deletes msg itself and initiate next msg.
}

void DCTrafGenApp::sendPacket(int numBytes, int expectedReplyBytes, bool serverClose)
{
    serverClose = true; // we want our connections closed afterwards

    //EV << "sending " << numBytes << " bytes, expecting " << expectedReplyBytes << (serverClose ? ", and server should close afterwards\n" : "\n");

    RequestMsg *msg = new RequestMsg("requestTG");
    msg->setByteLength(numBytes);
    msg->setExpectedReplyLength(expectedReplyBytes);
    msg->setServerClose(serverClose);
    msg->setReplyDelay((double)par("replyDelay"));
    msg->setServiceport((int)par("port"));

    //EV << "TrafGen:Send message " << msg->getClassName() << " , " << msg->getName() << " from TG " <<  addr.str().c_str() << " to traffic con.\n";
    EV << "SCT: 1. Sending " << msg->getClassName() << ".\n";

    departureTime = simTime();

    socket.send(msg);

    packetsSent++;
    bytesSent+=numBytes;
}
