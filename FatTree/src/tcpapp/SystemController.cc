//
// Author: Michael Klopf
// 

#include "SystemController.h"
#include "StartVM_m.h"
#include "ShutDownVM_m.h"
#include "IPAddressResolver.h"
#include "StopClient_m.h"

Define_Module(SystemController);

void SystemController::fillInTimeAndPercentagePairs()
{
    bool dncycleOn = par("dncycle");
    if (dncycleOn) {
        int j=0;
        double load[13] = {0.4,0.2,0.2,0.4,0.5,0.6,0.8,0.8,0.5,0.4,0.6,0.5,0.4};
        for (int i = 0; i<=43200; i=i+3600) {
            timePercentagePairs->addNewEntry(i, load[j]);
            j++;
        }
    } else {
        timePercentagePairs->addNewEntry(0,1);
    }
    //timePercentagePairs->addNewEntry(0, 0.2);
    //timePercentagePairs->addNewEntry(120, 0.4);
    //timePercentagePairs->addNewEntry(240, 0.6);
    //timePercentagePairs->addNewEntry(360, 0.8);
    //timePercentagePairs->addNewEntry(480, 0.2);
    //timePercentagePairs->addNewEntry(600, 0.5);
}

void SystemController::initialize(int stage)
{
	if (stage == 2) {
		// fill map with all possible ports.
	    ports = new std::deque<int>();
	    for (int i = 0; i < 65535; i++) {
	    	ports->push_back(i);
	    }
	    this->HTTPPORT = par("HTTPPORT");
	    this->FTPPORT = par("FTPPORT");

	    // create data structures and fill the map with the time/percentage pairs.
	    this->httpClients = new ServiceMap();
	    this->activeHTTPClients = new ServiceMap();
	    this->ftpClients = new ServiceMap();
	    this->activeFTPClients = new ServiceMap();
	    this->timePercentagePairs = new ClientTimeMap();
	    fillInTimeAndPercentagePairs();
	    currentStep = 0;	// for iteration through timePercPairs
	    numActFTPClientsOld = 0;
	    numActHTTPClientsOld = 0;

	    // Find all nodes for the respective service for calling them later.
	    cTopology httptopo("httpNodes");
	    httptopo.extractByProperty("HTTP");

	    cTopology ftptopo("ftpNodes");
	    ftptopo.extractByProperty("FTP");

	    for (int i = 0; i < httptopo.getNumNodes(); i++) {
	    	cModule *trafgen = httptopo.getNode(i)->getModule();
	    	IPvXAddress tgaddr = IPAddressResolver().addressOf(trafgen, IPAddressResolver::ADDR_IPv4);

	    	httpClients->addVM(tgaddr, HTTPPORT);
	    	//delete trafgen;
	    }

	    for (int i = 0; i < ftptopo.getNumNodes(); i++) {
	    	cModule *trafgen = ftptopo.getNode(i)->getModule();
	    	IPvXAddress tgaddr = IPAddressResolver().addressOf(trafgen, IPAddressResolver::ADDR_IPv4);

	    	ftpClients->addVM(tgaddr, FTPPORT);
	    	//delete trafgen;
	    }

	    // schedule the first message for adapting the number of active clients.
	    // Deactivating client start up functionality
	    /*
	    this->timerMsg = new StartClient("Initiating first start.");
	    timerMsg->setClientIsOn(false);
	    scheduleAt((simtime_t)timePercentagePairs->getElement(currentStep), timerMsg);
	    */
	}
}

void SystemController::handleMessage(cMessage *msg)
{
	StartClient *startcl = dynamic_cast<StartClient *>(msg);

	if (startcl) {
	    if (startcl->getClientIsOn()) {
	        DCTCPControllerApp::handleMessage(msg);
	    } else {
	        // start the client adaption process.
	        updateNumberOfActiveClients();
	        //delete startcl;
	        currentStep++;
	        if (currentStep < timePercentagePairs->getSize()) {
	            scheduleAt((simtime_t)timePercentagePairs->getElement(currentStep), startcl);
	        }
	    }
	} else {
		// hand the message over to thread.
		DCTCPControllerApp::handleMessage(msg);
	}
}

void SystemController::startUpVM(IPvXAddress addr, int port)
{
	Enter_Method("startUpVM in SysCon");
	// Create socket to vm.
    TCPSocket nsock;
    nsock.setOutputGate(gate("tcpOut"));

    const char *serverThreadClass = par("serverThreadClass");
    DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
    nsock.setCallbackObject(proc);
    proc->init(this, &nsock);
    socketMap.addSocket(&nsock);

    nsock.bind(ports->front());
    ports->pop_front();

    nsock.connect(addr, port); // remote addr and port
    // Create msg to start the vm.
    StartVM *startmsg = new StartVM("Start VM");
    startmsg->setVmaddress(addr.str().c_str());
    startmsg->setServiceport(port);
    startmsg->setByteLength(1);

    nsock.send(startmsg);
}

void SystemController::shutDownVM(IPvXAddress addr, int port)
{
	Enter_Method("shutDownVM in SysCon");
	// Create socket to vm.
    TCPSocket nsock;
    nsock.setOutputGate(gate("tcpOut"));

    const char *serverThreadClass = par("serverThreadClass");
    DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
    nsock.setCallbackObject(proc);
    proc->init(this, &nsock);
    socketMap.addSocket(&nsock);

    nsock.bind(ports->front());
    ports->pop_front();

    nsock.connect(addr, port); // remote addr and port

    // Msg to shut down VM
    ShutDownVM *stopmsg = new ShutDownVM("Stop VM");
    std::string addrstr = addr.str();
    const char *addrchar = addrstr.c_str();
    stopmsg->setVmaddress(addrchar);
    stopmsg->setServiceport(port);
    stopmsg->setByteLength(1);

    nsock.send(stopmsg);
}

void SystemController::startVMMigration(VMMigMsg *vmmig)
{
	Enter_Method("sending vmmig msg out to controller thread");
	// Create socket to controller.
	TCPSocket nsock;
	nsock.setOutputGate(gate("tcpOut"));

    const char *serverThreadClass = par("serverThreadClass");
    DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
    nsock.setCallbackObject(proc);
    proc->init(this, &nsock);
    socketMap.addSocket(&nsock);

    nsock.bind(ports->front());
    ports->pop_front();

    nsock.connect(IPvXAddress(vmmig->getConaddress()), 1000);

    VMMigMsg *newvmmig = new VMMigMsg(*vmmig);

    delete vmmig;

    nsock.send(newvmmig);
}

cModule *SystemController::findTrafficControllerModule()
{
	return getParentModule()->	// get trafficcontroller
			getParentModule()->  // get controller
			getSubmodule("TrafficController", 0)->
			getSubmodule("tcpApp", 0);
}

void SystemController::updateNumberOfActiveClients()
{
	// get the number of all active clients for the next interval for all services.
	int numActHTTPClients = httpClients->getSize() *
			timePercentagePairs->getPercentageOfActiveClients(timePercentagePairs->
					getElement(currentStep));

	int numActFTPClients = ftpClients->getSize() *
			timePercentagePairs->getPercentageOfActiveClients(timePercentagePairs->
					getElement(currentStep));

	// Compute the number of clients which need to be activated or deactivated.
	int numToChangeHTTPClients = numActHTTPClients - numActHTTPClientsOld;
	numActHTTPClientsOld = numActHTTPClients;
	updateHTTPClients(numToChangeHTTPClients);

	int numToChangeFTPClients = numActFTPClients - numActFTPClientsOld;
	numActFTPClientsOld = numActFTPClients;
	updateFTPClients(numToChangeFTPClients);
}

void SystemController::updateHTTPClients(int numClientsToChange)
{
	if (numClientsToChange > 0) {
		// Starting clients.
		//ServiceMap *deactivatedClients = httpClients->getVMsNotInOtherMap(activeHTTPClients);
		std::auto_ptr<ServiceMap> deactivatedClients(httpClients->getVMsNotInOtherMap(activeHTTPClients));
		for (int i = 0; i < numClientsToChange; i++) {
			// Create socket to client.
			TCPSocket nsock;
			nsock.setOutputGate(gate("tcpOut"));

		    const char *serverThreadClass = par("serverThreadClass");
		    DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
		    nsock.setCallbackObject(proc);
		    proc->init(this, &nsock);
		    socketMap.addSocket(&nsock);

		    nsock.bind(ports->front());
		    ports->pop_front();

		    IPvXAddress addr = deactivatedClients->getIPAddress(i);
		    int clientPort = deactivatedClients->getPortForAddress(addr);
		    nsock.connect(addr, clientPort);

		    StartClient *startMsg = new StartClient("Starting Webclient");
		    startMsg->setByteLength(1);
		    startMsg->setClientIsOn(false);
		    startMsg->setClientAddr(addr.str().c_str());
		    startMsg->setServiceport(clientPort);
		    //startMsg->setDelay(10);

		    nsock.send(startMsg);

            //activeHTTPClients->addVM(addr, clientPort);
		}
		//delete deactivatedClients;
	} else {
		// Stopping clients.
		numClientsToChange = abs(numClientsToChange);
		for (int i = 0; i < numClientsToChange; i++) {
			// Create socket to client.
			TCPSocket nsock;
			nsock.setOutputGate(gate("tcpOut"));

		    const char *serverThreadClass = par("serverThreadClass");
		    DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
		    nsock.setCallbackObject(proc);
		    proc->init(this, &nsock);
		    socketMap.addSocket(&nsock);

		    nsock.bind(ports->front());
		    ports->pop_front();

		    IPvXAddress addr = activeHTTPClients->getIPAddress(i);
		    nsock.connect(addr, activeHTTPClients->getPortForAddress(addr));

		    StopClient *stopMsg = new StopClient("Stopping Webclient");
		    stopMsg->setByteLength(1);
		    stopMsg->setClientIsOff(false);
		    stopMsg->setClientAddr(addr.str().c_str());
		    stopMsg->setServiceport(activeHTTPClients->getPortForAddress(addr));
		    //stopMsg->setDelay(10);

		    nsock.send(stopMsg);

		    //activeHTTPClients->removeVM(addr);
		}
	}
}

void SystemController::updateFTPClients(int numClientsToChange)
{
	if (numClientsToChange > 0) {
		// Starting clients.
		//ServiceMap *deactivatedClients = ftpClients->getVMsNotInOtherMap(activeFTPClients);
        std::auto_ptr<ServiceMap> deactivatedClients(ftpClients->getVMsNotInOtherMap(activeFTPClients));
		for (int i = 0; i < numClientsToChange; i++) {
			// Create socket to client.
			TCPSocket nsock;
			nsock.setOutputGate(gate("tcpOut"));

		    const char *serverThreadClass = par("serverThreadClass");
		    DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
		    nsock.setCallbackObject(proc);
		    proc->init(this, &nsock);
		    socketMap.addSocket(&nsock);

		    nsock.bind(ports->front());
		    ports->pop_front();

		    IPvXAddress addr = deactivatedClients->getIPAddress(i);
		    nsock.connect(addr, deactivatedClients->getPortForAddress(addr));

		    StartClient *startMsg = new StartClient("Starting FTPClient");
		    startMsg->setByteLength(1);
		    startMsg->setClientIsOn(false);
		    startMsg->setClientAddr(addr.str().c_str());
		    startMsg->setServiceport(deactivatedClients->getPortForAddress(addr));
		    //startMsg->setDelay(10);

		    nsock.send(startMsg);

		    //activeFTPClients->addVM(addr, deactivatedClients->getPortForAddress(addr));
		}
		//delete deactivatedClients;
	} else {
		// Stopping clients.
		numClientsToChange = abs(numClientsToChange);
		for (int i = 0; i < numClientsToChange; i++) {
			// Create socket to client.
			TCPSocket nsock;
			nsock.setOutputGate(gate("tcpOut"));

		    const char *serverThreadClass = par("serverThreadClass");
		    DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));
		    nsock.setCallbackObject(proc);
		    proc->init(this, &nsock);
		    socketMap.addSocket(&nsock);

		    nsock.bind(ports->front());
		    ports->pop_front();

		    IPvXAddress addr = activeFTPClients->getIPAddress(i);
		    nsock.connect(addr, activeFTPClients->getPortForAddress(addr));

		    StopClient *stopMsg = new StopClient("Stopping FTPClient");
		    stopMsg->setByteLength(1);
		    stopMsg->setClientIsOff(false);
		    stopMsg->setClientAddr(addr.str().c_str());
		    stopMsg->setServiceport(activeFTPClients->getPortForAddress(addr));
		    //stopMsg->setDelay(10);

		    nsock.send(stopMsg);

		    //activeFTPClients->removeVM(addr);
		}
	}
}

void SystemController::finish()
{
	delete httpClients;
	delete ftpClients;
	delete activeHTTPClients;
	delete activeFTPClients;
	delete timePercentagePairs;
	//delete timerMsg;
}

void SystemController::updateActiveClients(IPvXAddress addr, int port) {
    DCTCPControllerApp *trafCon = check_and_cast<DCTCPControllerApp*>(findTrafficControllerModule());
    if (port == HTTPPORT) {
        activeHTTPClients->addVM(addr, port);
        trafCon->emitNumberOfActiveClientsForService(activeHTTPClients->getSize(), HTTPPORT);
    } else if (port == FTPPORT) {
        activeFTPClients->addVM(addr, port);
        trafCon->emitNumberOfActiveClientsForService(activeFTPClients->getSize(), FTPPORT);
    }
}

void SystemController::updateDeactivatedClients(IPvXAddress addr, int port) {
    DCTCPControllerApp *trafCon = check_and_cast<DCTCPControllerApp*>(findTrafficControllerModule());
    if (port == HTTPPORT) {
        activeHTTPClients->removeVM(addr);
        trafCon->emitNumberOfActiveClientsForService(activeHTTPClients->getSize(), HTTPPORT);
    } else if (port == FTPPORT) {
        activeFTPClients->removeVM(addr);
        trafCon->emitNumberOfActiveClientsForService(activeFTPClients->getSize(), FTPPORT);
    }
}
