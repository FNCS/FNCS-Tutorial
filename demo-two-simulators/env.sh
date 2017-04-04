#!/bin/sh

export FNCS_INSTALL="$HOME/FNCS-install"

# update LD_LIBRARY_PATH for all but MCR
if test "x$LD_LIBRARY_PATH" = x
then
    export LD_LIBRARY_PATH="$FNCS_INSTALL/lib"
else
    export LD_LIBRARY_PATH="$FNCS_INSTALL/lib:$LD_LIBRARY_PATH"
fi

# update PATH for all but MCR
if test "x$PATH" = x
then
    export PATH="$FNCS_INSTALL/bin"
else
    export PATH="$FNCS_INSTALL/bin:$PATH"
fi
