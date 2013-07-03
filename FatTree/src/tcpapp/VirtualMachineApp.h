    //
// Author: Michael Klopf
// 

#ifndef __FATTREEVMMIGRATIONCASE1_VIRTUALMACHINEAPP_H_
#define __FATTREEVMMIGRATIONCASE1_VIRTUALMACHINEAPP_H_

#include <omnetpp.h>
#include "DCTCPControllerApp.h"
#include "VMMigMsg_m.h"

/**
 * Class for adding capabilities for virtual machine migration
 * to virtual machines.
 */
class VirtualMachineApp : public DCTCPControllerApp
{
  protected:
    bool vmON;
    /*
     * Flag to show that vm is starting, so server won't shut down.
     */
    bool startUpRequested;

    cFSM *fsmVM;
    simsignal_t vmOnSignal;
    simsignal_t busyState;
    simsignal_t idleState;
    simsignal_t offState;
    simsignal_t startUpState;
    simsignal_t shutDownState;

    bool reqThreadClosed;
    bool vmMigrationRequest;
    DCTCPControllerApp *controller;
    enum {
       INIT = 0,
       VMOFF = FSM_Steady(1),
       IDLE = FSM_Steady(2),
       BUSY = FSM_Steady(3)};
    //POWERINGON = FSM_Steady(4),
    //POWERINGDOWN = FSM_Steady(5),
    int HTTPPORT;
    int FTPPORT;
    TCPSocket *hvSocket;
protected:
    virtual int numInitStages() const
    {
        return 3;
    }

    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    virtual void setServiceLogo();
public:
    virtual void setVMstate(bool state)
    {
        this->vmON = state;
    };

    virtual bool getVMState()
    {
        return this->vmON;
    };

    virtual cFSM *getFSMVM()
    {
        return this->fsmVM;
    };

    virtual void emitSignal();
    virtual void emitVMBusy();
    virtual void emitPoweringOn();
    virtual void emitPoweringOff();

    bool isReqThreadClosed() const
    {
        return reqThreadClosed;
    }

    void setReqThreadClosed(bool reqThreadClosed)
    {
        this->reqThreadClosed = reqThreadClosed;
    }

    bool getVMMigRequest() const
    {
        return this->vmMigrationRequest;
    }

    void setVMMigRequest(bool vmMigrationRequest)
    {
        this->vmMigrationRequest = vmMigrationRequest;
    }

    bool getStartUpRequested()
    {
        return this->startUpRequested;
    }

    void setStartUpRequested(bool isrequested)
    {
        this->startUpRequested = isrequested;
    }

    int getServicePort()
    {
        return this->par("port");
    }

    void setServicePort(int serviceport)
    {
        this->par("port") = serviceport;
    }

    void startUpVM();
    void shutDownVM();

    TCPSocket *getHvSocket() const
    {
        return hvSocket;
    }

    virtual void updateHVSocket();
};

#endif
