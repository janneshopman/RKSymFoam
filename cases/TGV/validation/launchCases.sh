#!/bin/bash

launchDir=$(pwd)
templateDir="$launchDir"/../templateCase

caseNames=(icoFoam RKSymFoam)
solvers=(icoFoam RKSymFoam)
simulationTypes=(none laminar)
RKSchemes=(none BackwardEuler)

for iRun in "${!caseNames[@]}"; do
    caseName="${caseNames[$iRun]}"
    solver="${solvers[$iRun]}"
    simulationType="${simulationTypes[$iRun]}"
    RKScheme="${RKSchemes[$iRun]}"

    if [[ -d $caseName ]]; then
        echo "$caseName" already exists, not overwriting
    else
        echo creating "$caseName"
        cp -r "$templateDir" "$caseName"

        cd "$caseName"

        source run.sh $solver $simulationType $RKScheme &

        cd "$launchDir"
    fi
done
    
