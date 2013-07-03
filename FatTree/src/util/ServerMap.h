//
// ServerMap is used to save the ipaddress of all
// virtual machines and the hypervisors or server
// they are bound to.
//
// Author: Michael Klopf
// 

#ifndef SERVERMAP_H_
#define SERVERMAP_H_

#include <map>
#include <IPvXAddress.h>
#include "TCPVMSet.h"
#include <memory>

class ServerMap {
protected:
	typedef std::map<IPvXAddress, IPvXAddress> Server;
	typedef Server::const_iterator server_iterator;
	server_iterator begin() {return server.begin();}
	server_iterator end() {return server.end();}
	Server server;
public:
	ServerMap() {};
	virtual ~ServerMap() {};

	/*
	 * Add a pair of VM and Server.
	 */
    virtual void addVM(IPvXAddress vmAddr, IPvXAddress hvAddr);

    /*
     * Change the hypervisor of a virtual machine. This
     * works similar to adding because of the side effect of
     * map[key] = value.
     */
    virtual void changeHV(IPvXAddress vmAddr, IPvXAddress hvAddr);

    /*
     * Removing a VM from the list.
     */
    virtual void removeVM(IPvXAddress vmAddr);

    /*
     * Get number of the elements in the map.
     */
    virtual int getSize();

    /*
     * Get ip address of hypervisor for a certain vm address.
     */
    virtual IPvXAddress getServerAddr(IPvXAddress vmAddr);

    /*
     * Get all other vms which belong to the hypervisor of the given vm.
     */
    virtual std::auto_ptr<TCPVMSet> getPeerVMsOfGivenVM(IPvXAddress vmAddr);
};

#endif /* SERVERMAP_H_ */
