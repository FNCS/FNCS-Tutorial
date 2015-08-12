#!/bin/sh

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

