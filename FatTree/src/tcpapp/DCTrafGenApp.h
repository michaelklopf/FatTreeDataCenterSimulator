//
// TCP Application for creation of traffic flows in the
// data center network.
//
// Author: Michael Klopf
//

#ifndef __FATTREEVMMIGRATIONCASE1_DCTRAFGENAPP_H_
#define __FATTREEVMMIGRATIONCASE1_DCTRAFGENAPP_H_

#include <omnetpp.h>
#include <TCPBasicClientApp.h>

/**
 * TCP application to generate and send packets to controller.
 */
class DCTrafGenApp : public TCPBasicClientApp
{
	protected:
		simsignal_t processingTime;
		simtime_t departureTime;
		simtime_t arrivalTime;
	protected:
		virtual void initialize();

		virtual void socketDataArrived(int connId, void *yourPtr, cPacket *msg, bool urgent);
		virtual void sendPacket(int numBytes, int expectedReplyBytes, bool serverClose=false);
};

#endif
