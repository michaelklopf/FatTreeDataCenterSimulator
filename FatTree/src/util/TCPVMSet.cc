//
// Author: Michael Klopf
// 

#include "TCPVMSet.h"

void TCPVMSet::addVM(IPvXAddress addr) {
    this->vmset.insert(addr);
}

void TCPVMSet::removeVM(IPvXAddress addr) {
	std::set<IPvXAddress>::iterator it;
    it = this->vmset.find(addr);
    if (it != this->vmset.end())
        this->vmset.erase(it);
}

int TCPVMSet::getSize() {
    return vmset.size();
}

IPvXAddress TCPVMSet::getIPAddress(int position) {
	if ((int)vmset.size() < position) {
		opp_error("Position of element bigger than VMSet.");
	}
	std::set<IPvXAddress>::iterator it(vmset.begin());
	std::advance(it, position);
	return *it;
}

bool TCPVMSet::findIPAddress(IPvXAddress addr) {
	std::set<IPvXAddress>::iterator it;
	it = this->vmset.find(addr);
	if (it != this->vmset.end())
		return true;
	else
		return false;
}
