//
// Author: Michael Klopf
// 

#ifndef HYPERVISORTHREAD_H_
#define HYPERVISORTHREAD_H_

#include <omnetpp.h>
#include "HyperVisorApp.h"
#include "SocketMapExt.h"
#include "VMMigMsg_m.h"
#include "VirtualMachineApp.h"
#include "StartHV_m.h"
#include "ShutDownHV_m.h"

/*
 * Handling of a finite state machine for virtual machine migration.
 */
class HyperVisorThread: public DCTCPControllerThreadBase {
    protected:
        SocketMapExt socketMap;
        SocketMapExt *sockets;
        cFSM *fsm;
        int connIdVMSock;
        enum {
           INIT = 0,
           HVOFF = FSM_Steady(1),
           ACTIVE = FSM_Steady(2),
           COPYING = FSM_Steady(3),
           //POWERINGON = FSM_Steady(4),
           //POWERINGDOWN = FSM_Steady(5),
        };

    protected:
        /**
         * Method to handle vm migration instructions.
         */
        virtual void handleVirtualMachineMigration(VMMigMsg *vmmsg);
        /**
         * Method to handle the start up of the hypervisor/server.
         */
        virtual void handleHyperVisorStart(StartHV *hvstart);
        /**
         * Method to handle shut down of server.
         */
        virtual void handleHyperVisorShutDown(ShutDownHV *hvstop);
        /*
         * Check if any VM being part of the hypervisor is still on
         * and turn all VMs off.
         */
        virtual void checkAndPowerDownVMs();
        //virtual void changeVM1Address(VMMigMsg *vmmsg);
        //virtual void changeVM2Address(VMMigMsg *vmmsg);

    public:
        HyperVisorThread() {};
        virtual ~HyperVisorThread() {};

        virtual void established();
        virtual void dataArrived(cMessage *msg, bool urgent);
        virtual void timerExpired(cMessage *timer);
        virtual void closed() {
            if (getSocket()->getLocalAddress() == IPvXAddress()) {
                return;
            } else if (getSocket()->getRemoteAddress() == IPvXAddress()) {
                return;
            }
            /*
            const char *addrchar1 = getSocket()->getLocalAddress().str().c_str();
            const char *addrchar = getSocket()->getRemoteAddress().str().c_str();
            std::cerr << "Removing socket from sockmap " << addrchar1 << " and " << addrchar << "\n";
            */
            //getHostModule()->getSocketMaps()->printSockets();
            hostmod->removeThread(this);
            // TODO check for empty socket
            //getHostModule()->getSocketMaps()->printSockets();
        }
        virtual void peerClosed() {
            if (getSocket()->getLocalAddress() == IPvXAddress()) {
                return;
            } else if (getSocket()->getRemoteAddress() == IPvXAddress()) {
                return;
            } else if (getSocket()->getState()==TCPSocket::PEER_CLOSED) {
                /*
                const char *addrchar1 = getSocket()->getLocalAddress().str().c_str();
                const char *addrchar = getSocket()->getRemoteAddress().str().c_str();
                std::cerr << "Peer Closed " << addrchar1 << " and " << addrchar << "\n";
                */
                getSocket()->close();
            }/*getSocket()->close();*/
        }
};

#endif /* HYPERVISORTHREAD_H_ */
