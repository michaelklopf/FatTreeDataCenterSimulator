//
// Author: Michael Klopf
// 

#include "TrafficGenerationThread.h"
#include "StartClient_m.h"
#include "StopClient_m.h"
#include "ReplyMsg_m.h"
#include "RequestMsg_m.h"

Register_Class(TrafficGenerationThread);

void TrafficGenerationThread::established()
{
	hostmodule = check_and_cast<TGAppWithIAT*>(getHostModule());
	scheduleMsg = new cMessage("scheduling");
	startstop = false;
	if (hostmodule->isClientOn()) {
	    scheduleAt(simTime(), scheduleMsg);
	}
}

void TrafficGenerationThread::dataArrived(cMessage *msg, bool)
{

	ReplyMsg *replyMsg = dynamic_cast<ReplyMsg *>(msg);

	if (replyMsg) {
	    simtime_t arrivalTime = simTime();
	    simtime_t departureTime = (simtime_t)replyMsg->getDepTime();
	    simtime_t cycleTime = arrivalTime - departureTime;
	    hostmodule->emitProcessingTime(cycleTime);

	    delete replyMsg;

	    /*
	    if (hostmodule->isFirstSend()) {
	        double delay = exponential(hostmodule->getCurrentIat());
	        hostmodule->emitIAT(delay);
	        scheduleAt(simTime()+(simtime_t)delay, scheduleMsg);
	        hostmodule->setFirstSend(false);
	    }
	    */
		return;
	}

    opp_error("Message (%s)%s is not a StartClient, nor StopClient, or ReplyMsg message -- "
              "probably wrong client app, or wrong setting of TCP's "
              "sendQueueClass/receiveQueueClass parameters "
              "(try \"TCPMsgBasedSendQueue\" and \"TCPMsgBasedRcvQueue\")",
              msg->getClassName(), msg->getName());
}

void TrafficGenerationThread::timerExpired(cMessage *timer)
{
	// Create new request msg and send it to the controller for further processing.
	//bool serverClose = true; // we want our connections closed afterwards
    long requestLength = hostmodule->par("requestLength");
    long replyLength = hostmodule->par("replyLength");
    if (requestLength < 1)
    	requestLength = 1;
    if (replyLength < 1)
    	replyLength = 1;

	RequestMsg *reqmsg = new RequestMsg("requestTG");
	reqmsg->setByteLength(requestLength);
	reqmsg->setExpectedReplyLength(replyLength);
	//reqmsg->setServerClose(serverClose);
	reqmsg->setReplyDelay((double)hostmodule->par("replyDelay"));
	reqmsg->setServiceport((int)hostmodule->par("port"));
	reqmsg->setServerClose(true);
	reqmsg->setDepTime(simTime().dbl());

	hostmodule->getConSock()->send(reqmsg);

    double delay = exponential(hostmodule->getCurrentIat());
    hostmodule->emitIAT(delay);
    scheduleAt(simTime()+(simtime_t)delay, scheduleMsg);
}

void TrafficGenerationThread::closed()
{
	hostmod->removeThread(this);
	if (!startstop) {
	    delete scheduleMsg;
	}
}

void TrafficGenerationThread::failure()
{
	//hostmod->removeThread(this);
	opp_error("TrafficGenThread failure. Shouldn't happen.");
}

void TrafficGenerationThread::peerClosed()
{
    if (getSocket()->getState()==TCPSocket::PEER_CLOSED)
    {
        EV << "remote TCP closed, closing here as well\n";
        getSocket()->close();
    }
}
