//
// Author: Michael Klopf
// 

#include "TGAppWithIAT.h"
#include <math.h>

Define_Module(TGAppWithIAT);

void TGAppWithIAT::computeIAT(double currentLoadLevel)
{
    if (currentLoadLevel == 0) {
        opp_error("Dividing through zero.");
    }
    //currentIAT = iatMean + iatMean * (1-currentLoadLevel);
    currentIAT = iatMean * 1/currentLoadLevel;
}

void TGAppWithIAT::initialize(int stage)
{
    if (stage == 0) {
        // Starting at 2am
        //double load[13] = {0.4,0.2,0.2,0.4,0.5,0.6,0.8,0.8,0.5,0.4,0.6,0.5,0.4};
        double load[1] = {1};
        // Starting at 0:00
        double loadAdap[13] = {0.79, 0.52, 0.22, 0.13, 0.10, 0.40, 0.72, 0.74, 0.77, 0.80, 0.61, 0.73, 0.79};

        int loadLen = sizeof(load)/sizeof(double);
        int loadAdapLen = sizeof(loadAdap)/sizeof(double);

        loadLevels = new std::deque<double>();
        loadAdaptation = par("loadAdaptation");

        if (loadAdaptation) {
            for (int i=0; i < loadAdapLen; i++) {
                loadLevels->push_back(loadAdap[i]);
            }
        } else {
            for (int i=0; i < loadLen; i++) {
                loadLevels->push_back(load[i]);
            }
        }

        initializeSignals();

        TCPSrvHostApp::initialize();

        this->clientOn = par("clientOn");

        //this->iatMax = par("iatMax");
        //this->iatMin = par("iatMin");
        this->iatMean = par("iatMean");

        this->loadAdaptionInterval = par("loadAdaptionInterval");
        if(loadAdaptionInterval <= 0) {
            opp_error("loadAdaptionInterval was not correctly specified.");
        }

        step = 0;

        warmUpInterval = par("warmUpInterval");

        computeIAT(loadLevels->at(step));

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

            conSock->send(reqmsg);

            TTimer *newScheduleMsg = new TTimer("updatingLoadLevel");
            scheduleAt(simTime()+simulation.getWarmupPeriod(), newScheduleMsg);
        }
    }
}

void TGAppWithIAT::handleMessage(cMessage *msg)
{
    TTimer *updateLoadLevel = dynamic_cast<TTimer *>(msg);

    if (updateLoadLevel) {
        step++;
        if (step >= (int)loadLevels->size()) {
            opp_warning("out of load array bounds");
            delete updateLoadLevel;
        } else {
            computeIAT(loadLevels->at(step));
            scheduleAt(simTime()+loadAdaptionInterval, updateLoadLevel);
        }
    } else {
        DCTCPControllerApp::handleMessage(msg);
    }
}

void TGAppWithIAT::emitProcessingTime(simtime_t procTime)
{
    double currentTime = simTime().dbl();
    if ((int)currentTime%(int)loadAdaptionInterval >= warmUpInterval) {
        if (step == 0) {
            simsignal_t processingTime = processSigs[step];
            emit(processingTime, procTime.dbl());
        } else {
            simsignal_t processingTime = processSigs[step-1];
            emit(processingTime, procTime.dbl());
        }
    }
}

void TGAppWithIAT::emitIAT(double iatValue)
{
    double currentTime = simTime().dbl();
    if ((int)currentTime%(int)loadAdaptionInterval >= warmUpInterval) {
        if (step == 0) {
            simsignal_t interArrivalTime = iatSigs[step];
            emit(interArrivalTime, iatValue);
        } else {
            simsignal_t interArrivalTime = iatSigs[step-1];
            emit(interArrivalTime, iatValue);
        }
    }
}

void TGAppWithIAT::finish()
{
    delete conSock;
    delete loadLevels;
}

void TGAppWithIAT::initializeSignals()
{
    this->processingTime1 = registerSignal("processingTime1");
    processSigs[0] = processingTime1;
    this->interArrivalTime1 = registerSignal("interArrivalTime1");
    iatSigs[0] = interArrivalTime1;

    this->processingTime2 = registerSignal("processingTime2");
    processSigs[1] = processingTime2;
    this->interArrivalTime2 = registerSignal("interArrivalTime2");
    iatSigs[1] = interArrivalTime2;

    this->processingTime3 = registerSignal("processingTime3");
    processSigs[2] = processingTime3;
    this->interArrivalTime3 = registerSignal("interArrivalTime3");
    iatSigs[2] = interArrivalTime3;

    this->processingTime4 = registerSignal("processingTime4");
    processSigs[3] = processingTime4;
    this->interArrivalTime4 = registerSignal("interArrivalTime4");
    iatSigs[3] = interArrivalTime4;

    this->processingTime5 = registerSignal("processingTime5");
    processSigs[4] = processingTime5;
    this->interArrivalTime5 = registerSignal("interArrivalTime5");
    iatSigs[4] = interArrivalTime5;

    this->processingTime6 = registerSignal("processingTime6");
    processSigs[5] = processingTime6;
    this->interArrivalTime6 = registerSignal("interArrivalTime6");
    iatSigs[5] = interArrivalTime6;

    this->processingTime7 = registerSignal("processingTime7");
    processSigs[6] = processingTime7;
    this->interArrivalTime7 = registerSignal("interArrivalTime7");
    iatSigs[6] = interArrivalTime7;

    this->processingTime8 = registerSignal("processingTime8");
    processSigs[7] = processingTime8;
    this->interArrivalTime8 = registerSignal("interArrivalTime8");
    iatSigs[7] = interArrivalTime8;

    this->processingTime9 = registerSignal("processingTime9");
    processSigs[8] = processingTime9;
    this->interArrivalTime9 = registerSignal("interArrivalTime9");
    iatSigs[8] = interArrivalTime9;

    this->processingTime10 = registerSignal("processingTime10");
    processSigs[9] = processingTime10;
    this->interArrivalTime10 = registerSignal("interArrivalTime10");
    iatSigs[9] = interArrivalTime10;

    this->processingTime11 = registerSignal("processingTime11");
    processSigs[10] = processingTime11;
    this->interArrivalTime11 = registerSignal("interArrivalTime11");
    iatSigs[10] = interArrivalTime11;

    this->processingTime12 = registerSignal("processingTime12");
    processSigs[11] = processingTime12;
    this->interArrivalTime12 = registerSignal("interArrivalTime12");
    iatSigs[11] = interArrivalTime12;

}
