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

#include "ECMPRoutingTable.h"

Define_Module(ECMPRoutingTable);

std::vector<IPRoute*> ECMPRoutingTable::findBestMatchingRoutes(const IPAddress& dest) const
{
	Enter_Method("findBestMatchingRoutes(%u.%u.%u.%u)", dest.getDByte(0), dest.getDByte(1), dest.getDByte(2), dest.getDByte(3)); // note: str().c_str() too slow here

	std::vector<IPRoute*> shortestpaths;
    for (RouteVector::const_iterator i=routes.begin(); i!=routes.end(); ++i)
    {
        IPRoute *e = *i;
        if (IPAddress::maskedAddrAreEqual(dest, e->getHost(), e->getNetmask()))
        {
            shortestpaths.push_back(e);
        }
    }
    return shortestpaths;
}
