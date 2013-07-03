//
// Author: Michael Klopf
// 

#include "ExtTrafficGenerationApp.h"
#include <math.h>

Define_Module(ExtTrafficGenerationApp);

void ExtTrafficGenerationApp::computeIAT()
{
    // compute difference of max/min load, take load percentage of that, add it to min load
    double iatInt = fabs(iatMin - iatMax);
    double support = iatInt*currentLoadLevel;
    currentIAT = iatMin - support;
}

void ExtTrafficGenerationApp::initialize(int stage)
{
    if (stage == 0) {
        double load[13] = {0.4,0.2,0.2,0.4,0.5,0.6,0.8,0.8,0.5,0.4,0.6,0.5,0.4};

        int loadArrayLength = sizeof(load)/sizeof(double);

        loadLevels = new std::deque<double>();

        for (int i=0; i < loadArrayLength; i++) {
            loadLevels->push_back(load[i]);
        }

        TCPSrvHostApp::initialize();

        this->processingTime = registerSignal("processingTime");
        this->interArrivalTime = registerSignal("interArrivalTime");
        this->clientOn = par("clientOn");

        this->iatMax = par("iatMax");
        this->iatMin = par("iatMin");

        this->loadAdaptionInterval = par("loadAdaptionInterval");
        if(loadAdaptionInterval <= 0) {
            opp_error("loadAdaptionInterval was not correctly specified.");
        }

        currentLoadLevel = 0;
        step = 0;

        computeIAT();

        // Create socket to controller.
        conSock = new TCPSocket();
        conSock->setOutputGate(gate("tcpOut"));

        const char *serverThreadClass = par("serverThreadClass");
        DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
        conSock->setCallbackObject(proc);
        proc->init(this, conSock);
        // Add socket to socket map.
        SocketMapExt socketMap = getSocketMap();
        socketMap.addSocket(conSock);

        conSock->bind((int)par("port"));

        // remote addr and port
        conSock->connect(IPvXAddress(par("connectAddress")), (int)par("connectPort"));


        if (isClientOn()) {
            // Create new request msg and send it to the controller for further processing.
            //bool serverClose = true; // we want our connections closed afterwards
            /*
            long requestLength = par("requestLength");
            long replyLength = par("replyLength");
            if (requestLength < 1)
                requestLength = 1;
            if (replyLength < 1)
                replyLength = 1;

            RequestMsg *reqmsg = new RequestMsg("requestTG");
            reqmsg->setByteLength(requestLength);
            reqmsg->setExpectedReplyLength(replyLength);
            //reqmsg->setServerClose(serverClose);
            reqmsg->setReplyDelay((double)par("replyDelay"));
            reqmsg->setServiceport((int)par("port"));
            reqmsg->setServerClose(true);

            reqmsg->setDepTime(simTime().dbl());

            conSock->send(reqmsg);*/

            TTimer *newScheduleMsg = new TTimer("updatingLoadLevel");
            scheduleAt(simTime(), newScheduleMsg);
        }
    }
}

void ExtTrafficGenerationApp::handleMessage(cMessage *msg)
{
    TTimer *updateLoadLevel = dynamic_cast<TTimer *>(msg);

    if (updateLoadLevel) {
        if (step >= (int)loadLevels->size()) {
            opp_warning("out of load array bounds");
            delete updateLoadLevel;
        } else {
            currentLoadLevel = loadLevels->at(step);
            step++;
            computeIAT();
            scheduleAt(simTime()+loadAdaptionInterval, updateLoadLevel);
        }
    } else {
        DCTCPControllerApp::handleMessage(msg);
    }
}

void ExtTrafficGenerationApp::emitProcessingTime(simtime_t procTime)
{
	emit(processingTime, procTime.dbl());
}

void ExtTrafficGenerationApp::emitIAT(double iatValue)
{
    emit(interArrivalTime, iatValue);
}

void ExtTrafficGenerationApp::finish()
{
    delete conSock;
    delete loadLevels;
}
