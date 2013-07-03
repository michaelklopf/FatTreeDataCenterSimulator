//
// Author: Michael Klopf
// 

cplusplus {{
#include "GenericAppMsg_m.h"
}}
packet GenericAppMsg;

//
// Message which is used to stop a client.
//
packet StopClient extends GenericAppMsg {
    int delay;
    bool clientIsOff;
    int connIDSysCon;
    string clientAddr;
    int serviceport;
}
