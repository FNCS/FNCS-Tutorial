#!/bin/sh

# use a fresh custom loader path
unset LD_LIBRARY_PATH

# shortcut
export FNCS2_INSTALL="$HOME/FNCS2-install"

# update LD_LIBRARY_PATH
if test "x$LD_LIBRARY_PATH" = x
then
    export LD_LIBRARY_PATH="$FNCS2_INSTALL/lib"
else
    export LD_LIBRARY_PATH="$FNCS2_INSTALL/lib:$LD_LIBRARY_PATH"
fi

# update PATH
if test "x$PATH" = x
then
    export PATH="$FNCS2_INSTALL/bin"
else
    export PATH="$FNCS2_INSTALL/bin:$PATH"
fi

export FNCS_LOG_STDOUT=no

# run ns3 in separate window
export FNCS_LOG_FILE=ns3.log
xterm -e ./firstN LinkModelGLDNS3.txt &

# run gld in separate window
export FNCS_LOG_FILE=gld.log
xterm -e gridlabd fncs_GLD_300node_Feeder_1.glm &

# run fncsbroker in separate window
export FNCS_LOG_FILE=broker.log
xterm -e fncsbroker 2 &

echo "running"
