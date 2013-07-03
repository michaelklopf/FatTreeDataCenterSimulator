//
// Author: Michael Klopf
// 

cplusplus {{
#include "GenericAppMsg_m.h"
}}
packet GenericAppMsg;

//
// Msg. to inititate turning off a vm.
//
packet ShutDownVM extends GenericAppMsg {
    string vmaddress;
    int serviceport;
    int connIDtoCon;
    bool serverState;
}
