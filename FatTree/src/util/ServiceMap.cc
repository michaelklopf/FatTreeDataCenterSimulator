//
// Author: Michael Klopf
//

#include "ServiceMap.h"

void ServiceMap::addVM(IPvXAddress addr, int serviceport) {
	services[addr] = serviceport;
}

void ServiceMap::removeVM(IPvXAddress addr) {
	Services::iterator it;
	it = services.find(addr);
	if (it != services.end())
		services.erase(it);
}

int ServiceMap::getSize() {
	return services.size();
}

IPvXAddress ServiceMap::getIPAddress(int position) {
	if ((int)services.size() < position) {
		opp_error("Position of element bigger than ServiceMap.");
	}
	Services::iterator it(services.begin());
	std::advance(it,position);
	return it->first;
}

int ServiceMap::getPortForAddress(IPvXAddress addr) {
	return services[addr];
}

std::auto_ptr<ServiceMap> ServiceMap::getVMsWithPort(int serviceport) {
	//ServiceMap *vmsWithPort = new ServiceMap();
	std::auto_ptr<ServiceMap> vmsWithPort(new ServiceMap());
	for (service_iterator it = this->begin(); it != this->end(); it++) {
		if (it->second == serviceport) {
			vmsWithPort->addVM(it->first, it->second);
		}
	}
	return vmsWithPort;
}

std::auto_ptr<ServiceMap> ServiceMap::getVMsNotInOtherMap(ServiceMap *otherMap) {
	if (otherMap->getSize() > this->getSize()) {
		opp_error("Given map is too big to find elements.");
	}
	//ServiceMap *vmsNotInOtherMap = new ServiceMap();
	std::auto_ptr<ServiceMap> vmsNotInOtherMap(new ServiceMap());
	for (service_iterator it = this->begin(); it != this->end(); it++) {
		if (!otherMap->findIPAddress(it->first)) {
			vmsNotInOtherMap->addVM(it->first, it->second);
		}
	}
	return vmsNotInOtherMap;
}

bool ServiceMap::findIPAddress(IPvXAddress addr) {
	Services::iterator it;
	it = services.find(addr);
	if (it != services.end())
		return true;
	else
		return false;
}

bool ServiceMap::isThereFreeVMForService(int serviceport) {
	//ServiceMap *vms = this->getVMsWithPort(serviceport);
	std::auto_ptr<ServiceMap> vms(this->getVMsWithPort(serviceport));

	if (vms->getSize() == 0) {
		//delete vms;
		return false;
	} else {
		//delete vms;
		return true;
	}
}
