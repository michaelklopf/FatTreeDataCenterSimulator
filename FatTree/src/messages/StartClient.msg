//
// Author: Michael Klopf
// 

cplusplus {{
#include "GenericAppMsg_m.h"
}}
packet GenericAppMsg;

//
// Used to start a client up.
//
packet StartClient extends GenericAppMsg {
    int delay;
    bool clientIsOn;
    string clientAddr;
    int serviceport;
}
