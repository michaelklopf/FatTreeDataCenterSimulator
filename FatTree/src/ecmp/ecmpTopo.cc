//
// Author: Michael Klopf
//

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <deque>
#include <algorithm>
#include <sstream>
#include "ecmpTopo.h"
#include "cpar.h"
#include "globals.h"
#include "cexception.h"
#include "cproperty.h"
#include <IPAddressResolver.h>
#include <IInterfaceTable.h>
//#include "patternmatcher.h"
#define LINKWEIGHT 1

#ifdef WITH_PARSIM
#include "ccommbuffer.h"
#endif

USING_NAMESPACE

Register_Class(ecmpTopo);


ecmpTopo::ECMPLinkIn *ecmpTopo::ECMPNode::getECMPLinkIn(int i)
{
    if (i<0 || i>=num_in_ECMPLinks)
        throw cRuntimeError("ecmpTopo::ECMPNode::getECMPLinkIn: invalid ECMPLink index %d", i);
    return (ecmpTopo::ECMPLinkIn *)in_ECMPLinks[i];
}

ecmpTopo::ECMPLinkOut *ecmpTopo::ECMPNode::getECMPLinkOut(int i)
{
    if (i<0 || i>=num_out_ECMPLinks)
        throw cRuntimeError("ecmpTopo::ECMPNode::getECMPLinkOut: invalid index %d", i);
    return (ecmpTopo::ECMPLinkOut *)(out_ECMPLinks+i);
}

//----

ecmpTopo::ecmpTopo(const char *name) : cOwnedObject(name)
{
    num_ECMPNodes = 0;
    ECMPNodev = NULL;
}

ecmpTopo::ecmpTopo(const ecmpTopo& topo) : cOwnedObject(topo)
{
    throw cRuntimeError(this,"copy ctor not implemented yet");
}

ecmpTopo::~ecmpTopo()
{
    clear();
}

std::string ecmpTopo::info() const
{
    std::stringstream out;
    out << "n=" << num_ECMPNodes;
    return out.str();
}

void ecmpTopo::parsimPack(cCommBuffer *buffer)
{
    throw cRuntimeError(this,"parsimPack() not implemented");
}

void ecmpTopo::parsimUnpack(cCommBuffer *buffer)
{
    throw cRuntimeError(this,"parsimUnpack() not implemented");
}

ecmpTopo& ecmpTopo::operator=(const ecmpTopo&)
{
    throw cRuntimeError(this,"operator= not implemented yet");
}

void ecmpTopo::clear()
{
    for (int i=0; i<num_ECMPNodes; i++)
    {
        delete [] ECMPNodev[i].in_ECMPLinks;
        delete [] ECMPNodev[i].out_ECMPLinks;
    }
    delete [] ECMPNodev;

    num_ECMPNodes = 0;
    ECMPNodev = NULL;
}

//---
/*
static bool selectByModulePath(cModule *mod, void *data)
{
    // actually, this is selectByModuleFullPathPattern()
    const std::vector<std::string>& v = *(const std::vector<std::string> *)data;
    std::string path = mod->getFullPath();
    for (int i=0; i<(int)v.size(); i++)
        if (PatternMatcher(v[i].c_str(), true, true, true).matches(path.c_str()))
            return true;
    return false;
}
*/
static bool selectByNedTypeName(cModule *mod, void *data)
{
    const std::vector<std::string>& v = *(const std::vector<std::string> *)data;
    return std::find(v.begin(), v.end(), mod->getNedTypeName()) != v.end();
}

static bool selectByProperty(cModule *mod, void *data)
{
    struct ParamData {const char *name; const char *value;};
    ParamData *d = (ParamData *)data;
    cProperty *prop = mod->getProperties()->get(d->name);
    if (!prop)
        return false;
    const char *value = prop->getValue(cProperty::DEFAULTKEY, 0);
    if (d->value)
        return opp_strcmp(value, d->value)==0;
    else
        return opp_strcmp(value, "false")!=0;
}

static bool selectByParameter(cModule *mod, void *data)
{
    struct PropertyData{const char *name; const char *value;};
    PropertyData *d = (PropertyData *)data;
    return mod->hasPar(d->name) && (d->value==NULL || mod->par(d->name).str()==std::string(d->value));
}

//---
/*
void ecmpTopo::extractByModulePath(const std::vector<std::string>& fullPathPatterns)
{
    extractFromNetwork(selectByModulePath, (void *)&fullPathPatterns);
}
*/
void ecmpTopo::extractByNedTypeName(const std::vector<std::string>& nedTypeNames)
{
    extractFromNetwork(selectByNedTypeName, (void *)&nedTypeNames);
}

void ecmpTopo::extractByProperty(const char *propertyName, const char *value)
{
    struct {const char *name; const char *value;} data = {propertyName, value};
    extractFromNetwork(selectByProperty, (void *)&data);
}

void ecmpTopo::extractByParameter(const char *paramName, const char *paramValue)
{
    struct {const char *name; const char *value;} data = {paramName, paramValue};
    extractFromNetwork(selectByParameter, (void *)&data);
}

//---

static bool selectByPredicate(cModule *mod, void *data)
{
    ecmpTopo::Predicate *predicate = (ecmpTopo::Predicate *)data;
    return predicate->matches(mod);
}

void ecmpTopo::extractFromNetwork(Predicate *predicate)
{
    extractFromNetwork(selectByPredicate, (void *)predicate);
}

void ecmpTopo::extractFromNetwork(bool (*selfunc)(cModule *,void *), void *data)
{
    clear();

    ECMPNode *temp_ECMPNodev = new ECMPNode[simulation.getLastModuleId()];

    // Loop through all modules and find those which have the required
    // parameter with the (optionally) required value.
    int k=0;
    for (int mod_id=0; mod_id<=simulation.getLastModuleId(); mod_id++)
    {
        cModule *mod = simulation.getModule(mod_id);
        if (mod && selfunc(mod,data))
        {
            // ith module is OK, insert into ECMPNodev[]
            temp_ECMPNodev[k].module_id = mod_id;
            temp_ECMPNodev[k].wgt = 0;
            temp_ECMPNodev[k].enabl = true;

            // init auxiliary variables
            temp_ECMPNodev[k].known = 0;
            temp_ECMPNodev[k].dist = INFINITY;
            temp_ECMPNodev[k].out_path = NULL;

            // create in_ECMPLinks[] arrays (big enough...)
            temp_ECMPNodev[k].num_in_ECMPLinks = 0;
            temp_ECMPNodev[k].in_ECMPLinks = new ecmpTopo::ECMPLink *[mod->gateCount()];

            k++;
        }
    }
    num_ECMPNodes = k;

    ECMPNodev = new ECMPNode[num_ECMPNodes];
    memcpy(ECMPNodev, temp_ECMPNodev, num_ECMPNodes*sizeof(ECMPNode));
    delete [] temp_ECMPNodev;

    // Discover out neighbors too.
    for (int k=0; k<num_ECMPNodes; k++)
    {
        // Loop through all its gates and find those which come
        // from or go to modules included in the topology.

        cModule *mod = simulation.getModule(ECMPNodev[k].module_id);
        ecmpTopo::ECMPLink *temp_out_ECMPLinks = new ecmpTopo::ECMPLink[mod->gateCount()];

        int n_out=0;
        for (cModule::GateIterator i(mod); !i.end(); i++)
        {
            cGate *gate = i();
            if (gate->getType()!=cGate::OUTPUT)
                continue;

            // follow path
            cGate *src_gate = gate;
            do {
                gate = gate->getNextGate();
            }
            while(gate && !selfunc(gate->getOwnerModule(),data));

            // if we arrived at a module in the topology, record it.
            if (gate)
            {
                temp_out_ECMPLinks[n_out].src_ECMPNode = ECMPNodev+k;
                temp_out_ECMPLinks[n_out].src_gate = src_gate->getId();
                temp_out_ECMPLinks[n_out].dest_ECMPNode = getECMPNodeFor(gate->getOwnerModule());
                temp_out_ECMPLinks[n_out].dest_gate = gate->getId();
                temp_out_ECMPLinks[n_out].wgt = 1.0;
                temp_out_ECMPLinks[n_out].enabl = true;
                n_out++;
            }
        }
        ECMPNodev[k].num_out_ECMPLinks = n_out;

        ECMPNodev[k].out_ECMPLinks = new ecmpTopo::ECMPLink[n_out];
        memcpy(ECMPNodev[k].out_ECMPLinks, temp_out_ECMPLinks, n_out*sizeof(ecmpTopo::ECMPLink));
        delete [] temp_out_ECMPLinks;
    }

    // fill in_ECMPLinks[] arrays
    for (int k=0; k<num_ECMPNodes; k++)
    {
        for (int l=0; l<ECMPNodev[k].num_out_ECMPLinks; l++)
        {
            ecmpTopo::ECMPLink *ECMPLink = &ECMPNodev[k].out_ECMPLinks[l];
            ECMPLink->dest_ECMPNode->in_ECMPLinks[ECMPLink->dest_ECMPNode->num_in_ECMPLinks++] = ECMPLink;
        }
    }
}

ecmpTopo::ECMPNode *ecmpTopo::getECMPNode(int i)
{
    if (i<0 || i>=num_ECMPNodes)
        throw cRuntimeError(this,"invalid ECMPNode index %d",i);
    return ECMPNodev+i;
}

ecmpTopo::ECMPNode *ecmpTopo::getECMPNodeFor(cModule *mod)
{
    // binary search can be done because ECMPNodev[] is ordered

    int lo, up, index;
    for ( lo=0, up=num_ECMPNodes, index=(lo+up)/2;
          lo<index;
          index=(lo+up)/2 )
    {
        // cycle invariant: ECMPNodev[lo].mod_id <= mod->getId() < ECMPNodev[up].mod_id
        if (mod->getId() < ECMPNodev[index].module_id)
             up = index;
        else
             lo = index;
    }
    return (mod->getId() == ECMPNodev[index].module_id) ? ECMPNodev+index : NULL;
}

void ecmpTopo::calculateUnweightedSingleShortestPathsTo(ECMPNode *_target)
{
    // multiple paths not supported :-(

    if (!_target)
        throw cRuntimeError(this,"..ShortestPathTo(): target ECMPNode is NULL");
    target = _target;

    for (int i=0; i<num_ECMPNodes; i++)
    {
       ECMPNodev[i].known = false;   // not really needed for unweighted
       ECMPNodev[i].dist = INFINITY;
       ECMPNodev[i].out_path = NULL;
    }
    target->dist = 0;

    std::deque<ECMPNode*> q;

    q.push_back(target);

    while (!q.empty())
    {
       ECMPNode *v = q.front();
       q.pop_front();

       // for each w adjacent to v...
       for (int i=0; i<v->num_in_ECMPLinks; i++)
       {
           if (!(v->in_ECMPLinks[i]->enabl)) continue;

           ECMPNode *w = v->in_ECMPLinks[i]->src_ECMPNode;
           if (!w->enabl) continue;

           if (w->dist == INFINITY)
           {
               w->dist = v->dist + 1;
               w->out_path = v->in_ECMPLinks[i];
               q.push_back(w);
           }
       }
    }
}

void ecmpTopo::calculateUnweightedMultipleShortestPathsTo(ECMPNode *_target)
{
    EV << "Doing some shortest paths. foobar.\n";
    if (!_target)
        throw cRuntimeError(this,"..ShortestPathTo(): target node is NULL");
    target = _target;

    // Set all nodes to standard values.
    for (int i=0; i<num_ECMPNodes; i++)
    {
    	ECMPNodev[i].known = false;   // not really needed for unweighted
    	ECMPNodev[i].dist = INFINITY;
    	// Erase the old out_paths.
    	for (int j = 0; j < ECMPNodev[i].getNumOutPaths(); j++) {
    		ECMPNodev[i].out_paths.erase(ECMPNodev[i].out_paths.begin(), ECMPNodev[i].out_paths.end());
    	}
    }
    target->dist = 0;
    // Holds the nodes which need to be visited.
    std::deque<ECMPNode *> open;

    // closest node is target
    ECMPNode *closest = target;
    open.push_back(closest);

    while (!open.empty())
    {
    	// Check for node closest to target
        for (uint i = 0; i < open.size(); i++) {
        	if (open.at(i)->dist <= open.at(0)->dist) {
        		closest = open.at(i);
        	}
        }
    	// Getting node from queue and removing it
    	ECMPNode *v = open.front();
    	open.pop_front();

        // for each outgoing link
        for (int i=0; i<v->num_in_ECMPLinks; i++)
        {
            if (!(v->in_ECMPLinks[i]->enabl)) continue;

            // Next node following outgoing link
            ECMPNode *w = dynamic_cast<ECMPNode *>(v->in_ECMPLinks[i]->src_ECMPNode);
            if (!w->enabl) continue;

            // Check if dist to closest is shorter than dist of w to target
            if (closest->dist + LINKWEIGHT < w->dist) {
            	w->dist = v->dist + LINKWEIGHT;
            	w->out_paths.push_back(v->in_ECMPLinks[i]);
            	// adding out path
            	open.push_back(w);
            	// delete duplicated entries
            	open.erase(std::unique(open.begin(),open.end()), open.end());
            } else if (closest->dist + LINKWEIGHT == w->dist){
            	w->out_paths.push_back(v->in_ECMPLinks[i]);
            	// adding out path
            	open.push_back(w);
            	// delete duplicated entries
            	open.erase(std::unique(open.begin(),open.end()), open.end());
            }
        }
    }
}
