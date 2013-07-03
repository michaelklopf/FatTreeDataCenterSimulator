//
// Author: Michael Klopf
// 

cplusplus {{
#include "GenericAppMsg_m.h"
}}
packet GenericAppMsg;

//
// Msg. to initiate the start of a vm.
//
packet StartVM extends GenericAppMsg {
    string vmaddress;
    int serviceport;
    int connIDtoCon;
    bool serverState;
    int activeVMSize;
}
