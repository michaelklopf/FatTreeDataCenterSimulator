//
// Author: Michael Klopf
// 

#ifndef VIRTUALMACHINETHREAD_H_
#define VIRTUALMACHINETHREAD_H_

#include <omnetpp.h>
#include "VirtualMachineApp.h"
#include "SocketMapExt.h"
#include "RequestMsg_m.h"
#include "VMMigMsg_m.h"
#include "StartVM_m.h"
#include "ShutDownVM_m.h"
#include <IPAddress.h>

/*
 * Handling of a final state machine for virtual machine migration.
 */
class VirtualMachineThread: public DCTCPControllerThreadBase {
    protected:
        SocketMapExt socketMap;
        cFSM *fsm;
        VMMigMsg *migReqMsg;
        VMMigMsg *changeIPMsg;
        StartVM *vmstartmsg;
        ShutDownVM *vmstopmsg;
        bool isVM1;
        bool isVM2;
        bool isReq;
        bool comingFromHV;
        bool waitingForStartTimeSet;
        bool waitingForHVStartUp;
        bool serverIsShuttingDown;
        enum {
           INIT = 0,
           VMOFF = FSM_Steady(1),
           IDLE = FSM_Steady(2),
           BUSY = FSM_Steady(3),
           //POWERINGON = FSM_Steady(4),
           //POWERINGDOWN = FSM_Steady(5),
        };
    protected:
        virtual void handleRequestMsg(RequestMsg *reqmsg);
        virtual void handleVirtualMachineMigration(VMMigMsg *vmmsg);
        virtual void handleVMStartUp(StartVM *startvm);
        virtual void handleVMShutDown(ShutDownVM *stopvm);
        // Methods for changing vm ip addresses.
        virtual void setIPAddressForVM2(VMMigMsg *vmmsg);
        virtual void setIPAddressForVM1(VMMigMsg *vmmsg);
        virtual void initiateReRouting(VMMigMsg *vmmsg);
        //virtual uint32 getNewVMIP();

    public:
        VirtualMachineThread() {
            migReqMsg = NULL;
            isVM1 = false;
            isVM2 = false;
            isReq = false;
        };
        virtual ~VirtualMachineThread() {};

        virtual void established();
        virtual void dataArrived(cMessage *msg, bool urgent);
        virtual void timerExpired(cMessage *timer);
        virtual void closed(); //{hostmod->removeThread(this);EV << "Hack the VMThread.\n";};
        virtual void peerClosed(); //{getSocket()->close();EV << "Hack the VMThread.\n";};
};

#endif /* VIRTUALMACHINETHREAD_H_ */
