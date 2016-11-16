#!/bin/sh

export FNCS_LOG_STDOUT=no
export FNCS_LOG_FILE=yes
export FNCS_TRACE=yes
if test "x$MATLAB" = x
then
export MATLAB=/Applications/MATLAB_R2015a.app/bin/matlab
fi

fncs_broker 3 > broker.out &
FNCS_CONFIG_FILE=simA.zpl $MATLAB -nodesktop -nojvm -nosplash -r "addpath('$HOME/ccsi/fncs/matlab'); simA; exit;" &
FNCS_CONFIG_FILE=simB.zpl ./simB > simB.out &
FNCS_CONFIG_FILE=tracer.zpl fncs_tracer 100s tracer.txt > tracer.out
wait
