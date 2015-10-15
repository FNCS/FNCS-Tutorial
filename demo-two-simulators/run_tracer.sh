#!/bin/sh

export FNCS_LOG_STDOUT=no
export FNCS_LOG_FILE=yes
export FNCS_TRACE=yes
fncs_broker 3 > broker.out &
FNCS_CONFIG_FILE=simA.zpl ./simA > simA.out &
FNCS_CONFIG_FILE=simB.zpl ./simB > simB.out &
FNCS_CONFIG_FILE=tracer.zpl fncs_tracer 100s tracer.txt > tracer.out
