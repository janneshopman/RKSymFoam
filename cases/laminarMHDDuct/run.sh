#!/bin/bash

BC=$1

if [ "$BC" == "Hunt" ]; then
    cp -r 0.orgHunt 0
elif [ "$BC" == "Shercliff" ]; then
    cp -r 0.orgShercliff 0
else
    echo "Error: Input variable must be 'Hunt' or 'Shercliff'"
    exit 1
fi

blockMesh > log.blockMesh

RKSymMagFoam > log.run

python3 plotSolutions/plotUProfiles.py $BC
