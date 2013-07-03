//
// ServiceMaps are used to save an ip address of a
// service plus the port number which is used for
// this service, i.e., 80 for htttp, 21 for ftp etc.
//
// Author: Michael Klopf
// 

#ifndef SERVICEMAP_H_
#define SERVICEMAP_H_

#include <map>
#include <IPvXAddress.h>
#include <memory>

class ServiceMap {
protected:
	typedef std::map<IPvXAddress, int> Services;
	typedef Services::const_iterator service_iterator;
	service_iterator begin() {return services.begin();}
	service_iterator end() {return services.end();}
	Services services;
public:
	ServiceMap() {};
	virtual ~ServiceMap() {};

	/*
	 * Adding a VM with addr and port to the map.
	 */
    virtual void addVM(IPvXAddress addr, int serviceport);

    /*
     * Removing a VM with addr and port from the map.
     */
    virtual void removeVM(IPvXAddress addr);

    /*
     * Get number of the elements in the map.
     */
    virtual int getSize();

    /*
     * Get ip address for it's position in map.
     */
    virtual IPvXAddress getIPAddress(int position);

    /*
     * Get the port connected to an ip address.
     */
    virtual int getPortForAddress(IPvXAddress addr);

    /*
     * Returns a new ServiceMap which includes only the elements with
     * the given port number.
     */
    virtual std::auto_ptr<ServiceMap> getVMsWithPort(int serviceport);

    /*
     * Returns a new ServiceMap which includes all the elements which
     * can't be found in the otherMap.
     *
     * This means that the size of otherMap has to be bigger than this map.
     */
    virtual std::auto_ptr<ServiceMap> getVMsNotInOtherMap(ServiceMap *otherMap);

    /*
     * Returns true if a given ip address can be found in this map.
     */
    virtual bool findIPAddress(IPvXAddress addr);

    /*
     * Returns true if there are vms with given port in the map.
     */
    virtual bool isThereFreeVMForService(int serviceport);
};

#endif /* SERVICEMAP_H_ */
