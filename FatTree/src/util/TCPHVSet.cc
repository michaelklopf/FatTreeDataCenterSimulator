//
// Author: Michael Klopf
// 

#include "TCPHVSet.h"

void TCPHVSet::addHV(IPvXAddress addr) {
    this->hvset.insert(addr);
}

void TCPHVSet::removeHV(IPvXAddress addr) {
	std::set<IPvXAddress>::iterator it;
    it = this->hvset.find(addr);
    if (it != this->hvset.end())
        this->hvset.erase(it);
}

int TCPHVSet::getSize() {
    return hvset.size();
}

IPvXAddress TCPHVSet::getIPAddress(int position) {
	if (hvset.size() < (uint)position) {
		opp_error("Position of element bigger than VMSet.");
	}
	std::set<IPvXAddress>::iterator it(hvset.begin());
	std::advance(it, position);
	return *it;
}
