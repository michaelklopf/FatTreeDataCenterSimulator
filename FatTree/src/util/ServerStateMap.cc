//
// Author: Michael Klopf
// 

#include "ServerStateMap.h"

void ServerStateMap::addHV(IPvXAddress hvAddr, bool state) {
	serverStates[hvAddr] = state;
}

void ServerStateMap::changeState(IPvXAddress hvAddr, bool state) {
	serverStates[hvAddr] = state;
}

bool ServerStateMap::getStateFor(IPvXAddress hvAddr) {
	return serverStates[hvAddr];
}

int ServerStateMap::getSize() {
    return serverStates.size();
}
