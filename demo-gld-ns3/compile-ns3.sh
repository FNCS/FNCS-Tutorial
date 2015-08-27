#!/bin/bash

g++ \
-g \
-o firstN \
firstN.cc \
-I$FNCS2_INSTALL/include \
-I$FNCS2_INSTALL/include/fncs \
-I$FNCS2_INSTALL/include/ns3.22 \
-L$FNCS2_INSTALL/lib \
-lns3.22-applications-debug \
-lns3.22-fncs-debug \
-lns3.22-nix-vector-routing-debug \
-lns3.22-internet-debug \
-lns3.22-bridge-debug \
-lns3.22-csma-debug \
-lns3.22-point-to-point-debug \
-lns3.22-mpi-debug \
-lns3.22-stats-debug \
-lns3.22-network-debug \
-lns3.22-core-debug \
-lfncs \
-lczmq \
-lzmq

#g++ \
#-g \
#-o firstN \
#firstN.cc \
#-I$FNCS_INSTALL/include/fncs \
#-I$FNCS_INSTALL/include/ns3.19 \
#-L$FNCS_INSTALL/lib \
#-lns3.19-fncs-debug \
#-lfncs \
#-lzmq \
#-lns3.19-point-to-point-debug \
#-lns3.19-applications-debug \
#-lns3.19-nix-vector-routing-debug \
#-lns3.19-csma-debug \
#-lns3.19-point-to-point-layout-debug \
#-lns3.19-wifi-debug \
#-lns3.19-netanim-debug \
#-lns3.19-wimax-debug \
#-lns3.19-core-debug

