//
// C++ STL Set to save and handle the ip addresses of
// virtual machines.
//
// Author: Michael Klopf
// 

#ifndef TCPVMSET_H_
#define TCPVMSET_H_

#include <set>
#include <IPvXAddress.h>

class TCPVMSet {
    protected:
        typedef std::set<IPvXAddress> VMSet;
        VMSet vmset;
    public:
        TCPVMSet() {};
        virtual ~TCPVMSet() {};

        virtual void addVM(IPvXAddress addr);

        virtual void removeVM(IPvXAddress addr);

        virtual int getSize();

        virtual IPvXAddress getIPAddress(int position);

        virtual bool findIPAddress(IPvXAddress addr);
};

#endif /* TCPVMSET_H_ */
