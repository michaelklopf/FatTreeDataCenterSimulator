//
// Extention of TCPSocketMap, to allow us searching for
// a TCPSocket with a given connection ID.
//
// Author: Michael Klopf
// 

#ifndef SOCKETMAPEXT_H_
#define SOCKETMAPEXT_H_

#include <TCPSocketMap.h>

class SocketMapExt: public TCPSocketMap {
    public:
        TCPSocket *findSocketForConnId(int connID);

        int getConnectionID(int i);

        void printSockets();
};

#endif /* SOCKETMAPEXT_H_ */
