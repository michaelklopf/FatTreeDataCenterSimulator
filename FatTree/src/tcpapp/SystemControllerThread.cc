//
// Author: Michael Klopf
// 

#include "SystemControllerThread.h"
#include "SystemController.h"
#include "StartVM_m.h"
#include "ShutDownVM_m.h"
#include "VMMigMsg_m.h"
#include "StopClient_m.h"
#include "StartClient_m.h"

Register_Class(SystemControllerThread);

void SystemControllerThread::established()
{
    // empty
}

void SystemControllerThread::dataArrived(cMessage *msg, bool)
{
	StartVM *startvm = dynamic_cast<StartVM *>(msg);

	if (startvm) {
		IPvXAddress *addr = new IPvXAddress(startvm->getVmaddress());
		SystemController *con = check_and_cast<SystemController *>(getHostModule());
		// make changes at traffic controller
		DCTCPControllerApp *trafcon = check_and_cast<DCTCPControllerApp *>(con->findTrafficControllerModule());
		IPvXAddress hvaddr = trafcon->getServers()->getServerAddr(*addr);
		if (startvm->getServerState()) {    // if on, change state
	        trafcon->getServerStates()->changeState(hvaddr, startvm->getServerState());
	        //std::cerr << "DEBUG: Syscon Activated server." << hvaddr << " at " << simTime().dbl() << "\n";
	        trafcon->updateListsVMStart(*addr, startvm->getServiceport());
		} else {
		    trafcon->getActivatingVMs()->removeVM(*addr);
		}

		//hostmod->getPortList()->push_back(getSocket()->getLocalPort());
		getSocket()->close();

		delete startvm;
		delete addr;
		return;
	}

	ShutDownVM *stopvm = dynamic_cast<ShutDownVM *>(msg);

	if (stopvm) {
		IPvXAddress *addr = new IPvXAddress(stopvm->getVmaddress());
		SystemController *con = check_and_cast<SystemController *>(getHostModule());
		// make changes at traffic controller
		DCTCPControllerApp *trafcon = check_and_cast<DCTCPControllerApp *>(con->findTrafficControllerModule());
		IPvXAddress hvaddr = trafcon->getServers()->getServerAddr(*addr);
        if (!stopvm->getServerState()) { // if off, change state
            trafcon->getServerStates()->changeState(hvaddr, stopvm->getServerState());
            //std::cerr << "DEBUG: Syscon Deactivated server." << hvaddr << " at " << simTime().dbl() << "\n";
        }
		trafcon->updateListsVMShutDown(*addr, stopvm->getServiceport());

		//hostmod->getPortList()->push_back(getSocket()->getLocalPort());
		getSocket()->close();

		delete stopvm;
		delete addr;
		return;
	}

	/*deprecated*/
	StopClient *stopCl = dynamic_cast<StopClient *>(msg);

	if (stopCl) {
	    IPvXAddress addr = IPvXAddress(stopCl->getClientAddr());
	    int port = stopCl->getServiceport();
	    check_and_cast<SystemController*>(getHostModule())->updateDeactivatedClients(addr, port);
	    delete stopCl;
	    //getSocket()->close();
	    return;
	}

	/*deprecated*/
	StartClient *startCl = dynamic_cast<StartClient *>(msg);

	if (startCl) {
	    IPvXAddress addr = IPvXAddress(startCl->getClientAddr());
	    int port = startCl->getServiceport();
	    check_and_cast<SystemController*>(getHostModule())->updateActiveClients(addr, port);
	    delete startCl;
	    //getSocket()->close();
	    return;
	}

	VMMigMsg *vmmig = dynamic_cast<VMMigMsg *>(msg);

	if (vmmig) {
		SystemController *con = check_and_cast<SystemController *>(getHostModule());
		// make changes at traffic controller
		DCTCPControllerApp *trafcon = check_and_cast<DCTCPControllerApp *>(con->findTrafficControllerModule());
		trafcon->finishedVMMigration(vmmig);
		//getSocket()->close();
	}

    if (!vmmig)
        opp_error("Message (%s)%s is not a StartVMMsg, ShutDownMsg, StopCl, StartCl, VMMigMsg -- "
                  "probably wrong client app, or wrong setting of TCP's "
                  "sendQueueClass/receiveQueueClass parameters "
                  "(try \"TCPMsgBasedSendQueue\" and \"TCPMsgBasedRcvQueue\")",
                  msg->getClassName(), msg->getName());
}

void SystemControllerThread::timerExpired(cMessage *timer)
{
    // empty
}

void SystemControllerThread::closed()
{
    hostmod->getPortList()->push_back(getSocket()->getLocalPort());
    hostmod->removeThread(this);
}

void SystemControllerThread::peerClosed()
{
    /*
    if (getSocket()->getState()==TCPSocket::PEER_CLOSED)
    {
        EV << "remote TCP closed, closing here as well\n";
        getSocket()->close();
    }
    */
}
