#!/bin/sh

export FNCS_LOG_STDOUT=no
FNCS_LOG_FILE=broker.log fncs_broker 2 > broker.out &
FNCS_LOG_FILE=simA.log FNCS_CONFIG_FILE=simA.zpl ./simA > simA.out &
FNCS_LOG_FILE=simB.log FNCS_CONFIG_FILE=simB.zpl ./simB > simB.out
