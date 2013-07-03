//
// App is used to support sending energy consumption values
// to statistics.
//
// Author: Michael Klopf
// 

#ifndef __FATTREE_AGGROUTERAPP_H_
#define __FATTREE_AGGROUTERAPP_H_

#include <omnetpp.h>

/**
 * Aggregation router TCP application.
 */
class AggRouterApp : public cSimpleModule
{
  protected:
    virtual int numInitStages() const  {return 3;}
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual cModule *findControllerModule();
};

#endif
