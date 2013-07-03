    // Implementation of DCTCPControllerApp

#include "DCTCPControllerApp.h"
#include "ReplyMsg_m.h"
#include <IPAddressResolver.h>
#include <IPvXAddress.h>
#include <IInterfaceTable.h>
#include "VMMigMsg_m.h"
#include "VirtualMachineApp.h"
#include "HyperVisorApp.h"
#include "SystemController.h"
#include "TTimerCon_m.h"

#define ONE 1

Define_Module(DCTCPControllerApp);

void DCTCPControllerApp::initialize(int stage)
{
	if (stage == 0) {
	    initializeAllSignals();
	    this->numFreeVMs = registerSignal("numFreeVMs");
	    this->jobQueueLength = registerSignal("jobQueueLength");
	    this->numAllVMs = registerSignal("numAllVMs");

	    this->numActiveWebVMs = registerSignal("numActiveWebVMs");
	    this->numActiveFTPVMs = registerSignal("numActiveFTPVMs");
	    this->numFreeWebVMs = registerSignal("numFreeWebVMs");
	    this->numFreeFTPVMs = registerSignal("numFreeFTPVMs");
	    //this->jobQLWeb = registerSignal("jobQueueWeb");
	    //this->jobQLFTP = registerSignal("jobQueueFTP");

	    //this->totalPowerConsumption = registerSignal("totalPowerConsumption");

	    this->timemovavgWeb = registerSignal("timemovavgWeb");
	    this->timemovavgFTP = registerSignal("timemovavgFTP");

	    this->movavgWeb = registerSignal("movavgWeb");
	    this->movavgFTP = registerSignal("movavgFTP");

	    this->doneVmMigrations = registerSignal("doneVmMigrations");
	    this->startFinishVMMig = registerSignal("startFinishVMMig");

	    this->numberOfActiveFtpClients = registerSignal("numberOfActiveFtpClients");
	    this->numberOfActiveWebClients = registerSignal("numberOfActiveWebClients");

	    this->countDeadZoneHits = registerSignal("countDeadZoneHits");

	    this->UPPERHTTPTH = par("UPPERHTTPTH");
	    this->LOWERHTTPTH = par("LOWERHTTPTH");
	    this->UPPERFTPTH = par("UPPERFTPTH");
	    this->LOWERFTPTH = par("LOWERFTPTH");
	    this->VMMIGTH = par("VMMIGTH");

	    this->HTTPPORT = par("HTTPPORT");
	    this->FTPPORT = par("FTPPORT");

        this->alpha = par("alpha");
        this->gamma = par("gamma");

        this->powerConsumption = 0;
        //this->numWebServers = 0;
        //this->numFtpServers = 0;
        this->numServers = 0;

        this->updateNumVMSwitch = par("updateNumVMs");
        this->migrateVMSwitch = par("migrateVMs");

        this->loadAdaptionInterval = par("loadAdaptionInterval");
        this->warmUpInterval = par("warmUpInterval");
        this->currentIntervalStart = 0;
        this->loadAdaptation = par("loadAdaptation");
        this->step = 0;
	}
	if (stage == 3) {
	    TCPSrvHostApp::initialize();
	    // Extract all virtual machine ip addresses and save them in data structure
	    cTopology topo("vms");
	    topo.extractByProperty("VM");

	    allVMs = new ServiceMap();
	    activeVMs = new ServiceMap();
	    freeVMs = new ServiceMap();
	    jobqueue = new JobQueue();
	    webJobQueue = new JobQueue();
	    ftpJobQueue = new JobQueue();
	    ports = new std::deque<int>();
	    servers = new ServerMap();
	    serverStates = new ServerStateMap();
	    deactivatingVMs = new TCPVMSet();
	    activatingVMs = new TCPVMSet();

	    for (int i = 0; i < 65535; i++) {
	    	ports->push_back(i);
	    }

	    // Filling lists where vms are involved.
	    for (int i = 0; i < topo.getNumNodes(); i++) {
	        IInterfaceTable *ift = IPAddressResolver().interfaceTableOf(topo.getNode(i)->getModule());
	        int addrType = IPAddressResolver::ADDR_IPv4;
	        IPvXAddress addr = IPAddressResolver().getAddressFrom(ift,addrType);
	        //delete ift;

	        /*
	         * Check the module for type tcpApp. It should hold a VMApp. We
	         * extract the vmON parameter and in the case the VM is on, we add
	         * it to the list of active VMs.
	         */
	        VirtualMachineApp *vmmod = check_and_cast<VirtualMachineApp*>(topo.getNode(i)->getModule()->getSubmodule("tcpApp", 0));

	        allVMs->addVM(addr, vmmod->getServicePort());
	        if (!vmmod) {
	        	opp_error("Could not find tcpApp");
	        }
	        VirtualMachineApp *vmapp = dynamic_cast<VirtualMachineApp*>(vmmod);

            /*
             * Check the parent module for including the hypervisor. We get his HVApp
             * and extract the hvON parameter. In case HV is on, we add the address
             * to the list of active VMs.
             */
            HyperVisorApp *hvapp = dynamic_cast<HyperVisorApp*>(topo.getNode(i)->getModule()->getParentModule()->getSubmodule("hypervisor",0)->getSubmodule("tcpApp",0));
            if (!hvapp) {
                opp_error("Could not find hypervisor module and HVApp.");
            }

            // Get HV ip address for filling ServerMap
            cModule *hv = topo.getNode(i)->getModule()->getParentModule()->getSubmodule("hypervisor",0);
            IPvXAddress hvaddr = IPAddressResolver().addressOf(hv, IPAddressResolver::ADDR_IPv4);
            servers->addVM(addr, hvaddr);

            // Fill data structure holding the states of the server.
            if (hvapp->getHVState()) {
                serverStates->addHV(hvaddr, true);
            } else {
                serverStates->addHV(hvaddr, false);
            }

	        if (vmapp->getVMState()) {
	        	if (hvapp->getHVState()) {
	                activeVMs->addVM(addr, vmmod->getServicePort());
	                freeVMs->addVM(addr, vmmod->getServicePort());

	                if (addr == IPvXAddress("20.0.0.1")) {
	                        int a = 0;
	                        a = a++;
	                }
	        	}
	        }
	    }

	    this->fsmCON = new cFSM();

	    this->fsmCON->setName("confsm");

	    // set first value for statistics
	    emitFreeVMsSignal(freeVMs->getSize());
	    emitFreeVMsSignal(freeVMs->getVMsWithPort(HTTPPORT)->getSize(), HTTPPORT);
	    emitFreeVMsSignal(freeVMs->getVMsWithPort(FTPPORT)->getSize(), FTPPORT);

	    emitActiveVMsState(activeVMs->getSize());
	    emitActiveVMsState(activeVMs->getVMsWithPort(HTTPPORT)->getSize(), HTTPPORT);
	    emitActiveVMsState(activeVMs->getVMsWithPort(FTPPORT)->getSize(), FTPPORT);

	    emitNumAllVMs(allVMs->getSize());

	    this->oldHttpReq = 0;
	    this->oldFtpReq = 0;

	    this->expWMAHTTP = 0; // starting with queue length 0 as first value
	    this->timeExpWMAHTTP = 0; // starting with queue length 0
	    this->sOfXTEWMAHTTP = 0; // according to paper starting with queue length 0
	    this->nOfXTEMWAHTTP = 1; // according to paper starting with 1 value recorded
	    this->oldTimeHTTP = 0;

	    this->expWMAFTP = 0; // starting with queue length 0 as first value
	    this->timeExpWMAFTP = 0; // starting with queue length 0
	    this->sOfXTEWMAFTP = 0; // according to paper starting with queue length 0
	    this->nOfXTEMWAFTP = 1; // according to paper starting with 1 value recorded
	    this->oldTimeFTP = 0;

	    this->migratingVM = false;
	    this->migratedVM = IPvXAddress();
	    this->migratedToVM = IPvXAddress();
	    this->migFromHV = IPvXAddress();
	    this->migratedToHV = IPvXAddress();

	    if (loadAdaptation) {
	        TTimerCon *newScheduleMsg = new TTimerCon("updatingLoadLevel");
	        scheduleAt(simTime()+simulation.getWarmupPeriod(), newScheduleMsg);
	    }
	}
}

void DCTCPControllerApp::handleMessage(cMessage *msg)
{
    EV << "Gotcha again: Here we look for the close.";
    if (msg->isSelfMessage())
    {
        TTimerCon *scheduler = dynamic_cast<TTimerCon*>(msg);
        if (scheduler) {
            step++;
            if (step == 1) {
                currentIntervalStart = simulation.getWarmupPeriod().dbl();
                // to print it the first time in statistic
                emitActiveServerSignal(0);
                emitActiveVMsState(activeVMs->getSize());
            } else {
                currentIntervalStart = currentIntervalStart+loadAdaptionInterval;
            }
            scheduleAt(simTime()+loadAdaptionInterval, scheduler);
        } else {
            DCTCPControllerThreadBase *thread = (DCTCPControllerThreadBase *)msg->getContextPointer();
            thread->timerExpired(msg);
        }
    }
    else
    {
        TCPSocket *socket;
        socket = extSocketMap.findSocketFor(msg);
        if (!socket)
        {
            // new connection -- create new socket object and server process
            socket = new TCPSocket(msg);
            socket->setOutputGate(gate("tcpOut"));

            const char *serverThreadClass = par("serverThreadClass");
            DCTCPControllerThreadBase *proc = check_and_cast<DCTCPControllerThreadBase *>(createOne(serverThreadClass));

            socket->setCallbackObject(proc);
            proc->init(this, socket);

            extSocketMap.addSocket(socket);

            updateDisplay();
        }
        //Processing Message
        socket->processMessage(msg);
    }
}

void DCTCPControllerApp::updateDisplay()
{
    if (!ev.isGUI()) return;

    char buf[32];
    sprintf(buf, "%d threads", extSocketMap.size());
    getDisplayString().setTagArg("t", 0, buf);
}

void DCTCPControllerApp::removeThread(DCTCPControllerThreadBase *thread)
{
    // remove socket
    extSocketMap.removeSocket(thread->getSocket());

    // remove thread object
    delete thread;

    updateDisplay();
}

void DCTCPControllerApp::finish()
{
	emitNumAllVMs(allVMs->getSize());
	emitActiveServerSignal(0);

    emitActiveVMsState(activeVMs->getVMsWithPort(HTTPPORT)->getSize(), HTTPPORT);
    emitFreeVMsSignal(freeVMs->getVMsWithPort(HTTPPORT)->getSize(), HTTPPORT);


    emitActiveVMsState(activeVMs->getVMsWithPort(FTPPORT)->getSize(), FTPPORT);
    emitFreeVMsSignal(freeVMs->getVMsWithPort(FTPPORT)->getSize(), FTPPORT);

    delete this->fsmCON;

    delete this->allVMs;

    delete this->activeVMs;

    delete this->freeVMs;

    delete this->servers;

    delete this->serverStates;

    delete this->deactivatingVMs;

    delete this->activatingVMs;

    delete this->jobqueue;

    delete this->webJobQueue;

    delete this->ftpJobQueue;
}

cModule *DCTCPControllerApp::findControllerModule()
{
	Enter_Method_Silent("We find the controller to send statistics.");
	cModule *debugmod = getParentModule()->  // get hypervisor or vm
            getParentModule()-> // get server
            getParentModule()-> // get rack
            getParentModule()-> // get pod
            getParentModule()-> // get FatTree
            getSubmodule("Controller",0)->
            getSubmodule("TrafficController", 0)->
            getSubmodule("tcpApp", 0);
	cModule *mod = getParentModule()->  // get hypervisor or vm
            getParentModule()-> // get server
            getParentModule()-> // get rack
            getParentModule()-> // get pod
            getParentModule()-> // get FatTree
            getSubmodule("Controller",0)-> // get controller
            getSubmodule("TrafficController", 0)-> // get traffic controller
            getSubmodule("tcpApp", 0); // get tcp app;
	return mod;
}

cModule *DCTCPControllerApp::findSystemControllerModule()
{
	//EV << "Being in module " << getParentModule()->getName() << ".\n";
	//EV << "Being in module " << getParentModule()->getParentModule()->getName() << ".\n";
	//EV << "Being in module " << getParentModule()->getParentModule()->getSubmodule("SystemController", 0)->getName() << ".\n";
	return getParentModule()->	// get trafficcontroller
			getParentModule()->  // get controller
			getSubmodule("SystemController", 0)->
			getSubmodule("tcpApp", 0);
}

void DCTCPControllerApp::emitActiveServerSignal(int state)
{
	Enter_Method_Silent("Updating statistic of # active servers.");
    double currentTime = simTime().dbl();
    int numOfStatistics = sizeof(activeServSigs)/sizeof(int);
    //numWebServers = numWebServers + state;
    //numFtpServers = numFtpServers + state;
    numServers = numServers + state;

    if (step == 0) {
        if (!loadAdaptation) {
            if (currentIntervalStart+warmUpInterval <= currentTime) {
                emit(activeServSigs[step], (long)numServers);
            }
        }
    } else if (currentIntervalStart+warmUpInterval <= currentTime && currentTime < currentIntervalStart+loadAdaptionInterval) {
        if (step-1 < numOfStatistics) {
            emit(activeServSigs[step-1], (long)numServers);
        }
    }
}

void DCTCPControllerApp::emitFreeVMsSignal(int state)
{
	emit(numFreeVMs, (long)state);
}

void DCTCPControllerApp::emitFreeVMsSignal(int state, int service)
{
	if (service == HTTPPORT) {
		emit(numFreeWebVMs, (long)state);
	} else if (service == FTPPORT) {
		emit(numFreeFTPVMs, (long)state);
	}
}

void DCTCPControllerApp::emitJobQueueLength(int size)
{
	emit(jobQueueLength, (long)size);
}

void DCTCPControllerApp::emitJobQueueLength(int size, int service)
{
    double currentTime = simTime().dbl();
    int numOfWebStatistics = sizeof(jobQWebSigs)/sizeof(int);
    int numOfFTPStatistics = sizeof(jobQFTPSigs)/sizeof(int);

	if (service == HTTPPORT) {
	    if (step == 0) {
	        if (!loadAdaptation) {
	            if (currentIntervalStart+warmUpInterval <= currentTime) {
	                emit(jobQWebSigs[step], (long)size);
	            }
	        }
	    } else if (currentIntervalStart+warmUpInterval <= currentTime && currentTime < currentIntervalStart+loadAdaptionInterval) {
            if (step-1 < numOfWebStatistics) {
                emit(jobQWebSigs[step-1], (long)size);
            }
	    }
	} else if (service == FTPPORT) {
	    if (step == 0) {
	        if (!loadAdaptation) {
	            if (currentIntervalStart+warmUpInterval <= currentTime) {
	                emit(jobQFTPSigs[step], (long)size);
	            }
	        }
	    } else if (currentIntervalStart+warmUpInterval <= currentTime && currentTime < currentIntervalStart+loadAdaptionInterval) {
            if (step-1 < numOfFTPStatistics) {
                emit(jobQFTPSigs[step-1], (long)size);
            }
        }
	}
}

void DCTCPControllerApp::emitActiveVMsState(int state)
{
    double currentTime = simTime().dbl();
    int numOfStatistics = sizeof(activeVMsSigs)/sizeof(int);

    if (step == 0) {
        if (!loadAdaptation) {
            if (currentIntervalStart+warmUpInterval <= currentTime) {
                emit(activeVMsSigs[step], (long)state);
            }
        }
    } else if (currentIntervalStart+warmUpInterval <= currentTime && currentTime < currentIntervalStart+loadAdaptionInterval) {
        if (step-1 < numOfStatistics) {
            emit(activeVMsSigs[step-1], (long)state);
        }
    }
}

void DCTCPControllerApp::emitActiveVMsState(int state, int service)
{
	if (service == HTTPPORT) {
		emit(numActiveWebVMs, (long)state);
	} else if (service == FTPPORT) {
		emit(numActiveFTPVMs, (long)state);
	}
}

void DCTCPControllerApp::emitNumAllVMs(int size)
{
	emit(numAllVMs, (long)size);
}

void DCTCPControllerApp::emitNumberOfBusyVMs(int size)
{
    double currentTime = simTime().dbl();
    int numOfStatistics = sizeof(busyVMsSigs)/sizeof(int);

    if (step == 0) {
        if (!loadAdaptation) {
            if (currentIntervalStart+warmUpInterval <= currentTime) {
                emit(busyVMsSigs[step], (long)size);
            }
        }
    } else if (currentIntervalStart+warmUpInterval <= currentTime && currentTime < currentIntervalStart+loadAdaptionInterval) {
        if (step-1 < numOfStatistics) {
            emit(busyVMsSigs[step-1], (long)size);
        }
    }
}

void DCTCPControllerApp::emitEnergyConsumption(int power)
{
    if (simTime() > simulation.getWarmupPeriod()) {
        int f = 1;
        f = 3;
    }
	Enter_Method_Silent("Updating energy consumption of data center.");
	double currentTime = simTime().dbl();
    //std::cerr << "DEBUG: Emitting power." << power << " in step " << step << " at " << currentTime << "\n";
	int numOfStatistics = sizeof(pcSigs)/sizeof(int);
	powerConsumption = powerConsumption + power;
	//std::cerr << "DEBUG: POWER CONS " << powerConsumption << "\n";

	if (step == 0) {
	    if (!loadAdaptation) {
	        if (currentIntervalStart+warmUpInterval <= currentTime) {
	            emit(pcSigs[step], (long)powerConsumption);
	        }
	    }
	} else if (currentIntervalStart+warmUpInterval <= currentTime && currentTime < currentIntervalStart+loadAdaptionInterval) {
        if (step-1 < numOfStatistics) {
            emit(pcSigs[step-1], (long)powerConsumption);
        }
    }
}

void DCTCPControllerApp::emitTimeMovingAverage(int value, int service)
{
	if (service == HTTPPORT) {
		emit(timemovavgWeb, (long)value);
	} else if (service == FTPPORT) {
		emit(timemovavgFTP, (long)value);
	}
}

void DCTCPControllerApp::emitMovingAverage(int value, int service)
{
	if (service == HTTPPORT) {
		emit(movavgWeb, (long)value);
	} else if (service == FTPPORT) {
		emit(movavgFTP, (long)value);
	}
}

void DCTCPControllerApp::emitDoneVMMigrations()
{
	emit(doneVmMigrations, (long)ONE);
}

void DCTCPControllerApp::emitStartEndVMMigration()
{
	emit(startFinishVMMig, (long)ONE);
}

void DCTCPControllerApp::emitNumberOfActiveClientsForService(int size, int port)
{
    if (port == HTTPPORT) {
        emit(numberOfActiveWebClients, size);
    } else if (port == FTPPORT) {
        emit(numberOfActiveFtpClients, size);
    }
}

void DCTCPControllerApp::emitProcessedJobRequest(int service)
{
    double currentTime = simTime().dbl();
    int webSigs = sizeof(processedWebSigs)/sizeof(int);
    int ftpSigs = sizeof(processedFTPSigs)/sizeof(int);

    if (service == HTTPPORT) {
        if (step == 0) {
            if (!loadAdaptation) {
                if (currentIntervalStart+warmUpInterval <= currentTime) {
                    emit(processedWebSigs[step], ONE);
                }
            }
        } else if (currentIntervalStart+warmUpInterval <= currentTime && currentTime < currentIntervalStart+loadAdaptionInterval) {
            if (step-1 < webSigs) {
                emit(processedWebSigs[step-1], ONE);
            }
        }
    } else if (service == FTPPORT) {
        if (step == 0) {
            if (!loadAdaptation) {
                if (currentIntervalStart+warmUpInterval <= currentTime) {
                    emit(processedFTPSigs[step], ONE);
                }
            }
        } else if (currentIntervalStart+warmUpInterval <= currentTime && currentTime < currentIntervalStart+loadAdaptionInterval) {
            if (step-1 < ftpSigs) {
                emit(processedFTPSigs[step-1], ONE);
            }
        }
    }
}

void DCTCPControllerApp::updateNumOfActiveVMsAndServers(int port)
{
    /*
    if (simTime() < simulation.getWarmupPeriod()) {
        return;
    }
    */

    if (simTime().dbl() > 700) {
        int a = 23434;
        a = a*a;
    }
	/*
	 * Get the numbers of messages in job queue, of a service.
	 * Compute the job queue length moving average with new length.
	 * Call method for handling next decisions.
	 */
	if (port == HTTPPORT) {
		// Takes mov avg of new queue length and uses this for on/off decision.
		computeTimeExpWeightedMovingAverageHTTP(webJobQueue->size());
        //activateOrDeactivateForHTTPService(webJobQueue->size());
		activateOrDeactivateForHTTPService(timeExpWMAHTTP);
		emitTimeMovingAverage(timeExpWMAHTTP, HTTPPORT);

		//computeExpWeightedMovingAverageHTTP(httpReq);
		//emitMovingAverage(expWMAHTTP, HTTPPORT);
		//activateOrDeactivateForHTTPService(expWMAHTTP);
	} else if (port == FTPPORT) {
		// Takes mov avg of new queue length and uses this for on/off decision.
		computeTimeExpWeightedMovingAverageFTP(ftpJobQueue->size());
		//activateOrDeactivateForFTPService(ftpJobQueue->size());
		activateOrDeactivateForFTPService(timeExpWMAFTP);
		emitTimeMovingAverage(timeExpWMAFTP, FTPPORT);

		//computeExpWeightedMovingAverageFTP(ftpReq);
		//emitMovingAverage(expWMAFTP, FTPPORT);
		//activateOrDeactivateForFTPService(expWMAFTP);
	}
}

void DCTCPControllerApp::activateOrDeactivateForHTTPService(int httpReq)
{
	if (LOWERHTTPTH < httpReq && httpReq < UPPERHTTPTH) {
		// do nothing
	} else if (httpReq >= UPPERHTTPTH) {
		activateVM(HTTPPORT);
	} else {
		deactivateVM(HTTPPORT);
	}

	if (migrateVMSwitch) {
	    if (VMMIGTH >= httpReq) {
	        initiateVMMigration(HTTPPORT);
	    }
	}
}

void DCTCPControllerApp::activateOrDeactivateForFTPService(int ftpReq)
{
    if (simTime() > simulation.getWarmupPeriod()) {
        if (40 < ftpReq && ftpReq < 80) {
            emit(countDeadZoneHits, (long)ONE);
            std::auto_ptr<ServiceMap> vmsWithService(freeVMs->getVMsWithPort(FTPPORT));
            int numVms = vmsWithService->getSize();
            std::cerr << "I found num of free vms " << numVms << "\n";
        }
    }
    /*
    if (step > 2) {
        if (40 < ftpReq && ftpReq < 80) {
            emit(countDeadZoneHits, (long)ONE);
        }
    }*/

	if (LOWERFTPTH < ftpReq && ftpReq < UPPERFTPTH) {
		// do nothing
	} else if (ftpReq >= UPPERFTPTH) {
		activateVM(FTPPORT);
	} else {
		deactivateVM(FTPPORT);
	}

	if (migrateVMSwitch) {
	    if (VMMIGTH >= ftpReq) {
	        initiateVMMigration(FTPPORT);
	    }
	}
}

void DCTCPControllerApp::activateVM(int service)
{
	IPvXAddress vmToStart;
	// find vm which is off to start up.
	if (activeVMs->getSize() == allVMs->getSize()) {
		return;
	} else {
		// Get all vms which are not active.
		//ServiceMap *inactiveVMs = allVMs->getVMsNotInOtherMap(activeVMs);
	    std::auto_ptr<ServiceMap> inactiveVMs(allVMs->getVMsNotInOtherMap(activeVMs));
		// Get all vms which support the service.
		//ServiceMap *vmsWithService = inactiveVMs->getVMsWithPort(service);
	    // all off vms for service
		std::auto_ptr<ServiceMap> vmsWithService(inactiveVMs->getVMsWithPort(service));
		int numVms = vmsWithService->getSize();
        TCPVMSet *vmsToStart = new TCPVMSet();

		if (numVms < 1) {
			return;
		} else {
		    //bool foundAVm = false;
		    for (int i = 0; i<numVms; i++) {
		        vmToStart = vmsWithService->getIPAddress(i);
		        // look if vm is already started
		        if (!activatingVMs->findIPAddress(vmToStart)) {
	                // prevent activating vms of hv2
	                if (servers->getServerAddr(vmToStart) != migratedToHV) { // vmToStart != migratedToVM
	                    // prevent activating vms of hv1
	                    if (servers->getServerAddr(vmToStart) != migFromHV) {
	                        vmsToStart->addVM(vmToStart);
	                        break;
	                    }
	                }
		        }
		    }
		}

        int numVmsToStart = vmsToStart->getSize();
        if (numVmsToStart == 0) {
            return;
        } else {
            int k = (rand() % numVmsToStart);
            vmToStart = vmsToStart->getIPAddress(k);
        }
        delete vmsToStart;
	}
    //std::cerr << "DEBUG: Choosing to activate." << vmToStart.str().c_str() << " at " << simTime().dbl() << "\n";

	// tell SysController to start up that VM, check if its server is on
	// and if necessary start up the server.
	check_and_cast<SystemController *>(findSystemControllerModule())->startUpVM(vmToStart, service);

	// changing lists initiated by syscon in updateListsVMStart.
	// add them to active VMs but not to free so they are not called to start up two times.
    if (simTime().dbl() > 4800) {
        int a = 23434;
        a = a*a;
    }
    if (vmToStart == IPvXAddress()) {
        return;
    }
	//activeVMs->addVM(vmToStart, service);
    activatingVMs->addVM(vmToStart);

    //emitActiveVMsState(activeVMs->getSize());
    //emitActiveVMsState(activeVMs->getVMsWithPort(service)->getSize(), service);
}

void DCTCPControllerApp::updateListsVMStart(IPvXAddress addr, int serviceport)
{
    if (simTime().dbl() > 4800) {
        int a = 23434;
        a = a*a;
    }
    if (addr == IPvXAddress("20.0.0.1")) {
            int a = 0;
            a = a++;
    }
	Enter_Method("Update lists, adding the started VM.");

	// Problem with emit here is: vms are added to active vms before start up to prevent turning them on twice. But they are
	// all added before emit can be called which results in emitting the same value multiple times.
    //std::cerr << "DEBUG: Num active Vms actv " << activeVMs->getSize() << "\n";
    activeVMs->addVM(addr, serviceport);
	emitActiveVMsState(activeVMs->getSize());
	emitActiveVMsState(activeVMs->getVMsWithPort(serviceport)->getSize(), serviceport);

	activatingVMs->removeVM(addr);
    //DEBUG
	/*
    std::string addrstr = addr.str();
    const char *addrchar = addrstr.c_str();
    std::cerr << "DEBUG: Updating activating List." << addrstr << " at " << simTime().dbl() << "\n";
    */
    //DEBUG

	freeVMs->addVM(addr, serviceport);
	emitFreeVMsSignal(freeVMs->getSize());
	emitFreeVMsSignal(freeVMs->getVMsWithPort(serviceport)->getSize(), serviceport);

	// in case job queues are not empty, schedule job.
	if (serviceport == HTTPPORT) {
	    if (webJobQueue->size() > 0) {
            RequestMsg *reqmsg = new RequestMsg(webJobQueue->front());
            webJobQueue->pop_front();
            scheduleAt(simTime(), reqmsg);

	    }
	} else if (serviceport == FTPPORT) {
	    if (ftpJobQueue->size() > 0) {
            RequestMsg *reqmsg = new RequestMsg(ftpJobQueue->front());
            ftpJobQueue->pop_front();
            scheduleAt(simTime(), reqmsg);
	    }
	}
}

void DCTCPControllerApp::deactivateVM(int service)
{
	// find an unused vm of that service.
	IPvXAddress vmToTurnOff;
	// get an unused VM and remove it from list.
	//ServiceMap *vmsWithService = freeVMs->getVMsWithPort(service);
	std::auto_ptr<ServiceMap> vmsWithService(freeVMs->getVMsWithPort(service));
	int numVms = vmsWithService->getSize();

	if (vmsWithService.get() == NULL || numVms <= 1) {
		return;
	} else {
	    TCPVMSet *vmsToStop = new TCPVMSet();
	    for (int i = 0; i< numVms; i++) {
	        vmToTurnOff = vmsWithService->getIPAddress(i);
	        if (servers->getServerAddr(vmToTurnOff) != migratedToHV) {
	            if (servers->getServerAddr(vmToTurnOff) != migFromHV) {
	                vmsToStop->addVM(vmToTurnOff);
	            }
	        }
	    }
	    int numVmsToStop = vmsToStop->getSize();
	    if (numVmsToStop == 0) {
	        return;
	    } else {
	        int k = (rand() % numVmsToStop);
	        vmToTurnOff = vmsToStop->getIPAddress(k);
	    }
	    delete vmsToStop;
	}


    if (vmToTurnOff == IPvXAddress("20.0.0.1")) {
            int a = 0;
            a = a++;
    }
	//delete vmsWithService;
	// add the vm to list of those deactivated
	deactivatingVMs->addVM(vmToTurnOff);
	// remove it from lists
	//activeVMs->removeVM(vmToTurnOff);
	//emitActiveVMsState(activeVMs->getSize());

	freeVMs->removeVM(vmToTurnOff); // prevents deactivating it again
	//emitFreeVMsSignal(freeVMs->getSize());

	//DEBUG
	/*
    std::string addrstr = vmToTurnOff.str();
    const char *addrchar = addrstr.c_str();
    */

    //std::cerr << "DEBUG: Choosing to deactivate." << vmToTurnOff.str().c_str() << " at " << simTime().dbl() << "\n";
    //DEBUG

	// if not, tell SysController to shut down that VM.
	check_and_cast<SystemController *>(findSystemControllerModule())->shutDownVM(vmToTurnOff, service);
}

void DCTCPControllerApp::updateListsVMShutDown(IPvXAddress addr, int serviceport)
{
    Enter_Method("Update lists, removing shut down VM.");

    if (simTime().dbl() > 4800) {
        int a = 23434;
        a = a*a;
    }
    /*
    if (addr == IPvXAddress("20.0.0.1")) {
            int a = 0;
            a = a++;
    }*/
    //DEBUG

    std::string addrstr = addr.str();
    const char *addrchar = addrstr.c_str();
    //std::cerr << "DEBUG: Updating deactvitated List." << addrstr << " at " << simTime().dbl() << "\n";

    //DEBUG
	activeVMs->removeVM(addr);
	deactivatingVMs->removeVM(addr);
	//std::cerr << "DEBUG: Num active Vms deact " << activeVMs->getSize() << "\n";
	emitActiveVMsState(activeVMs->getSize());
	emitActiveVMsState(activeVMs->getVMsWithPort(serviceport)->getSize(), serviceport);
	emitFreeVMsSignal(freeVMs->getSize());
	emitFreeVMsSignal(freeVMs->getVMsWithPort(serviceport)->getSize(), serviceport);
}

void DCTCPControllerApp::computeTimeExpWeightedMovingAverageHTTP(int newValue) //newJobQueueLength
{
	sOfXTEWMAHTTP = sOfXTEWMAHTTP * exp(-gamma * (simTime().dbl()-oldTimeHTTP.dbl())) + newValue;
	nOfXTEMWAHTTP = nOfXTEMWAHTTP * exp(-gamma * (simTime().dbl()-oldTimeHTTP.dbl())) + 1;

	oldTimeHTTP = simTime();
	timeExpWMAHTTP = sOfXTEWMAHTTP / nOfXTEMWAHTTP;
}

void DCTCPControllerApp::computeExpWeightedMovingAverageHTTP(int newValue)
{
	expWMAHTTP = alpha * newValue + (1-alpha) * expWMAHTTP;
}

void DCTCPControllerApp::computeTimeExpWeightedMovingAverageFTP(int newValue)
{
	sOfXTEWMAFTP = sOfXTEWMAFTP * exp(-gamma * (simTime().dbl()-oldTimeFTP.dbl())) + newValue;
	nOfXTEMWAFTP = nOfXTEMWAFTP * exp(-gamma * (simTime().dbl()-oldTimeFTP.dbl())) + 1;

	oldTimeFTP = simTime();
	timeExpWMAFTP = sOfXTEWMAFTP / nOfXTEMWAFTP;
}

void DCTCPControllerApp::computeExpWeightedMovingAverageFTP(int newValue)
{
	expWMAFTP = alpha * newValue + (1-alpha) * expWMAFTP;
}

void DCTCPControllerApp::initiateVMMigration(int serviceport)
{
	// When VM is migrated prevent new VM migration.
	if(migratingVM) {
		return;
	}
	migratingVM = true;	// Starting process.

	// first, find a VM which is currently in use.
	//ServiceMap *usedVMs = activeVMs->getVMsNotInOtherMap(freeVMs);
	std::auto_ptr<ServiceMap> usedVMs(activeVMs->getVMsNotInOtherMap(freeVMs));
	std::auto_ptr<ServiceMap> usedVMsForService(usedVMs->getVMsWithPort(serviceport));
	IPvXAddress usedVM = findAUsedVMFrom(usedVMsForService, serviceport);

	if (usedVM == IPvXAddress()) {
		migratingVM = false;
		return;	// could not find an used vm
	}

    //std::cerr << "Migrating VM " << usedVM << "\n";
	IPvXAddress hvUsedVM = servers->getServerAddr(usedVM);
	//std::cerr << "From HV " << hvUsedVM << "\n";
	// second, find an off vm to migrate to.
	IPvXAddress targetVM = findATargetVMtoMigrateTo(hvUsedVM, serviceport);
	//std::cerr << "To VM " << targetVM << "\n";
	if (targetVM == IPvXAddress()) {
		migratingVM = false;
		return; // could not find an unused vm
	}

    migratedVM = usedVM;
    migratedToVM = targetVM;
	IPvXAddress hvTargetVM = servers->getServerAddr(targetVM);
	migFromHV = hvUsedVM;
	migratedToHV = hvTargetVM;
	//std::cerr << "To HV " << hvTargetVM << "\n";
	IPvXAddress controllerAddr = IPAddressResolver().addressOf(getParentModule(), IPAddressResolver::ADDR_IPv4);

	VMMigMsg *vmmigmsg = new VMMigMsg("vmmigrationmsg");
	vmmigmsg->setByteLength(10);
	vmmigmsg->setHv1address(hvUsedVM.str().c_str());
	vmmigmsg->setVm1address(usedVM.str().c_str());
	vmmigmsg->setHv2address(hvTargetVM.str().c_str());
	vmmigmsg->setVm2address(targetVM.str().c_str());
	vmmigmsg->setConaddress(controllerAddr.str().c_str());

	vmmigmsg->setTurnOffHV1(true);	// currently no other peer is on

	if (serverStates->getStateFor(hvTargetVM)) {
		vmmigmsg->setTurnOnHV2(false);
	} else {
		vmmigmsg->setTurnOnHV2(true);
	}

	vmmigmsg->setMsgtype(INIT_MIG);
	vmmigmsg->setServiceport(serviceport);

	emitStartEndVMMigration();
	check_and_cast<SystemController *>(findSystemControllerModule())->startVMMigration(vmmigmsg);

	//delete usedVMs;
}

IPvXAddress DCTCPControllerApp::findAUsedVMFrom(std::auto_ptr<ServiceMap> usedVMs, int serviceport)
{
	if (usedVMs->getSize() == 0) {
		return IPvXAddress();
	}
	//ServiceMap *offVMs = (allVMs->getVMsNotInOtherMap(activeVMs))->getVMsWithPort(serviceport);
	std::auto_ptr<ServiceMap> offVMs((allVMs->getVMsNotInOtherMap(activeVMs))->getVMsWithPort(serviceport));
	IPvXAddress usedVM;
	bool foundUsedVm = false;
	// check for all vms if it is the only one on among a server.
	for (int i = 0; i < usedVMs->getSize(); i++) {
		usedVM = usedVMs->getIPAddress(i);
		if (deactivatingVMs->findIPAddress(usedVM)) {
			continue;
		}
		// get all peers of that vm and check if they are off.
		//TCPVMSet *peerVMs = servers->getPeerVMsOfGivenVM(usedVM);
		std::auto_ptr<TCPVMSet> peerVMs(servers->getPeerVMsOfGivenVM(usedVM));
		// we check for all peer vms if it is among the list of off vms.
		for (int j = 0; j < peerVMs->getSize(); j++) {
			// look if vm is not among the off vms.
			if (!offVMs->findIPAddress(peerVMs->getIPAddress(j))) {
				foundUsedVm = false;
				break;	// in case we find the vm on we continue with next used vm.
			} else {
				foundUsedVm = true;
			}
		}
		if (foundUsedVm) {
			break;
		}
	}
	//delete offVMs;
	if (foundUsedVm) {
		return usedVM;
	} else {
		return IPvXAddress();
	}
}

IPvXAddress DCTCPControllerApp::findATargetVMtoMigrateTo(IPvXAddress hvAddr, int serviceport)
{
	//ServiceMap *offVMs = (allVMs->getVMsNotInOtherMap(activeVMs))->getVMsWithPort(serviceport);
    std::auto_ptr<ServiceMap> offVMs((allVMs->getVMsNotInOtherMap(activeVMs))->getVMsWithPort(serviceport));
	IPvXAddress targetVM;
	bool foundTarget = false;
	// look through all off vms and find one not among the same hypervisor and not currently activated.
	for (int i = 0; i < offVMs->getSize(); i++) {
		targetVM = offVMs->getIPAddress(i);
		if (activatingVMs->findIPAddress(targetVM)) {
		    continue;
		}
		if (servers->getServerAddr(targetVM) != hvAddr) {
			foundTarget = true;
			break;
		}
	}
	// debug code
    if (targetVM == IPvXAddress("20.0.0.1")) {
            int a = 0;
            a = a++;
    }
	//delete offVMs;
	if (foundTarget) {
		return targetVM;
	} else {
		return IPvXAddress();
	}
}

void DCTCPControllerApp::finishedVMMigration(VMMigMsg *vmmigmsg)
{
    // TODO all lists are correct?
	delete vmmigmsg;
	migratingVM = false;

	//DEBUG
	IPvXAddress ser1 = servers->getServerAddr(migratedVM);
    std::string addrstr = ser1.str();
    const char *addrchar = addrstr.c_str();

    IPvXAddress ser2 = servers->getServerAddr(migratedToVM);
    std::string addrstr2 = ser2.str();
    const char *addrchar2 = addrstr2.c_str();
    //DEBUG

    migratedVM = IPvXAddress();
	migratedToVM = IPvXAddress();
	migFromHV = IPvXAddress();
	migratedToHV = IPvXAddress();
	emitDoneVMMigrations();
	emitStartEndVMMigration();
}

void DCTCPControllerApp::initializeAllSignals()
{
    jobQLWeb1 = registerSignal("jobQueueWeb1");
    jobQWebSigs[0] = jobQLWeb1;

    jobQLFTP1 = registerSignal("jobQueueFTP1");
    jobQFTPSigs[0] = jobQLFTP1;

    totalPowerConsumption1 = registerSignal("totalPowerConsumption1");
    pcSigs[0] = totalPowerConsumption1;

    activeServers1 = registerSignal("globalServerState1");
    activeServSigs[0] = activeServers1;

    processedWebJob1 = registerSignal("processedWebJob1");
    processedWebSigs[0] = processedWebJob1;
    processedFtpJob1 = registerSignal("processedFtpJob1");
    processedFTPSigs[0] = processedFtpJob1;

    numActiveVMs1 = registerSignal("numActiveVMs1");
    activeVMsSigs[0] = numActiveVMs1;

    numBusyVMs1 = registerSignal("numBusyVMs1");
    busyVMsSigs[0] = numBusyVMs1;


    jobQLWeb2 = registerSignal("jobQueueWeb2");
    jobQWebSigs[1] = jobQLWeb2;

    jobQLFTP2 = registerSignal("jobQueueFTP2");
    jobQFTPSigs[1] = jobQLFTP2;

    totalPowerConsumption2 = registerSignal("totalPowerConsumption2");
    pcSigs[1] = totalPowerConsumption2;

    activeServers2 = registerSignal("globalServerState2");
    activeServSigs[1] = activeServers2;

    processedWebJob2 = registerSignal("processedWebJob2");
    processedWebSigs[1] = processedWebJob2;
    processedFtpJob2 = registerSignal("processedFtpJob2");
    processedFTPSigs[1] = processedFtpJob2;

    numActiveVMs2 = registerSignal("numActiveVMs2");
    activeVMsSigs[1] = numActiveVMs2;

    numBusyVMs2 = registerSignal("numBusyVMs2");
    busyVMsSigs[1] = numBusyVMs2;


    jobQLWeb3 = registerSignal("jobQueueWeb3");
    jobQWebSigs[2] = jobQLWeb3;

    jobQLFTP3 = registerSignal("jobQueueFTP3");
    jobQFTPSigs[2] = jobQLFTP3;

    totalPowerConsumption3 = registerSignal("totalPowerConsumption3");
    pcSigs[2] = totalPowerConsumption3;

    activeServers3 = registerSignal("globalServerState3");
    activeServSigs[2] = activeServers3;

    processedWebJob3 = registerSignal("processedWebJob3");
    processedWebSigs[2] = processedWebJob3;
    processedFtpJob3 = registerSignal("processedFtpJob3");
    processedFTPSigs[2] = processedFtpJob3;

    numActiveVMs3 = registerSignal("numActiveVMs3");
    activeVMsSigs[2] = numActiveVMs3;

    numBusyVMs3 = registerSignal("numBusyVMs3");
    busyVMsSigs[2] = numBusyVMs3;


    jobQLWeb4 = registerSignal("jobQueueWeb4");
    jobQWebSigs[3] = jobQLWeb4;

    jobQLFTP4 = registerSignal("jobQueueFTP4");
    jobQFTPSigs[3] = jobQLFTP4;

    totalPowerConsumption4 = registerSignal("totalPowerConsumption4");
    pcSigs[3] = totalPowerConsumption4;

    activeServers4 = registerSignal("globalServerState4");
    activeServSigs[3] = activeServers4;

    processedWebJob4 = registerSignal("processedWebJob4");
    processedWebSigs[3] = processedWebJob4;
    processedFtpJob4 = registerSignal("processedFtpJob4");
    processedFTPSigs[3] = processedFtpJob4;

    numActiveVMs4 = registerSignal("numActiveVMs4");
    activeVMsSigs[3] = numActiveVMs4;

    numBusyVMs4 = registerSignal("numBusyVMs4");
    busyVMsSigs[3] = numBusyVMs4;


    jobQLWeb5 = registerSignal("jobQueueWeb5");
    jobQWebSigs[4] = jobQLWeb5;

    jobQLFTP5 = registerSignal("jobQueueFTP5");
    jobQFTPSigs[4] = jobQLFTP5;

    totalPowerConsumption5 = registerSignal("totalPowerConsumption5");
    pcSigs[4] = totalPowerConsumption5;

    activeServers5 = registerSignal("globalServerState5");
    activeServSigs[4] = activeServers5;

    processedWebJob5 = registerSignal("processedWebJob5");
    processedWebSigs[4] = processedWebJob5;
    processedFtpJob5 = registerSignal("processedFtpJob5");
    processedFTPSigs[4] = processedFtpJob5;

    numActiveVMs5 = registerSignal("numActiveVMs5");
    activeVMsSigs[4] = numActiveVMs5;

    numBusyVMs5 = registerSignal("numBusyVMs5");
    busyVMsSigs[4] = numBusyVMs5;


    jobQLWeb6 = registerSignal("jobQueueWeb6");
    jobQWebSigs[5] = jobQLWeb6;

    jobQLFTP6 = registerSignal("jobQueueFTP6");
    jobQFTPSigs[5] = jobQLFTP6;

    totalPowerConsumption6 = registerSignal("totalPowerConsumption6");
    pcSigs[5] = totalPowerConsumption6;

    activeServers6 = registerSignal("globalServerState6");
    activeServSigs[5] = activeServers6;

    processedWebJob6 = registerSignal("processedWebJob6");
    processedWebSigs[5] = processedWebJob6;
    processedFtpJob6 = registerSignal("processedFtpJob6");
    processedFTPSigs[5] = processedFtpJob6;

    numActiveVMs6 = registerSignal("numActiveVMs6");
    activeVMsSigs[5] = numActiveVMs6;

    numBusyVMs6 = registerSignal("numBusyVMs6");
    busyVMsSigs[5] = numBusyVMs6;


    jobQLWeb7 = registerSignal("jobQueueWeb7");
    jobQWebSigs[6] = jobQLWeb7;

    jobQLFTP7 = registerSignal("jobQueueFTP7");
    jobQFTPSigs[6] = jobQLFTP7;

    totalPowerConsumption7 = registerSignal("totalPowerConsumption7");
    pcSigs[6] = totalPowerConsumption7;

    activeServers7 = registerSignal("globalServerState7");
    activeServSigs[6] = activeServers7;

    processedWebJob7 = registerSignal( "processedWebJob7");
    processedWebSigs[6] = processedWebJob7;
    processedFtpJob7 = registerSignal("processedFtpJob7");
    processedFTPSigs[6] = processedFtpJob7;

    numActiveVMs7 = registerSignal("numActiveVMs7");
    activeVMsSigs[6] = numActiveVMs7;

    numBusyVMs7 = registerSignal("numBusyVMs7");
    busyVMsSigs[6] = numBusyVMs7;


    jobQLWeb8 = registerSignal("jobQueueWeb8");
    jobQWebSigs[7] = jobQLWeb8;

    jobQLFTP8 = registerSignal("jobQueueFTP8");
    jobQFTPSigs[7] = jobQLFTP8;

    totalPowerConsumption8 = registerSignal("totalPowerConsumption8");
    pcSigs[7] = totalPowerConsumption8;

    activeServers8 = registerSignal("globalServerState8");
    activeServSigs[7] = activeServers8;

    processedWebJob8 = registerSignal("processedWebJob8");
    processedWebSigs[7] = processedWebJob8;
    processedFtpJob8 = registerSignal("processedFtpJob8");
    processedFTPSigs[7] = processedFtpJob8;

    numActiveVMs8 = registerSignal("numActiveVMs8");
    activeVMsSigs[7] = numActiveVMs8;

    numBusyVMs8 = registerSignal("numBusyVMs8");
    busyVMsSigs[7] = numBusyVMs8;


    jobQLWeb9 = registerSignal("jobQueueWeb9");
    jobQWebSigs[8] = jobQLWeb9;

    jobQLFTP9 = registerSignal("jobQueueFTP9");
    jobQFTPSigs[8] = jobQLFTP9;

    totalPowerConsumption9 = registerSignal("totalPowerConsumption9");
    pcSigs[8] = totalPowerConsumption9;

    activeServers9 = registerSignal("globalServerState9");
    activeServSigs[8] = activeServers9;

    processedWebJob9 = registerSignal("processedWebJob9");
    processedWebSigs[8] = processedWebJob9;
    processedFtpJob9 = registerSignal("processedFtpJob9");
    processedFTPSigs[8] = processedFtpJob9;

    numActiveVMs9 = registerSignal("numActiveVMs9");
    activeVMsSigs[8] = numActiveVMs9;

    numBusyVMs9 = registerSignal("numBusyVMs9");
    busyVMsSigs[8] = numBusyVMs9;


    jobQLWeb10 = registerSignal("jobQueueWeb10");
    jobQWebSigs[9] = jobQLWeb10;

    jobQLFTP10 = registerSignal("jobQueueFTP10");
    jobQFTPSigs[9] = jobQLFTP10;

    totalPowerConsumption10 = registerSignal("totalPowerConsumption10");
    pcSigs[9] = totalPowerConsumption10;

    activeServers10 = registerSignal("globalServerState10");
    activeServSigs[9] = activeServers10;

    processedWebJob10 = registerSignal("processedWebJob10");
    processedWebSigs[9] = processedWebJob10;
    processedFtpJob10 = registerSignal("processedFtpJob10");
    processedFTPSigs[9] = processedFtpJob10;

    numActiveVMs10 = registerSignal("numActiveVMs10");
    activeVMsSigs[9] = numActiveVMs10;

    numBusyVMs10 = registerSignal("numBusyVMs10");
    busyVMsSigs[9] = numBusyVMs10;


    jobQLWeb11 = registerSignal("jobQueueWeb11");
    jobQWebSigs[10] = jobQLWeb11;

    jobQLFTP11 = registerSignal("jobQueueFTP11");
    jobQFTPSigs[10] = jobQLFTP11;

    totalPowerConsumption11 = registerSignal("totalPowerConsumption11");
    pcSigs[10] = totalPowerConsumption11;

    activeServers11 = registerSignal("globalServerState11");
    activeServSigs[10] = activeServers11;

    processedWebJob11 = registerSignal("processedWebJob11");
    processedWebSigs[10] = processedWebJob11;
    processedFtpJob11 = registerSignal("processedFtpJob11");
    processedFTPSigs[10] = processedFtpJob11;

    numActiveVMs11 = registerSignal("numActiveVMs11");
    activeVMsSigs[10] = numActiveVMs11;

    numBusyVMs11 = registerSignal("numBusyVMs11");
    busyVMsSigs[10] = numBusyVMs11;


    jobQLWeb12 = registerSignal("jobQueueWeb12");
    jobQWebSigs[11] = jobQLWeb12;

    jobQLFTP12 = registerSignal("jobQueueFTP12");
    jobQFTPSigs[11] = jobQLFTP12;

    totalPowerConsumption12 = registerSignal("totalPowerConsumption12");
    pcSigs[11] = totalPowerConsumption12;

    activeServers12 = registerSignal("globalServerState12");
    activeServSigs[11] = activeServers12;

    processedWebJob12 = registerSignal("processedWebJob12");
    processedWebSigs[11] = processedWebJob12;
    processedFtpJob12 = registerSignal("processedFtpJob12");
    processedFTPSigs[11] = processedFtpJob12;

    numActiveVMs12 = registerSignal("numActiveVMs12");
    activeVMsSigs[11] = numActiveVMs12;

    numBusyVMs12 = registerSignal("numBusyVMs12");
    busyVMsSigs[11] = numBusyVMs12;
}
