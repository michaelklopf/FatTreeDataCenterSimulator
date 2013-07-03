//
// Message type for traffic generator to put a request at
// the server.
//
// Author: Michael Klopf
// 

cplusplus {{
#include "GenericAppMsg_m.h"
}}
packet GenericAppMsg;

//
// Adds a variable to save the source address, the msg was sent from.
//
packet RequestMsg extends GenericAppMsg {
    int connID;
    //bool queued = false;
    int serviceport;
    double depTime;
}
