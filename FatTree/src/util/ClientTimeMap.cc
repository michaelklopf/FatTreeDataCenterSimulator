//
// Author: Michael Klopf
// 

#include "ClientTimeMap.h"

void ClientTimeMap::addNewEntry(long time, double percentage)
{
	clientTimeMap[time] = percentage;
}

double ClientTimeMap::getPercentageOfActiveClients(long time)
{
	return clientTimeMap[time];
}

long ClientTimeMap::getElement(int position)
{
	/*
	if ((int)clientTimeMap.size() < position) {
		opp_error("Position of element bigger than ClientTimeMap.");
	}*/
	ClientTimeTable::iterator it(clientTimeMap.begin());
	std::advance(it,position);
	return it->first;
}

int ClientTimeMap::getSize()
{
	return clientTimeMap.size();
}
