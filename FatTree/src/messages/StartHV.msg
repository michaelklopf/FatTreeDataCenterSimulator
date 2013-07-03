//
// Author: Michael Klopf
// 

cplusplus {{
#include "GenericAppMsg_m.h"
}}
packet GenericAppMsg;

//
// Msg to initiate the start of a hypervisor/server.
//
packet StartHV extends GenericAppMsg {
    string hvaddress;
    int source @enum(SourceNames);
    enum SourceNames
    {
        VM = 0;
        CONTROLLER = 1;
    }
    bool serverState;
}
