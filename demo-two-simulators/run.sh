#!/bin/sh

fncs_broker 2 > broker.out &
FNCS_CONFIG_FILE=simA.zpl ./simA > simA.out &
FNCS_CONFIG_FILE=simB.zpl ./simB > simB.out
