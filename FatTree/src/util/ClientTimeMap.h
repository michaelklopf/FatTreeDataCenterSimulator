//
// Map which saves a pair of time intervals (as points in time) and the percentage
// of activated clients between the actual and the next point in time.
//
// Author: Michael Klopf
// 

#ifndef CLIENTTIMEMAP_H_
#define CLIENTTIMEMAP_H_

#include <map>

class ClientTimeMap {
protected:
	typedef std::map<long, double> ClientTimeTable;
	ClientTimeTable clientTimeMap;
public:
	ClientTimeMap() {};
	virtual ~ClientTimeMap() {};

	virtual double getPercentageOfActiveClients(long time);

	virtual void addNewEntry(long time, double percentage);

	virtual long getElement(int i);

	virtual int getSize();
};

#endif /* CLIENTTIMEMAP_H_ */
