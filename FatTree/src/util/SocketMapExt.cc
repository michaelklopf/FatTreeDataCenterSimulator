//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "SocketMapExt.h"

TCPSocket *SocketMapExt::findSocketForConnId(int connID)
{
    SocketMap::iterator i = socketMap.find(connID);
    ASSERT(i==socketMap.end() || i->first==i->second->getConnectionId());
    return (i==socketMap.end()) ? NULL : i->second;
}

int SocketMapExt::getConnectionID(int i)
{
    return socketMap.at(i)->getConnectionId();
}

void SocketMapExt::printSockets()
{
    for (uint i = 0; i < socketMap.size(); i++) {
        SocketMap::iterator it(socketMap.begin());
        std::advance(it,i);
        TCPSocket *sock = it->second;
        const char *addrchar1 = sock->getLocalAddress().str().c_str();
        const char *addrchar = sock->getRemoteAddress().str().c_str();
        std::cerr << "Socket local " << addrchar1 << " and remote " << addrchar << "\n";
    }
}
