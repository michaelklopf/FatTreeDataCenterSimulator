//
// Message type to represent an answer from the server, sent
// back to the traffic generator.
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
packet ReplyMsg extends GenericAppMsg {
    int connID;
    string vmaddress;
    int serviceport;
    double depTime;
}
