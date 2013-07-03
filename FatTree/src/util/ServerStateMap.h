//
// Map storing a pair of ipaddress and bool value,
// which describes the on/off state of a server.
//
// Author: Michael Klopf
// 

#ifndef SERVERSTATEMAP_H_
#define SERVERSTATEMAP_H_

#include <map>
#include <IPvXAddress.h>

class ServerStateMap {
protected:
	typedef std::map<IPvXAddress, bool> ServerState;
	ServerState serverStates;
	typedef ServerState::const_iterator state_iterator;
	state_iterator begin() {return serverStates.begin();}
	state_iterator end() {return serverStates.end();}
public:
	ServerStateMap() {};
	virtual ~ServerStateMap() {};

	/*
	 * Add new server, state pair.
	 */
	virtual void addHV(IPvXAddress hvAddr, bool state);

	/*
	 * Change server state.
	 */
	virtual void changeState(IPvXAddress hvAddr, bool state);

	/*
	 * Get state of server.
	 */
	virtual bool getStateFor(IPvXAddress hvAddr);

	/*
	 * Get number of elements in map.
	 */
	virtual int getSize();
};

#endif /* SERVERSTATEMAP_H_ */
