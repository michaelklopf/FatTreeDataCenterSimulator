#
# OMNeT++/OMNEST Makefile for FatTree
#
# This file was generated with the command:
#  opp_makemake -f --deep -I../../inet/src/networklayer/ipv4 -I../../inet/src/networklayer/common -I../../inet/src/networklayer/rsvp_te -I../../inet/src/networklayer/icmpv6 -I../../inet/src/transport/tcp -I../../inet/src/networklayer/mpls -I../../inet/src/base -I../../inet/src/networklayer/ted -I../../inet/src/util/headerserializers -I../../inet/src/networklayer/contract -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/mfcore -I../../inet/src/networklayer/ipv6 -I../../inet/src/transport/sctp -I../../inet/src/world -I../../inet/src/applications/pingapp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/networklayer/ldp -I../../inet/src/transport/udp -I../../inet/src/applications/tcpapp -L../../inet/out/$(CONFIGNAME)/src -linet -KINET_PROJ=../../inet
#

# Name of target to be created (-o option)
TARGET = FatTree$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../inet/src/networklayer/ipv4 \
    -I../../inet/src/networklayer/common \
    -I../../inet/src/networklayer/rsvp_te \
    -I../../inet/src/networklayer/icmpv6 \
    -I../../inet/src/transport/tcp \
    -I../../inet/src/networklayer/mpls \
    -I../../inet/src/base \
    -I../../inet/src/networklayer/ted \
    -I../../inet/src/util/headerserializers \
    -I../../inet/src/networklayer/contract \
    -I../../inet/src/util \
    -I../../inet/src/transport/contract \
    -I../../inet/src/linklayer/mfcore \
    -I../../inet/src/networklayer/ipv6 \
    -I../../inet/src/transport/sctp \
    -I../../inet/src/world \
    -I../../inet/src/applications/pingapp \
    -I../../inet/src/linklayer/contract \
    -I../../inet/src/networklayer/arp \
    -I../../inet/src/networklayer/ldp \
    -I../../inet/src/transport/udp \
    -I../../inet/src/applications/tcpapp \
    -I. \
    -Icomponents \
    -Iecmp \
    -Ilogic \
    -Ilogic/ipaddress \
    -Ilogic/routing \
    -Imessages \
    -Iout \
    -Iout/gcc-release \
    -Iout/gcc-release/ecmp \
    -Iout/gcc-release/logic \
    -Iout/gcc-release/logic/ipaddress \
    -Iout/gcc-release/logic/routing \
    -Iout/gcc-release/messages \
    -Iout/gcc-release/tcpapp \
    -Iout/gcc-release/util \
    -Irouter \
    -Itcpapp \
    -Itrafficgeneration \
    -Iutil

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/ecmp/ConfigECMPRouting.o \
    $O/ecmp/ECMPRoutingTable.o \
    $O/ecmp/ecmpTopo.o \
    $O/ecmp/IPECMP.o \
    $O/logic/ipaddress/AggIPConfig.o \
    $O/logic/ipaddress/ContIPConfig.o \
    $O/logic/ipaddress/CoreIPConfig.o \
    $O/logic/ipaddress/EdgeIPConfig.o \
    $O/logic/ipaddress/ServerIPConfig.o \
    $O/logic/ipaddress/VMIPConfig.o \
    $O/logic/routing/ConfigRouting.o \
    $O/tcpapp/AggRouterApp.o \
    $O/tcpapp/CoreRouterApp.o \
    $O/tcpapp/DCTCPControllerApp.o \
    $O/tcpapp/DCTCPControllerThread.o \
    $O/tcpapp/DCTrafGenApp.o \
    $O/tcpapp/EdgeRouterApp.o \
    $O/tcpapp/ExtTrafficGenerationApp.o \
    $O/tcpapp/HyperVisorApp.o \
    $O/tcpapp/HyperVisorThread.o \
    $O/tcpapp/SystemController.o \
    $O/tcpapp/SystemControllerThread.o \
    $O/tcpapp/TGAppWithIAT.o \
    $O/tcpapp/TGTestApp.o \
    $O/tcpapp/TrafficGenerationApp.o \
    $O/tcpapp/TrafficGenerationThread.o \
    $O/tcpapp/VirtualMachineApp.o \
    $O/tcpapp/VirtualMachineThread.o \
    $O/util/ClientTimeMap.o \
    $O/util/ServerMap.o \
    $O/util/ServerStateMap.o \
    $O/util/ServiceMap.o \
    $O/util/SocketMapExt.o \
    $O/util/TCPHVSet.o \
    $O/util/TCPVMSet.o \
    $O/messages/ReplyMsg_m.o \
    $O/messages/RequestMsg_m.o \
    $O/messages/ShutDownHV_m.o \
    $O/messages/ShutDownVM_m.o \
    $O/messages/StartClient_m.o \
    $O/messages/StartHV_m.o \
    $O/messages/StartVM_m.o \
    $O/messages/StopClient_m.o \
    $O/messages/TTimer_m.o \
    $O/messages/TTimerCon_m.o \
    $O/messages/VMMigMsg_m.o

# Message files
MSGFILES = \
    messages/ReplyMsg.msg \
    messages/RequestMsg.msg \
    messages/ShutDownHV.msg \
    messages/ShutDownVM.msg \
    messages/StartClient.msg \
    messages/StartHV.msg \
    messages/StartVM.msg \
    messages/StopClient.msg \
    messages/TTimer.msg \
    messages/TTimerCon.msg \
    messages/VMMigMsg.msg

# Other makefile variables (-K)
INET_PROJ=../../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f FatTree FatTree.exe libFatTree.so libFatTree.a libFatTree.dll libFatTree.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f components/*_m.cc components/*_m.h
	-rm -f ecmp/*_m.cc ecmp/*_m.h
	-rm -f logic/*_m.cc logic/*_m.h
	-rm -f logic/ipaddress/*_m.cc logic/ipaddress/*_m.h
	-rm -f logic/routing/*_m.cc logic/routing/*_m.h
	-rm -f messages/*_m.cc messages/*_m.h
	-rm -f out/*_m.cc out/*_m.h
	-rm -f out/gcc-release/*_m.cc out/gcc-release/*_m.h
	-rm -f out/gcc-release/ecmp/*_m.cc out/gcc-release/ecmp/*_m.h
	-rm -f out/gcc-release/logic/*_m.cc out/gcc-release/logic/*_m.h
	-rm -f out/gcc-release/logic/ipaddress/*_m.cc out/gcc-release/logic/ipaddress/*_m.h
	-rm -f out/gcc-release/logic/routing/*_m.cc out/gcc-release/logic/routing/*_m.h
	-rm -f out/gcc-release/messages/*_m.cc out/gcc-release/messages/*_m.h
	-rm -f out/gcc-release/tcpapp/*_m.cc out/gcc-release/tcpapp/*_m.h
	-rm -f out/gcc-release/util/*_m.cc out/gcc-release/util/*_m.h
	-rm -f router/*_m.cc router/*_m.h
	-rm -f tcpapp/*_m.cc tcpapp/*_m.h
	-rm -f trafficgeneration/*_m.cc trafficgeneration/*_m.h
	-rm -f util/*_m.cc util/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc components/*.cc ecmp/*.cc logic/*.cc logic/ipaddress/*.cc logic/routing/*.cc messages/*.cc out/*.cc out/gcc-release/*.cc out/gcc-release/ecmp/*.cc out/gcc-release/logic/*.cc out/gcc-release/logic/ipaddress/*.cc out/gcc-release/logic/routing/*.cc out/gcc-release/messages/*.cc out/gcc-release/tcpapp/*.cc out/gcc-release/util/*.cc router/*.cc tcpapp/*.cc trafficgeneration/*.cc util/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/ecmp/ConfigECMPRouting.o: ecmp/ConfigECMPRouting.cc \
	ecmp/ConfigECMPRouting.h \
	ecmp/ecmpTopo.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/ecmp/ECMPRoutingTable.o: ecmp/ECMPRoutingTable.cc \
	ecmp/ECMPRoutingTable.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTable.h
$O/ecmp/IPECMP.o: ecmp/IPECMP.cc \
	ecmp/ECMPRoutingTable.h \
	ecmp/IPECMP.h \
	$(INET_PROJ)/src/base/AbstractQueue.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/base/ProtocolMap.h \
	$(INET_PROJ)/src/base/QueueBase.h \
	$(INET_PROJ)/src/base/ReassemblyBuffer.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/arp/ARPPacket_m.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMP.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPAccess.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage.h \
	$(INET_PROJ)/src/networklayer/ipv4/ICMPMessage_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPDatagram_m.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPFragBuf.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTable.h \
	$(INET_PROJ)/src/networklayer/ipv4/RoutingTableAccess.h
$O/ecmp/ecmpTopo.o: ecmp/ecmpTopo.cc \
	ecmp/ecmpTopo.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h
$O/logic/ipaddress/AggIPConfig.o: logic/ipaddress/AggIPConfig.cc \
	logic/ipaddress/AggIPConfig.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h
$O/logic/ipaddress/ContIPConfig.o: logic/ipaddress/ContIPConfig.cc \
	logic/ipaddress/ContIPConfig.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h
$O/logic/ipaddress/CoreIPConfig.o: logic/ipaddress/CoreIPConfig.cc \
	logic/ipaddress/CoreIPConfig.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h
$O/logic/ipaddress/EdgeIPConfig.o: logic/ipaddress/EdgeIPConfig.cc \
	logic/ipaddress/EdgeIPConfig.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h
$O/logic/ipaddress/ServerIPConfig.o: logic/ipaddress/ServerIPConfig.cc \
	logic/ipaddress/ServerIPConfig.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h
$O/logic/ipaddress/VMIPConfig.o: logic/ipaddress/VMIPConfig.cc \
	logic/ipaddress/VMIPConfig.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h
$O/logic/routing/ConfigRouting.o: logic/routing/ConfigRouting.cc \
	logic/routing/ConfigRouting.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/INotifiable.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/base/NotificationBoard.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_PROJ)/src/networklayer/ipv4/IRoutingTable.h
$O/messages/ReplyMsg_m.o: messages/ReplyMsg_m.cc \
	messages/ReplyMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/RequestMsg_m.o: messages/RequestMsg_m.cc \
	messages/RequestMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/ShutDownHV_m.o: messages/ShutDownHV_m.cc \
	messages/ShutDownHV_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/ShutDownVM_m.o: messages/ShutDownVM_m.cc \
	messages/ShutDownVM_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/StartClient_m.o: messages/StartClient_m.cc \
	messages/StartClient_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/StartHV_m.o: messages/StartHV_m.cc \
	messages/StartHV_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/StartVM_m.o: messages/StartVM_m.cc \
	messages/StartVM_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/StopClient_m.o: messages/StopClient_m.cc \
	messages/StopClient_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/TTimerCon_m.o: messages/TTimerCon_m.cc \
	messages/TTimerCon_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/TTimer_m.o: messages/TTimer_m.cc \
	messages/TTimer_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/messages/VMMigMsg_m.o: messages/VMMigMsg_m.cc \
	messages/VMMigMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h
$O/tcpapp/AggRouterApp.o: tcpapp/AggRouterApp.cc \
	messages/RequestMsg_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/AggRouterApp.h \
	tcpapp/DCTCPControllerApp.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/CoreRouterApp.o: tcpapp/CoreRouterApp.cc \
	messages/RequestMsg_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/CoreRouterApp.h \
	tcpapp/DCTCPControllerApp.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/DCTCPControllerApp.o: tcpapp/DCTCPControllerApp.cc \
	messages/ReplyMsg_m.h \
	messages/RequestMsg_m.h \
	messages/StartClient_m.h \
	messages/TTimerCon_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/HyperVisorApp.h \
	tcpapp/SystemController.h \
	tcpapp/VirtualMachineApp.h \
	util/ClientTimeMap.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/DCTCPControllerThread.o: tcpapp/DCTCPControllerThread.cc \
	messages/ReplyMsg_m.h \
	messages/RequestMsg_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/DCTCPControllerThread.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/DCTrafGenApp.o: tcpapp/DCTrafGenApp.cc \
	messages/ReplyMsg_m.h \
	messages/RequestMsg_m.h \
	tcpapp/DCTrafGenApp.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPBasicClientApp.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPGenericCliAppBase.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h
$O/tcpapp/EdgeRouterApp.o: tcpapp/EdgeRouterApp.cc \
	messages/RequestMsg_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/EdgeRouterApp.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/ExtTrafficGenerationApp.o: tcpapp/ExtTrafficGenerationApp.cc \
	messages/RequestMsg_m.h \
	messages/StopClient_m.h \
	messages/TTimer_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/ExtTrafficGenerationApp.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/HyperVisorApp.o: tcpapp/HyperVisorApp.cc \
	messages/RequestMsg_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/HyperVisorApp.h \
	tcpapp/VirtualMachineApp.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/HyperVisorThread.o: tcpapp/HyperVisorThread.cc \
	messages/RequestMsg_m.h \
	messages/ShutDownHV_m.h \
	messages/StartHV_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/HyperVisorApp.h \
	tcpapp/HyperVisorThread.h \
	tcpapp/VirtualMachineApp.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/SystemController.o: tcpapp/SystemController.cc \
	messages/RequestMsg_m.h \
	messages/ShutDownVM_m.h \
	messages/StartClient_m.h \
	messages/StartVM_m.h \
	messages/StopClient_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/SystemController.h \
	util/ClientTimeMap.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/SystemControllerThread.o: tcpapp/SystemControllerThread.cc \
	messages/RequestMsg_m.h \
	messages/ShutDownVM_m.h \
	messages/StartClient_m.h \
	messages/StartVM_m.h \
	messages/StopClient_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/SystemController.h \
	tcpapp/SystemControllerThread.h \
	util/ClientTimeMap.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/TGAppWithIAT.o: tcpapp/TGAppWithIAT.cc \
	messages/RequestMsg_m.h \
	messages/StopClient_m.h \
	messages/TTimer_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/TGAppWithIAT.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/TGTestApp.o: tcpapp/TGTestApp.cc \
	messages/RequestMsg_m.h \
	messages/StopClient_m.h \
	messages/TTimer_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/TGTestApp.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/TrafficGenerationApp.o: tcpapp/TrafficGenerationApp.cc \
	messages/RequestMsg_m.h \
	tcpapp/TrafficGenerationApp.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h
$O/tcpapp/TrafficGenerationThread.o: tcpapp/TrafficGenerationThread.cc \
	messages/ReplyMsg_m.h \
	messages/RequestMsg_m.h \
	messages/StartClient_m.h \
	messages/StopClient_m.h \
	messages/TTimer_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/TGAppWithIAT.h \
	tcpapp/TrafficGenerationThread.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/VirtualMachineApp.o: tcpapp/VirtualMachineApp.cc \
	logic/routing/ConfigRouting.h \
	messages/RequestMsg_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/HyperVisorApp.h \
	tcpapp/VirtualMachineApp.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/tcpapp/VirtualMachineThread.o: tcpapp/VirtualMachineThread.cc \
	logic/routing/ConfigRouting.h \
	messages/ReplyMsg_m.h \
	messages/RequestMsg_m.h \
	messages/ShutDownHV_m.h \
	messages/ShutDownVM_m.h \
	messages/StartHV_m.h \
	messages/StartVM_m.h \
	messages/VMMigMsg_m.h \
	tcpapp/DCTCPControllerApp.h \
	tcpapp/HyperVisorApp.h \
	tcpapp/VirtualMachineApp.h \
	tcpapp/VirtualMachineThread.h \
	util/ServerMap.h \
	util/ServerStateMap.h \
	util/ServiceMap.h \
	util/SocketMapExt.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/applications/tcpapp/GenericAppMsg_m.h \
	$(INET_PROJ)/src/applications/tcpapp/TCPSrvHostApp.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/base/NotifierConsts.h \
	$(INET_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/ipv4/IPv4InterfaceData.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/util/ClientTimeMap.o: util/ClientTimeMap.cc \
	util/ClientTimeMap.h
$O/util/ServerMap.o: util/ServerMap.cc \
	util/ServerMap.h \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h
$O/util/ServerStateMap.o: util/ServerStateMap.cc \
	util/ServerStateMap.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h
$O/util/ServiceMap.o: util/ServiceMap.cc \
	util/ServiceMap.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h
$O/util/SocketMapExt.o: util/SocketMapExt.cc \
	util/SocketMapExt.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/transport/contract/TCPCommand_m.h \
	$(INET_PROJ)/src/transport/contract/TCPSocket.h \
	$(INET_PROJ)/src/transport/contract/TCPSocketMap.h
$O/util/TCPHVSet.o: util/TCPHVSet.cc \
	util/TCPHVSet.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h
$O/util/TCPVMSet.o: util/TCPVMSet.cc \
	util/TCPVMSet.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h

