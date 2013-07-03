//
// This TCP application provides the base for threads
// which handle incoming messages.
// Compared to TCPSrvHostApp, we use a different socketmap
// implementation allowing us finding certain
// TCPSockets easier.
// 
// Author: Michael Klopf
//

#ifndef __FATTREEVMMIGRATIONCASE1_DCTCPCONTROLLERAPP_H_
#define __FATTREEVMMIGRATIONCASE1_DCTCPCONTROLLERAPP_H_

#include <omnetpp.h>
#include <deque>
#include <TCPSrvHostApp.h>
#include "SocketMapExt.h"
#include "ServiceMap.h"
#include "RequestMsg_m.h"
#include "ServerMap.h"
#include "ServerStateMap.h"
#include "VMMigMsg_m.h"
#include "TCPVMSet.h"
#include <memory>

class DCTCPControllerThreadBase;

/**
 * Application is used for decapsulating incoming
 * ReplyMsg, taking the according socket and process
 * the message with this socket back to it's source.
 */
class DCTCPControllerApp : public TCPSrvHostApp
{
  protected:
    // Extended socket map, to find socket with connId.
    SocketMapExt extSocketMap;

    /*
     * all VMs that are turned on.
     */
    ServiceMap *activeVMs;
    /*
     * all VMs which are in the network.
     */
    ServiceMap *allVMs;
    /*
     * all VMs which are turned on, but are not processing a job request.
     */
    ServiceMap *freeVMs;

    /*
     * vm which is migrated during vm migration service.
     */
    IPvXAddress migratedVM;
    IPvXAddress migFromHV;

    /*
     * vm which is migrated to during vmmig service.
     */
    IPvXAddress migratedToVM;
    IPvXAddress migratedToHV;

    /*
     * Finite state machine which is used in case of virtual machine migration.
     */
    cFSM *fsmCON;

    /*
     * Queue, which holds all incoming job requests and processes them afterwards.
     */
    typedef std::deque<RequestMsg> JobQueue;
    JobQueue *jobqueue;

    JobQueue *webJobQueue;
    JobQueue *ftpJobQueue;

    /*
     * List of ports the controller app can use to build connections to vms.
     */
    std::deque<int> *ports;

    /*
     * List holding the vms and the according hypervisor.
     */
    ServerMap *servers;

    /*
     * List holding the hypervisors and their current on/off state.
     */
    ServerStateMap *serverStates;

    /*
     * Set of vms which are currently deactivated.
     */
    TCPVMSet *deactivatingVMs;

    /*
     * Set of vms which are currently activated.
     */
    TCPVMSet *activatingVMs;

    // Signals for statistic collection.
    simsignal_t numAllVMs;
    simsignal_t numFreeVMs;
    simsignal_t jobQueueLength;

    simsignal_t numActiveWebVMs;
    simsignal_t numActiveFTPVMs;

    simsignal_t numFreeWebVMs;
    simsignal_t numFreeFTPVMs;

    simsignal_t timemovavgWeb;
    simsignal_t timemovavgFTP;

    simsignal_t movavgWeb;
    simsignal_t movavgFTP;

    simsignal_t doneVmMigrations;
    simsignal_t startFinishVMMig;

    simsignal_t numberOfActiveWebClients;
    simsignal_t numberOfActiveFtpClients;

    // Thresholds for checking queue length.
    int UPPERHTTPTH;
    int LOWERHTTPTH;
    int UPPERFTPTH;
    int LOWERFTPTH;
    int VMMIGTH;

    // Number of requests while checking last time.
    int oldHttpReq;
    int oldFtpReq;

    // Moving Averages for comparing thresholds of HTTP service.
    double expWMAHTTP;	// EWMA
    double timeExpWMAHTTP;	// TEWMA
    double sOfXTEWMAHTTP;
    double nOfXTEMWAHTTP;
    // time of last jobqueue change.
    simtime_t oldTimeHTTP;

    // Moving Averages for comparing thresholds of HTTP service.
    double expWMAFTP;	// EWMA
    double timeExpWMAFTP;	// TEWMA
    double sOfXTEWMAFTP;
    double nOfXTEMWAFTP;
    // time of last jobqueue change.
    simtime_t oldTimeFTP;

    // present value for EWMA
    double alpha;
    // something like present value for TEWMA (?)
    double gamma;

    bool migratingVM;

    bool loadAdaptation;

    double loadAdaptionInterval;
    double warmUpInterval;

    double currentIntervalStart;

    // needed, because of warm-up period.
    int powerConsumption;
    //int numWebServers;
    //int numFtpServers;
    int numServers;

    bool updateNumVMSwitch;
    bool migrateVMSwitch;

    int step;

    simsignal_t jobQLWeb1;
    simsignal_t jobQLFTP1;
    simsignal_t totalPowerConsumption1;
    simsignal_t processedWebJob1;
    simsignal_t processedFtpJob1;
    simsignal_t activeServers1;
    simsignal_t numBusyVMs1;
    simsignal_t numActiveVMs1;

    simsignal_t jobQLWeb2;
    simsignal_t jobQLFTP2;
    simsignal_t totalPowerConsumption2;
    simsignal_t processedWebJob2;
    simsignal_t processedFtpJob2;
    simsignal_t activeServers2;
    simsignal_t numBusyVMs2;
    simsignal_t numActiveVMs2;

    simsignal_t jobQLWeb3;
    simsignal_t jobQLFTP3;
    simsignal_t totalPowerConsumption3;
    simsignal_t processedWebJob3;
    simsignal_t processedFtpJob3;
    simsignal_t activeServers3;
    simsignal_t numBusyVMs3;
    simsignal_t numActiveVMs3;

    simsignal_t jobQLWeb4;
    simsignal_t jobQLFTP4;
    simsignal_t totalPowerConsumption4;
    simsignal_t processedWebJob4;
    simsignal_t processedFtpJob4;
    simsignal_t activeServers4;
    simsignal_t numBusyVMs4;
    simsignal_t numActiveVMs4;

    simsignal_t jobQLWeb5;
    simsignal_t jobQLFTP5;
    simsignal_t totalPowerConsumption5;
    simsignal_t processedWebJob5;
    simsignal_t processedFtpJob5;
    simsignal_t activeServers5;
    simsignal_t numBusyVMs5;
    simsignal_t numActiveVMs5;

    simsignal_t jobQLWeb6;
    simsignal_t jobQLFTP6;
    simsignal_t totalPowerConsumption6;
    simsignal_t processedWebJob6;
    simsignal_t processedFtpJob6;
    simsignal_t activeServers6;
    simsignal_t numBusyVMs6;
    simsignal_t numActiveVMs6;

    simsignal_t jobQLWeb7;
    simsignal_t jobQLFTP7;
    simsignal_t totalPowerConsumption7;
    simsignal_t processedWebJob7;
    simsignal_t processedFtpJob7;
    simsignal_t activeServers7;
    simsignal_t numBusyVMs7;
    simsignal_t numActiveVMs7;

    simsignal_t jobQLWeb8;
    simsignal_t jobQLFTP8;
    simsignal_t totalPowerConsumption8;
    simsignal_t processedWebJob8;
    simsignal_t processedFtpJob8;
    simsignal_t activeServers8;
    simsignal_t numBusyVMs8;
    simsignal_t numActiveVMs8;

    simsignal_t jobQLWeb9;
    simsignal_t jobQLFTP9;
    simsignal_t totalPowerConsumption9;
    simsignal_t processedWebJob9;
    simsignal_t processedFtpJob9;
    simsignal_t activeServers9;
    simsignal_t numBusyVMs9;
    simsignal_t numActiveVMs9;

    simsignal_t jobQLWeb10;
    simsignal_t jobQLFTP10;
    simsignal_t totalPowerConsumption10;
    simsignal_t processedWebJob10;
    simsignal_t processedFtpJob10;
    simsignal_t activeServers10;
    simsignal_t numBusyVMs10;
    simsignal_t numActiveVMs10;

    simsignal_t jobQLWeb11;
    simsignal_t jobQLFTP11;
    simsignal_t totalPowerConsumption11;
    simsignal_t processedWebJob11;
    simsignal_t processedFtpJob11;
    simsignal_t activeServers11;
    simsignal_t numBusyVMs11;
    simsignal_t numActiveVMs11;

    simsignal_t jobQLWeb12;
    simsignal_t jobQLFTP12;
    simsignal_t totalPowerConsumption12;
    simsignal_t processedWebJob12;
    simsignal_t processedFtpJob12;
    simsignal_t activeServers12;
    simsignal_t numBusyVMs12;
    simsignal_t numActiveVMs12;

    simsignal_t jobQWebSigs[12];
    simsignal_t jobQFTPSigs[12];
    simsignal_t pcSigs[12];
    simsignal_t activeServSigs[12];
    simsignal_t processedWebSigs[12];
    simsignal_t processedFTPSigs[12];
    simsignal_t busyVMsSigs[12];
    simsignal_t activeVMsSigs[12];

    simsignal_t countDeadZoneHits;

  public:
    // Ports for different services.
    int HTTPPORT;
    int FTPPORT;
  protected:
    virtual int numInitStages() const  {return 4;}
    virtual void initialize(int stage);
    /*
     * Looks for a TCPSocket connection in a SocketMap,
     * when no socket is found, then create a new one.
     * Then process message to thread.
     */
    virtual void handleMessage(cMessage *msg);

    virtual void updateDisplay();

    virtual void finish();

    // VM activation/deactivation functions.

    virtual void activateVM(int serviceport);

    virtual void deactivateVM(int serviceport);

    virtual void activateOrDeactivateForHTTPService(int httpRequests);

    virtual void activateOrDeactivateForFTPService(int ftpRequests);

    // Compute the averages.

    virtual void computeTimeExpWeightedMovingAverageHTTP(int newValue);

    virtual void computeExpWeightedMovingAverageHTTP(int newValue);

    virtual void computeTimeExpWeightedMovingAverageFTP(int newValue);

    virtual void computeExpWeightedMovingAverageFTP(int newValue);

    /*
     * Method to start a vm migration process, which results in a vmmigmsg
     * sent to module itself and triggering the finite state machine.
     */
    virtual void initiateVMMigration(int serviceport);

    virtual IPvXAddress findAUsedVMFrom(std::auto_ptr<ServiceMap> usedVMs, int serviceport);

    virtual IPvXAddress findATargetVMtoMigrateTo(IPvXAddress hvAddr, int serviceport);

    virtual void initializeAllSignals();

  public:
    /*
     * Get socket map containing the sockets.
     */
    virtual SocketMapExt getSocketMap() {return this->extSocketMap;};
    virtual SocketMapExt *getSocketMaps() {return &this->extSocketMap;};
    /*
     * Get set for active (on) virtual machines.
     */
    virtual ServiceMap *getActiveVMs() {return this->activeVMs;};
    /*
     * Get set for all virtual machines.
     */
    virtual ServiceMap *getAllVMs() {return this->allVMs;};
    /*
     * Get set for all vms not in use.
     */
    virtual ServiceMap *getFreeVMs() {return this->freeVMs;};
    /*
     * Get finite state machine of controller.
     */
    virtual cFSM *getFSMCON() {return this->fsmCON;};

    virtual ServerMap *getServers() {return this->servers;}

    virtual ServerStateMap *getServerStates() {return this->serverStates;}

    virtual TCPVMSet *getActivatingVMs() {return this->activatingVMs;}

    virtual void removeThread(DCTCPControllerThreadBase *thread);

    JobQueue *getJobQueue() const {return jobqueue;}

    JobQueue *getWebJobQueue() const {return webJobQueue;}

    JobQueue *getFTPJobQueue() const {return ftpJobQueue;}

    std::deque<int> *getPortList() const {return ports;}

    virtual IPvXAddress getMigratedVM() {return this->migratedVM;};

    virtual void setMigratedVM(IPvXAddress migratedVM) {this->migratedVM = migratedVM;};

    /*
     * Must not be used from this class. Only for hypervisor and vms.
     */
    virtual cModule *findControllerModule();

    virtual cModule *findSystemControllerModule();

    // Signal updating functions

    // Emit number of active servers/hypervisors. Called by hv module itself.
    virtual void emitActiveServerSignal(int state);

    // Emit number of free vms in system.
    virtual void emitFreeVMsSignal(int state);

    virtual void emitFreeVMsSignal(int state, int service);

    // Emit size of job queue.
    virtual void emitJobQueueLength(int size);

    virtual void emitJobQueueLength(int size, int service);

    // Emit number of vms which are on.
    virtual void emitActiveVMsState(int state);

    virtual void emitActiveVMsState(int state, int service);

    // Emit number of all vms.
    virtual void emitNumAllVMs(int size);

    // Emit time based moving average of job queue length
    virtual void emitTimeMovingAverage(int value, int service);

    // Emit not time based mov avg of jq length
    virtual void emitMovingAverage(int value, int service);

    // Emit the number of VMs which are active and processing a job.
    virtual void emitNumberOfBusyVMs(int size);

    /*
     * Emit energy consumption. Updated when vm/hypervisor/etc.
     * is active/idle/off/on. Power is therefore the power the
     * device is consuming in each state, or not consuming anymore,
     * if it consumes less power. power = 10 Watt or power = -10 Watt.
     *
     */
    virtual void emitEnergyConsumption(int power);

    /*
     * Emit done VM Migrations. Add 1 for each done Migration.
     */
    virtual void emitDoneVMMigrations();

    /*
     * Emit the start and end of a VM Migration.
     */
    virtual void emitStartEndVMMigration();

    /*
     * Emit number of active clients.
     */
    virtual void emitNumberOfActiveClientsForService(int size, int port);

    /*
     * Emit processed job request.
     */
    virtual void emitProcessedJobRequest(int port);

    // Functions for checking Thresholds of JobQueue

    /*
     * Called by methods which change the length of the job queue.
     */
    virtual void updateNumOfActiveVMsAndServers(int port);

    /*
     * Called by SystemController.
     */
    virtual void updateListsVMStart(IPvXAddress addr, int serviceport);

    /*
     * Called by SystemController.
     */
    virtual void updateListsVMShutDown(IPvXAddress addr, int serviceport);

    /*
     * Called by SystemController.
     */
    virtual void finishedVMMigration(VMMigMsg *vmmigmsg);

    virtual bool isUpdateNumVMS() {return updateNumVMSwitch;}
};

/**
 * TCP thread for controller to relay messages to the right host. Needs
 * to be extended by subclass.
 */
class DCTCPControllerThreadBase: public cPolymorphic, public TCPSocket::CallbackInterface {
    protected:
        DCTCPControllerApp *hostmod;
        TCPSocket *sock;

    protected:
      // internal: TCPSocket::CallbackInterface methods
      virtual void socketDataArrived(int, void *, cPacket *msg, bool urgent) {dataArrived(msg,urgent);}
      virtual void socketEstablished(int, void *)  {established();}
      virtual void socketPeerClosed(int, void *) {peerClosed();}
      virtual void socketClosed(int, void *) {closed();}
      virtual void socketFailure(int, void *, int code) {failure(code);}
      virtual void socketStatusArrived(int, void *, TCPStatusInfo *status) {statusArrived(status);}

    public:
      virtual void init(DCTCPControllerApp *hostmodule, TCPSocket *socket) {hostmod=hostmodule; sock=socket;};

    public:
        DCTCPControllerThreadBase()  {sock=NULL;}
        virtual ~DCTCPControllerThreadBase() {}

        virtual void established() = 0;
        virtual void dataArrived(cMessage *msg, bool urgent) = 0;
        virtual void timerExpired(cMessage *timer) = 0;

        virtual TCPSocket *getSocket() {return sock;}

        virtual DCTCPControllerApp *getHostModule() {return hostmod;} ;

        virtual void scheduleAt(simtime_t t, cMessage *msg)  {
            RequestMsg *reqmsg = dynamic_cast<RequestMsg *>(msg);
            if (reqmsg) {
                hostmod->scheduleAt(t,msg);
            } else {
                msg->setContextPointer(this);
                hostmod->scheduleAt(t,msg);
            }
        }

        virtual void cancelEvent(cMessage *msg)  {hostmod->cancelEvent(msg);}

        virtual void peerClosed() {getSocket()->close();}

        virtual void closed() {hostmod->removeThread(this);}

        virtual void failure(int code) {hostmod->removeThread(this);}

        virtual void statusArrived(TCPStatusInfo *status) {delete status;}

};

#endif
