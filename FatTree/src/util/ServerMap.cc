//
// Author: Michael Klopf
// 

#include "ServerMap.h"

void ServerMap::addVM(IPvXAddress vmAddr, IPvXAddress hvAddr) {
	server[vmAddr] = hvAddr;
}

void ServerMap::changeHV(IPvXAddress vmAddr, IPvXAddress hvAddr) {
	server[vmAddr] = hvAddr;
}

void ServerMap::removeVM(IPvXAddress vmAddr) {
	Server::iterator it;
	it = server.find(vmAddr);
	if (it != server.end())
		server.erase(it);
}

int ServerMap::getSize() {
	return server.size();
}

IPvXAddress ServerMap::getServerAddr(IPvXAddress vmAddr) {
	return server[vmAddr];
}

std::auto_ptr<TCPVMSet> ServerMap::getPeerVMsOfGivenVM(IPvXAddress vmAddr) {
	IPvXAddress hvAddr = this->getServerAddr(vmAddr);
	//TCPVMSet *peerVMs = new TCPVMSet();
	std::auto_ptr<TCPVMSet> peerVMs(new TCPVMSet());
	for (server_iterator it = this->begin(); it != this->end(); it++) {
		if (it->second == hvAddr && it->first != vmAddr) {
			peerVMs->addVM(it->first);
		}
	}
	return peerVMs;
}
