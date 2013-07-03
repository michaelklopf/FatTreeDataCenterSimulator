//
// Author: Michael Klopf
//

#ifndef __FATTREEVMMIGRATIONCASE1_HYPERVISORAPP_H_
#define __FATTREEVMMIGRATIONCASE1_HYPERVISORAPP_H_

#include <omnetpp.h>
#include "DCTCPControllerApp.h"

/**
 * Class for adding capabilities for virtual machine migration
 * to hyper visors.
 */
class HyperVisorApp : public DCTCPControllerApp
{
  protected:
	/*
	 * Flag to represent on/off state of server.
	 */
    bool hvON;
    /*
     * Set flag to show that starting up, preventing VMs of sending another
     * startupmsg.
     */
    bool startUpIni;
    /*
     * Set flag to prevent other VMs shutting down server.
     */
    bool shutDownIni;
    simtime_t startTime;
    cFSM *fsmHV;
	simsignal_t hvOnSignal;
    simsignal_t busyState;
    simsignal_t idleState;
    simsignal_t offState;
    simsignal_t startUpState;
    simsignal_t shutDownState;

	DCTCPControllerApp *controller;
    enum {
       INIT = 0,
       HVOFF = FSM_Steady(1),
       ACTIVE = FSM_Steady(2),
       COPYING = FSM_Steady(3),
       //POWERINGON = FSM_Steady(4),
       //POWERINGDOWN = FSM_Steady(5),
    };

  protected:
    virtual int numInitStages() const  {return 3;}
    virtual void initialize(int stage);

    virtual void finish();

  public:
    virtual void setHVState(bool state) {this->hvON = state;};
    virtual bool getHVState() {return this->hvON;};

    virtual cFSM *getFSMHV() {return this->fsmHV;};

    virtual void emitSignal();
    //virtual void emitPoweringOnAndOff(bool processing);
    virtual void emitPoweringOn();
    virtual void emitPoweringOff();
    virtual void emitHVBusy(bool isBusy);

    virtual bool areAllVMsOff();

    virtual void setStartUpIni(bool state) {this->startUpIni = state;}
    virtual bool getStartUpIni() {return this->startUpIni;}

    virtual void setShutDownIni(bool state) {this->shutDownIni = state;}
    virtual bool getShutDownIni() {return this->shutDownIni;}

    virtual void setStartTime(simtime_t time) {this->startTime = time;}
    virtual simtime_t getStartTime() {return this->startTime;}

    virtual void startUpServer();
    virtual void shutDownServer();
};

#endif
