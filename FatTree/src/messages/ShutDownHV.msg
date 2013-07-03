//
// Author: Michael Klopf
// 

cplusplus {{
#include "GenericAppMsg_m.h"
}}
packet GenericAppMsg;

//
// Msg. to initiate turning off a hypervisor/server.
//
packet ShutDownHV extends GenericAppMsg {
    string hvaddress;
    bool serverState;
}
