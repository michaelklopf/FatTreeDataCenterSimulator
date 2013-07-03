//
// Author: Michael Klopf
// 

cplusplus {{
#include "GenericAppMsg_m.h"
}}
packet GenericAppMsg;

//
// Message type to enable virtual machine migration.
//
packet VMMigMsg extends GenericAppMsg {
    int msgtype enum(StateNames);
    enum StateNames
    {
        INIT_MIG = 0;
        
        ENABLEHV2 = 1;
        HV2ENABLED = 2;
        
        STARTVMMIG = 3;
        
        COPYINGDATA = 4;
        COPYSTART = 5;
        DATARECEIVED = 6;
        COPYSUCCESS = 7;
        
        TURNOFFVM1 = 8;
        VM1TURNEDOFF = 9;
        
        STARTVM2 = 10;
        VM2TURNEDON = 11;
        STARTSUCCESS = 12;
        
        TURNOFFHV1 = 13;
        HV1TURNEDOFF = 14;
    }
    string hv1address;
    string hv2address;
    string vm1address;
    string vm2address;
    string conaddress;
    
    bool turnOffHV1;
    
    bool turnOnHV2;
    
    int connIdfromHV1ToCon;
    int connIdToMigS;
    int connIdfromHV2ToHV1;
    
    int serviceport;
}
