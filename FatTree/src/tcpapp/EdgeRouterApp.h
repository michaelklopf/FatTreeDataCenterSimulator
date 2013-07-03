//
// App is used to support sending energy consumption values
// to statistics.
//
// Author: Michael Klopf
// 

#ifndef __FATTREE_EDGEROUTERAPP_H_
#define __FATTREE_EDGEROUTERAPP_H_

#include <omnetpp.h>

/**
 * Edge router TCP application.
 */
class EdgeRouterApp : public cSimpleModule
{
  protected:
    virtual int numInitStages() const  {return 3;}
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual cModule *findControllerModule();
};

#endif
