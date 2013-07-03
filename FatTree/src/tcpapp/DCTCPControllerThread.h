//
// A thread which is used by DCTCPControllerApp and extending
// the DCTCPControllerThreadBase to handle messages of type
// RequestMsg, ReplyMsg, VMMigMsg
//
// Author: Michael Klopf
//

#ifndef DCTCPCONTROLLERTHREAD_H_
#define DCTCPCONTROLLERTHREAD_H_

#include <omnetpp.h>
#include <IPvXAddress.h>
#include "DCTCPControllerApp.h"
#include "RequestMsg_m.h"
#include "ReplyMsg_m.h"
#include "VMMigMsg_m.h"
#include "SocketMapExt.h"
#include "ServiceMap.h"

/**
 * TCP thread application for controller to relay messages to the right host.
 * Also handling of a final state machine for virtual machine migration.
 */
class DCTCPControllerThread: public DCTCPControllerThreadBase{
    protected:
        ServiceMap *activeVMs;
        ServiceMap *allVMs;
        ServiceMap *freeVMs;

        typedef std::deque<RequestMsg> JobQueue;
        JobQueue *jobQueue;
        JobQueue *webJobQueue;
        JobQueue *ftpJobQueue;

        //TCPHVSet *servers;

        SocketMapExt socketMap;
        cFSM *fsm;
        enum {
            INIT = 0,
            IDLE = FSM_Steady(1),
            STARTINGHOST = FSM_Steady(2),
            COPYING = FSM_Steady(3),
            MIGRATION = FSM_Steady(4),
        };
    protected:
        virtual void handleRequestMsg(RequestMsg *reqmsg);
        virtual void handleReplyMsg(ReplyMsg *repmsg);
        virtual void processRequestMsg(RequestMsg *reqmsg);
        virtual void handleVirtualMachineMigration(VMMigMsg *vmmsg);
        virtual void checkAndSetVMsInactive(VMMigMsg *vmmsg);
        virtual void emitJobQueueLengthForService(int serviceport);
    public:
        DCTCPControllerThread() {};

        virtual void established();
        /*
         * Checks the type of the incoming message.
         *
         * When request message type, find a proper virtual machine
         * and process message to this vm. Encapsulate the socket
         * information from this incoming message into the message,
         * which will be send to the vm.
         *
         * When reply message type, extract connId from encapsulated
         * message to send message back to the correct traffic
         * generator.
         *
         */
        virtual void dataArrived(cMessage *msg, bool urgent);
        virtual void timerExpired(cMessage *timer);
        virtual void closed() {hostmod->removeThread(this);}
        virtual void peerClosed(); //{getSocket()->close();EV << "Hack the ConThread.\n";}

        /*
         * Choose an ip address from all possible ones, to send messaget to.
         */
        virtual IPvXAddress chooseVMAddr(int port);
};

#endif /* DCTCPCONTROLLERTHREAD_H_ */
