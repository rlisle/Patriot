#!/usr/bin/env bash
# Compile and flash
# The specific example is specified as the 1st argument, or defaults to "everything"
# The photon device name is passed as the 2nd argument, or defaults to "myPhoton"
device=myPhoton
example=everything
if [ $# -gt 0 ]; then
    example=$1
    if [ "$2" != "" ]; then
        device=$2
    fi
fi
echo "particle flash $device examples/$example"
particle flash $device examples/$example
