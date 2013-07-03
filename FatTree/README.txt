OMNeT++ 4.2.2 and INET 1.99* Fat-Tree Data Center Simulator
Author: Michael Klopf

The idea behind this simulator is to simulate the impact of energy efficiency techniques on
a Fat-Tree data center. For this we have the possibilty to monitor the power consumption of
the data center depending on the load. To save energy it is possible to turn off unused
virtual machines. This is based on a hysteresis approach. Additionally we can migrate virtual
machines to other servers, to consolidate the number of active servers. This feature is
implemented and works for itself but it is not working when traffic is routed to the virtual
machines.

A few words to begin with:
The simulation was created as part of a diploma thesis at the University of Würzburg, Germany.
Due to time constraints it is partly unfinished, also commented debug code is still in there.
Also some features still won't work, i.e., virtual machine migration. It is completely 
implemented but bugs in the underlying INET framework (which is the only reason I can come up
with, it doesn't work) made it impossible to finish it on time.

We make extensive use here of INETs tcp applications. An application module is able to receive
multiple tcp connections with each query routed to a new thread. It's build on the basic examples
coming with INET.

So all the necessary logic for moving messages around is either in the *App.cc or *Thread.cc.
A server and a hypervisor are treated as one thing here.

FEATURES:
- Implementation of the Fat-Tree data center architecture in OMNeT++.
- IP addresses are set according to the Fat-Tree conventions (but not the routing, which is standard).
- Traffic generation and distribution of that traffic among the virtual machines of the data center, which
  will be also answered and going back to the traffic generator to measure processing time.
- Packet arrivals following a negative exponential inter arrival time.
- Energy efficiency mechanism: Unused virtual machines and servers are turned off, according to a
  hysteresis approach which depends on the length of the job queue in the controller modul.

The project structure is as follows:

/FatTree
    /simulations
        'multiple ini files as well as .ned root'
        /result
            'empty'
    /src
        /components
            '.ned modules to create the fat-tree structure'
        /ecmp
            'half baked ecmp implementation, which is not fully working, round robin style load balancing'
        /logic
            /ipaddress
                'modules to set ip addresses of the modules correctly'
            /routing
                'logic to fill routing tables'
        /messages
            'different message types'
        /router
            '.ned modules for routers'
        /tcpapp
            'here is the brain of the simulator'
        /trafficgeneration
            'http and ftp supported'
        /util
            'special data structures'

IMPORTANT:
There is an INET issue in this version. In TCPConnection.h there is are multiple timeout values:
/** @name Timeout values */
//@{
#define TCP_TIMEOUT_CONN_ESTAB    75    // 75 seconds
#define TCP_TIMEOUT_FIN_WAIT_2   600    // 10 minutes
#define TCP_TIMEOUT_2MSL         4//240    // 2 * 2 minutes
#define TCP_TIMEOUT_SYN_REXMIT     3    // initially 3 seconds
#define TCP_TIMEOUT_SYN_REXMIT_MAX 240  // 4 mins (will only be used with SYN+ACK: with SYN CONN_ESTAB occurs sooner)
//@}
2MSL can cause severe problems during runtime. Increase the value to prevent these errors.

INI File:
- There are a couple of ini files in the simulations folder.
- HTTP traffic (or here HTTPService) is off, because it creates a large amount of traffic, which increases the simulation runtime.
  Tests have shown that FTP traffic alone behaves similar to http traffic.
- The time when a load change should occur is set here, but the values for the load function are part of TGAppWithIAT.cc.
  In the initialize() method you will find following array:
  double loadAdap[13] = {0.79, 0.52, 0.22, 0.13, 0.10, 0.40, 0.72, 0.74, 0.77, 0.80, 0.61, 0.73, 0.79};
  Would you like to change load every hour instead of all two hours like here, you would have to add 12 more values and increase
  the array size accordingly. Note that the first and last value are the same to close the loop.
You are able to change here:
- Power consumption values for the hardware.
- Border values for the hysteresis.
- Start-up and shut-down times.
- Traffic characteristics.

Issues:
- ECMP implementation not working perfectly fine. Routing tables look awkward sometimes.
- Virtual machine migration not working while having load on the data center. Possible bug in the INET
  TCP modules.
- Long runtimes due to the packet simulators nature.

Possible areas for improvement:
- Upgrading it to the newest OMNeT++ and INET versions.
- Fixing virtual machine migration.
- Improving the process of choosing the best target and source vm for migration.
- Adding the ElasticTree technique to the simulation, to turn off unused routing hardware (this could be done in the same
  way as the virtual machine turns off the hypervisor when turning off).
  