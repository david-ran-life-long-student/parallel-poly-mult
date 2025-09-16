#!/bin/bash

# a simple test file to get you started

if [[ $# != 1 ]]; then
	echo "expected 1 argument, task name"
	exit 1
fi

echo Calling PolyMult${1}.check-rand repeatedly
for N in {1000..10000..200}
do
    echo N = $N
    ./bin/PolyMult${1}.check-rand $N | grep -v "errors is 0"
done
