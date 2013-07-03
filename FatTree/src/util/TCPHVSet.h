//
// C++ STL Set to save and handle the ip addresses of
// hypervisors/servers.
//
// Author: Michael Klopf
// 

#ifndef TCPHVSET_H_
#define TCPHVSET_H_

#include <set>
#include <IPvXAddress.h>

class TCPHVSet {
protected:
    typedef std::set<IPvXAddress> HVSet;
    HVSet hvset;
public:
    TCPHVSet() {};
    virtual ~TCPHVSet() {};

    virtual void addHV(IPvXAddress addr);

    virtual void removeHV(IPvXAddress addr);

    virtual int getSize();

    virtual IPvXAddress getIPAddress(int position);
};

#endif /* TCPHVSET_H_ */
