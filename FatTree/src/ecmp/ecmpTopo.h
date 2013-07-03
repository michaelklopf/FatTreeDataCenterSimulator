//
// Author: Michael Klopf
//

#ifndef __ECMPTOPO_H
#define __ECMPTOPO_H

#include <string>
#include <vector>
#include "cownedobject.h"
#include "csimulation.h"
#include "cmodule.h"
#include "cgate.h"

NAMESPACE_BEGIN

class cPar;

// not all compilers define INFINITY (gcc does)
#ifndef INFINITY
#define INFINITY HUGE_VAL
#endif

/**
 * This is a special cTopology version for the implementation of ECMP in date
 * center networks. It will be usable for other networks too.
 *
 * To support ECMP it is necessary to know all multiple shortest paths in a network
 * to a node.
 *
 * This is fullfilled by our own implementation based on the idea of Christian Schwartz
 * diploma thesis.
 *
 * Node is renamed to ECMPNode and Link to ECMPLink to prevent naming errors.
 *
 *-------------------------------------------------------------------------------------*
 *
 * Routing support. The ecmpTopo class was designed primarily to support
 * routing in telecommunication or multiprocessor networks.
 *
 * A ecmpTopo object stores an abstract representation of the network
 * in graph form:
 * <ul>
 *   <li> each ecmpTopo ECMPNode corresponds to a module (simple or compound), and
 *   <li> each ecmpTopo edge corresponds to a ECMPLink or series of connecting ECMPLinks.
 * </ul>
 *
 * You can specify which modules (either simple or compound) you want to
 * include in the graph. The graph will include all connections among the
 * selected modules. In the graph, all ECMPNodes are at the same level, there is
 * no submodule nesting. Connections which span across compound module
 * boundaries are also represented as one graph edge. Graph edges are directed,
 * just as module gates are.
 *
 * @ingroup SimSupport
 * @see ecmpTopo::ECMPNode, ecmpTopo::ECMPLink, ecmpTopo::ECMPLinkIn, ecmpTopo::ECMPLinkOut
 */
class SIM_API ecmpTopo : public cOwnedObject
{
  public:
    class ECMPLink;
    class ECMPLinkIn;
    class ECMPLinkOut;

    /**
     * Supporting class for ecmpTopo, represents a ECMPNode in the graph.
     */
    class SIM_API ECMPNode
    {
        friend class ecmpTopo;

      private:
        int module_id;
        double wgt;
        bool enabl;

        int num_in_ECMPLinks;
        ECMPLink **in_ECMPLinks;
        int num_out_ECMPLinks;
        ECMPLink *out_ECMPLinks;

        // variables used by the shortest-path algorithms
        bool known;
        double dist;
        ECMPLink *out_path;
        std::vector<ECMPLink*> out_paths;

      public:
        /** @name ECMPNode attributes: weight, enabled state, correspondence to modules. */
        //@{

        /**
         * Returns the ID of the network module to which this ECMPNode corresponds.
         */
        int getModuleId() const  {return module_id;}

        /**
         * Returns the pointer to the network module to which this ECMPNode corresponds.
         */
        cModule *getModule() const  {return simulation.getModule(module_id);}

        /**
         * Returns the weight of this ECMPNode. Weight is used with the
         * weighted shortest path finder methods of ecmpTopo.
         */
        double getWeight() const  {return wgt;}

        /**
         * Sets the weight of this ECMPNode. Weight is used with the
         * weighted shortest path finder methods of ecmpTopo.
         */
        void setWeight(double d)  {wgt=d;}

        /**
         * Returns true of this ECMPNode is enabled. This has significance
         * with the shortest path finder methods of ecmpTopo.
         */
        bool isEnabled() const  {return enabl;}

        /**
         * Enable this ECMPNode. This has significance with the shortest path
         * finder methods of ecmpTopo.
         */
        void enable()  {enabl=true;}

        /**
         * Disable this ECMPNode. This has significance with the shortest path
         * finder methods of ecmpTopo.
         */
        void disable()  {enabl=false;}
        //@}

        /** @name ECMPNode connectivity. */
        //@{

        /**
         * Returns the number of incoming ECMPLinks to this graph ECMPNode.
         */
        int getNumInECMPLinks() const  {return num_in_ECMPLinks;}

        /**
         * Returns ith incoming ECMPLink of graph ECMPNode.
         */
        ECMPLinkIn *getECMPLinkIn(int i);

        /**
         * Returns the number of outgoing ECMPLinks from this graph ECMPNode.
         */
        int getNumOutECMPLinks() const  {return num_out_ECMPLinks;}

        /**
         * Returns ith outgoing ECMPLink of graph ECMPNode.
         */
        ECMPLinkOut *getECMPLinkOut(int i);
        //@}

        /** @name Result of shortest path extraction. */
        //@{

        /**
         * Returns the distance of this ECMPNode to the target ECMPNode.
         */
        double getDistanceToTarget() const  {return dist;}

        /**
         * Returns the number of shortest paths towards the target ECMPNode.
         * (There may be several paths with the same length.)
         */
        int getNumPaths() const  {return out_path?1:0;}

        int getNumOutPaths() const {return out_paths.size();}

        /**
         * Returns the next ECMPLink in the ith shortest paths towards the
         * target ECMPNode. (There may be several paths with the same
         * length.)
         */
        ECMPLinkOut *getPath(int) const  {return (ECMPLinkOut *)out_path;}
        //@}

        ECMPLinkOut *getOutPath(int i) const {return (ECMPLinkOut *)out_paths.at(i);}

        void addOutPath(ECMPLink* link) {out_paths.push_back(link);}
    };


    /**
     * Supporting class for ecmpTopo, represents a ECMPLink in the graph.
     */
    class SIM_API ECMPLink
    {
        friend class ecmpTopo;

      protected:
        ECMPNode *src_ECMPNode;
        int src_gate;
        ECMPNode *dest_ECMPNode;
        int dest_gate;
        double wgt;
        bool enabl;

      public:
        /**
         * Returns the weight of this ECMPLink. Weight is used with the
         * weighted shortest path finder methods of ecmpTopo.
         */
        double getWeight() const  {return wgt;}

        /**
         * Sets the weight of this ECMPLink. Weight is used with the
         * weighted shortest path finder methods of ecmpTopo.
         */
        void setWeight(double d)  {wgt=d;}

        /**
         * Returns true of this ECMPLink is enabled. This has significance
         * with the shortest path finder methods of ecmpTopo.
         */
        bool isEnabled() const  {return enabl;}

        /**
         * Enables this ECMPLink. This has significance with the shortest path
         * finder methods of ecmpTopo.
         */
        void enable()  {enabl=true;}

        /**
         * Disables this ECMPLink. This has significance with the shortest path
         * finder methods of ecmpTopo.
         */
        void disable()  {enabl=false;}
    };


    /**
     * Supporting class for ecmpTopo.
     *
     * While navigating the graph stored in a ecmpTopo, ECMPNode's methods return
     * ECMPLinkIn and ECMPLinkOut objects, which are 'aliases' to ECMPLink objects.
     * ECMPLinkIn and ECMPLinkOut provide convenience functions that return the
     * 'local' and 'remote' end of the connection when traversing the topology.
     */
    class SIM_API ECMPLinkIn : public ECMPLink
    {
      public:
        /**
         * Returns the ECMPNode at the remote end of this connection.
         *
         * Note: There is no corresponding localECMPNode() method: the local ECMPNode of
         * this connection is the ECMPNode object whose method returned
         * this ECMPLinkIn object.
         */
        ECMPNode *getRemoteECMPNode() const  {return src_ECMPNode;}

        /**
         * Returns the gate ID at the remote end of this connection.
         */
        int getRemoteGateId() const  {return src_gate;}

        /**
         * Returns the gate ID at the local end of this connection.
         */
        int getLocalGateId() const  {return dest_gate;}

        /**
         * Returns the gate at the remote end of this connection.
         */
        cGate *getRemoteGate() const  {return src_ECMPNode->getModule()->gate(src_gate);}

        /**
         * Returns the gate at the local end of this connection.
         */
        cGate *getLocalGate() const  {return dest_ECMPNode->getModule()->gate(dest_gate);}
    };


    /**
     * Supporting class for ecmpTopo.
     *
     * While navigating the graph stored in a ecmpTopo, ECMPNode's methods return
     * ECMPLinkIn and ECMPLinkOut objects, which are 'aliases' to ECMPLink objects.
     * ECMPLinkIn and ECMPLinkOut provide convenience functions that return the
     * 'local' and 'remote' end of the connection when traversing the topology.
     */
    class SIM_API ECMPLinkOut : public ECMPLink
    {
      public:
        /**
         * Returns the ECMPNode at the remote end of this connection.
         *
         * Note: There is no corresponding localECMPNode() method: the local ECMPNode of
         * this connection is the ECMPNode object whose method returned
         * this ECMPLinkIn object.
         */
        ECMPNode *getRemoteECMPNode() const  {return dest_ECMPNode;}

        /**
         * Returns the gate ID at the remote end of this connection.
         */
        int getRemoteGateId() const  {return dest_gate;}

        /**
         * Returns the gate ID at the local end of this connection.
         */
        int getLocalGateId() const  {return src_gate;}

        /**
         * Returns the gate at the remote end of this connection.
         */
        cGate *getRemoteGate() const  {return dest_ECMPNode->getModule()->gate(dest_gate);}

        /**
         * Returns the gate at the local end of this connection.
         */
        cGate *getLocalGate() const  {return src_ECMPNode->getModule()->gate(src_gate);}
    };

    /**
     * Base class for selector objects used in extract...() methods of ecmpTopo.
     * Redefine the matches() method to return whether the given module
     * should be included in the extracted topology or not.
     */
    class SIM_API Predicate
    {
      public:
        virtual ~Predicate() {}
        virtual bool matches(cModule *module) = 0;
    };

  protected:
    int num_ECMPNodes;
    ECMPNode *ECMPNodev;
    ECMPNode *target;

  public:
    /** @name Constructors, destructor, assignment */
    //@{

    /**
     * Constructor.
     */
    explicit ecmpTopo(const char *name=NULL);

    /**
     * Copy constructor.
     */
    ecmpTopo(const ecmpTopo& topo);

    /**
     * Destructor.
     */
    virtual ~ecmpTopo();

    /**
     * Assignment operator. The name member is not copied; see cNamedObject's operator=() for more details.
     */
    ecmpTopo& operator=(const ecmpTopo& topo);
    //@}

    /** @name Redefined cObject member functions. */
    //@{

    /**
     * Creates and returns an exact copy of this object.
     * See cObject for more details.
     */
    virtual ecmpTopo *dup() const  {return new ecmpTopo(*this);}

    /**
     * Produces a one-line description of the object's contents.
     * See cObject for more details.
     */
    virtual std::string info() const;

    /**
     * Serializes the object into an MPI send buffer.
     * Used by the simulation kernel for parallel execution.
     * See cObject for more details.
     */
    virtual void parsimPack(cCommBuffer *buffer);

    /**
     * Deserializes the object from an MPI receive buffer
     * Used by the simulation kernel for parallel execution.
     * See cObject for more details.
     */
    virtual void parsimUnpack(cCommBuffer *buffer);
    //@}

    /** @name Extracting the topology from a network.
     *
     * extract...() functions build topology from the model.
     * User can select which modules to include. All connections
     * between those modules will be in the topology. Connections can
     * cross compound module boundaries.
     */
    //@{

    /**
     * Extracts model topology by a user-defined criteria. Includes into the graph
     * modules for which the passed selfunc() returns nonzero. The userdata
     * parameter may take any value you like, and it is passed back to selfunc()
     * in its second argument.
     */
    void extractFromNetwork(bool (*selfunc)(cModule *,void *), void *userdata=NULL);

    /**
     * The type safe, object-oriented equivalent of extractFromNetwork(selfunc, userdata).
     */
    void extractFromNetwork(Predicate *predicate);

    /**
     * Extracts model topology by module full path. All modules whole getFullPath()
     * matches one of the patterns in given string vector will get included.
     * The patterns may contain wilcards in the same syntax as in ini files.
     *
     * An example:
     *
     * <tt>topo.extractByModulePath(cStringTokenizer("**.host[*] **.router*").asVector());</tt>
     */
    void extractByModulePath(const std::vector<std::string>& fullPathPatterns);

    /**
     * Extracts model topology by the fully qualified NED type name of the
     * modules. All modules whose getNedTypeName() is listed in the given string
     * vector will get included.
     *
     * Note: If you have all class names as a single, space-separated string,
     * you can use cStringTokenizer to turn it into a string vector:
     *
     * <tt>topo.extractByNedTypeName(cStringTokenizer("some.package.Host other.package.Router").asVector());</tt>
     */
    void extractByNedTypeName(const std::vector<std::string>& nedTypeNames);

    /**
     * Extracts model topology by a module property. All modules get included
     * that have a property with the given name and the given value
     * (more precisely, the first value of its default key being the specified
     * value). If value is NULL, the property's value may be anything except
     * "false" (i.e. the first value of the default key may not be "false").
     *
     * For example, <tt>topo.extractByProperty("ECMPNode");</tt> would extract
     * all modules that contain the <tt>\@ECMPNode</tt> property, like the following
     * one:
     *
     * <pre>
     * module X {
     *     \@ECMPNode;
     * }
     * </pre>
     *
     */
    void extractByProperty(const char *propertyName, const char *value=NULL);

    /**
     * Extracts model topology by a module parameter. All modules get included
     * that have a parameter with the given name, and the parameter's str()
     * method returns the paramValue string. If paramValue is NULL, only the
     * parameter's existence is checked but not its value.
     */
    void extractByParameter(const char *paramName, const char *paramValue=NULL);

    /**
     * Deletes the topology stored in the object.
     */
    void clear();
    //@}

    /** @name Functions to examine topology by hand.
     *
     * Users also need to rely on ECMPNode and ECMPLink member functions
     * to explore the graph stored in the object.
     */
    //@{

    /**
     * Returns the number of ECMPNodes in the graph.
     */
    int getNumECMPNodes() const  {return num_ECMPNodes;}

    /**
     * Returns pointer to the ith ECMPNode in the graph. ECMPNode's methods
     * can be used to further examine the ECMPNode's connectivity, etc.
     */
    ECMPNode *getECMPNode(int i);

    /**
     * Returns the graph ECMPNode which corresponds to the given module in the
     * network. If no graph ECMPNode corresponds to the module, the method returns
     * NULL. This method assumes that the topology corresponds to the
     * network, that is, it was probably created with one of the
     * extract...() functions.
     */
    ECMPNode *getECMPNodeFor(cModule *mod);
    //@}

    /** @name Algorithms to find shortest paths. */
    /*
     * To be implemented:
     *    -  void unweightedMultiShortestPathsTo(ECMPNode *target);
     *    -  void weightedSingleShortestPathsTo(ECMPNode *target);
     *    -  void weightedMultiShortestPathsTo(ECMPNode *target);
     */
    //@{

    /**
     * Apply the Dijkstra algorithm to find all shortest paths to the given
     * graph ECMPNode. The paths found can be extracted via ECMPNode's methods.
     */
    void calculateUnweightedSingleShortestPathsTo(ECMPNode *target);

    /**
     * Returns the ECMPNode that was passed to the most recently called
     * shortest path finding function.
     */
    ECMPNode *getTargetECMPNode() const {return target;}
    //@}

    /**
     * Applies the Dijkstra algorithm to find all shortest paths to a given
     * graph ECMP node. The paths found can be extracted via ECMPNode's methods.
     */
    void calculateUnweightedMultipleShortestPathsTo(ECMPNode *target);
};

NAMESPACE_END


#endif
