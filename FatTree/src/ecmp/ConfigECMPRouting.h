//
// Author: Michael Klopf
// 

#ifndef __FATTREE_CONFIGECMPROUTING_H_
#define __FATTREE_CONFIGECMPROUTING_H_

#include <omnetpp.h>
#include <ecmpTopo.h>

/**
 * Module to fill the routing tables in the network.
 *
 * This is a special ECMP Routing version which finds all
 * multiple paths to a node and writes them into the routing table.
 */
class ConfigECMPRouting : public cSimpleModule
{
    protected:
        struct NodeInfo {
            NodeInfo() {usesDefaultRoute=false;}
            bool usesDefaultRoute;
        };
        typedef std::vector<NodeInfo> NodeInfoVector;
        bool isReRouting;
  protected:
    virtual int numInitStages() const  {return 3;}
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);

    /**
     * Adds routes to nodes with only one connection to the network.
     */
    virtual void addDefaultRoutes(ecmpTopo& topo, NodeInfoVector& nodeInfo);
    /**
     * Computes and fills the routing tables of all other nodes.
     */
    virtual void fillRoutingTables(ecmpTopo& topo, NodeInfoVector& nodeInfo);
    /**
     * Computes new routes and deletes old routes to virtual machines from routing tables of all other nodes.
     */
    virtual void fillRoutingTables(ecmpTopo& topo, NodeInfoVector& nodeInfo, const char * vm1address, const char * vm2address);

  public:
    ConfigECMPRouting() {
    	this->isReRouting = false;
    };
    /**
     * Manually start Routing Process
     */
    virtual void refreshRoutingTables(const char * vm1address, const char * vm2address);
};

#endif
