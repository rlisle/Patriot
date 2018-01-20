#!/usr/bin/env bash
# Compile only
# The specific example is specified as the 1st argument, or defaults to "everything"
example=everything
if [ $# -gt 0 ]; then
    example=$1
else
    example="starter"
fi
echo "particle compile photon examples/$example"
particle compile photon examples/$example
