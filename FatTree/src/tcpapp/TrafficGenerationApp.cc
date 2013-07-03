//
// Author: Michael Klopf
//


#include "TrafficGenerationApp.h"
#include "IPAddressResolver.h"
#include "GenericAppMsg_m.h"
#include "RequestMsg_m.h"

#define MSGKIND_CONNECT  0
#define MSGKIND_SEND     1


Define_Module(TrafficGenerationApp);

TrafficGenerationApp::TrafficGenerationApp()
{
    timeoutMsg = NULL;
}

TrafficGenerationApp::~TrafficGenerationApp()
{
    cancelAndDelete(timeoutMsg);
}

void TrafficGenerationApp::initialize()
{
    numSessions = numBroken = packetsSent = packetsRcvd = bytesSent = bytesRcvd = 0;
    WATCH(numSessions);
    WATCH(numBroken);
    WATCH(packetsSent);
    WATCH(packetsRcvd);
    WATCH(bytesSent);
    WATCH(bytesRcvd);

    // parameters
    const char *address = par("address");
    int port = par("port");
    socket.bind(*address ? IPvXAddress(address) : IPvXAddress(), port);

    socket.setCallbackObject(this);
    socket.setOutputGate(gate("tcpOut"));

    setStatusString("waiting");

    timeoutMsg = new cMessage("timer");

    numRequestsToSend = 0;
    earlySend = false;  // TBD make it parameter
    WATCH(numRequestsToSend);
    WATCH(earlySend);

    timeoutMsg->setKind(MSGKIND_CONNECT);

	this->processingTime = registerSignal("processingTime");

	this->departureTime = 0;
	this->arrivalTime = 0;

    scheduleAt((simtime_t)par("startTime"), timeoutMsg);
}

void TrafficGenerationApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
        handleTimer(msg);
    else
        socket.processMessage(msg);
}

void TrafficGenerationApp::sendRequest()
{
     EV << "sending request, " << numRequestsToSend-1 << " more to go\n";
     if(ev.isGUI()) {
    	 getParentModule()->bubble("Sending Message.");
     }

     long requestLength = par("requestLength");
     long replyLength = par("replyLength");
     if (requestLength<1) requestLength=1;
     if (replyLength<1) replyLength=1;

     sendPacket(requestLength, replyLength);
}

void TrafficGenerationApp::handleTimer(cMessage *msg)
{
    switch (msg->getKind())
    {
        case MSGKIND_CONNECT:
            EV << "starting session\n";
            connect(); // active OPEN

            // significance of earlySend: if true, data will be sent already
            // in the ACK of SYN, otherwise only in a separate packet (but still
            // immediately)
            if (earlySend)
                sendRequest();
            break;

        case MSGKIND_SEND:
           sendRequest();
           numRequestsToSend--;
           // no scheduleAt(): next request will be sent when reply to this one
           // arrives (see socketDataArrived())
           break;
    }
}

void TrafficGenerationApp::socketEstablished(int connId, void *ptr)
{
    // *redefine* to perform or schedule first sending
    EV << "connected\n";
    setStatusString("connected");
    if(ev.isGUI()) {
    	getParentModule()->bubble("Socket established.");
    }

    // determine number of requests in this session
    numRequestsToSend = (long) par("numRequestsPerSession");
    if (numRequestsToSend<1) numRequestsToSend=1;

    // perform first request if not already done (next one will be sent when reply arrives)
    if (!earlySend)
        sendRequest();
    numRequestsToSend--;
}

void TrafficGenerationApp::socketDataArrived(int connId, void *ptr, cPacket *msg, bool urgent)
{
	arrivalTime = simTime();
	EV << "SCT: 10. Job finished.\n";

    simtime_t cycleTime = arrivalTime - departureTime;
    emit(processingTime, cycleTime.dbl());

    // *redefine* to perform or schedule next sending
    packetsRcvd++;
    bytesRcvd+=msg->getByteLength();

    delete msg;

    if (numRequestsToSend>0)
    {
    	if(ev.isGUI()) {
    		getParentModule()->bubble("Received Message.");
    	}
        EV << "reply arrived\n";
        timeoutMsg->setKind(MSGKIND_SEND);
        scheduleAt(simTime()+(simtime_t)par("thinkTime"), timeoutMsg);
    }
    else
    {
        EV << "reply to last request arrived, closing session\n";
        close();
        if(ev.isGUI()) {
        	getParentModule()->bubble("Closing Socket.");
        }
    }
}

void TrafficGenerationApp::sendPacket(int numBytes, int expectedReplyBytes, bool serverClose)
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

void TrafficGenerationApp::socketClosed(int connId, void *ptr)
{
    // *redefine* to start another session etc.
    EV << "connection closed\n";
    setStatusString("closed");

    // start another session after a delay
    timeoutMsg->setKind(MSGKIND_CONNECT);
    scheduleAt(simTime()+(simtime_t)par("idleInterval"), timeoutMsg);
}

void TrafficGenerationApp::socketFailure(int connId, void *ptr, int code)
{
    // subclasses may override this function, and add code try to reconnect after a delay.
    EV << "connection broken\n";
    setStatusString("broken");

    numBroken++;

    // reconnect after a delay
    timeoutMsg->setKind(MSGKIND_CONNECT);
    scheduleAt(simTime()+(simtime_t)par("reconnectInterval"), timeoutMsg);
}

void TrafficGenerationApp::connect()
{
    // we need a new connId if this is not the first connection
    socket.renewSocket();

    // connect
    const char *connectAddress = par("connectAddress");
    int connectPort = par("connectPort");

    EV << "issuing OPEN command\n";
    setStatusString("connecting");

    socket.connect(IPAddressResolver().resolve(connectAddress), connectPort);

    numSessions++;
}

void TrafficGenerationApp::close()
{
    setStatusString("closing");
    EV << "issuing CLOSE command\n";
    socket.close();
}

void TrafficGenerationApp::setStatusString(const char *s)
{
    if (ev.isGUI()) getDisplayString().setTagArg("t", 0, s);
}

void TrafficGenerationApp::socketPeerClosed(int, void *)
{
    // close the connection (if not already closed)
    if (socket.getState()==TCPSocket::PEER_CLOSED)
    {
        EV << "remote TCP closed, closing here as well\n";
        close();
    }
}

void TrafficGenerationApp::finish()
{
    EV << getFullPath() << ": opened " << numSessions << " sessions\n";
    EV << getFullPath() << ": sent " << bytesSent << " bytes in " << packetsSent << " packets\n";
    EV << getFullPath() << ": received " << bytesRcvd << " bytes in " << packetsRcvd << " packets\n";

    recordScalar("number of sessions", numSessions);
    recordScalar("packets sent", packetsSent);
    recordScalar("packets rcvd", packetsRcvd);
    recordScalar("bytes sent", bytesSent);
    recordScalar("bytes rcvd", bytesRcvd);
}
