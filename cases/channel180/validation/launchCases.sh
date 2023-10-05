#!/bin/bash

launchDir=$(pwd)
templateDir="$launchDir"/../templateCase

caseNames=(icoFoam RKSymFoam_DNS_BackwardEuler RKSymFoam_DNS_Kutta RKSymFoam_DNS_4p7q pimpleFoam RKSymFoam_LES_BackwardEuler RKSymFoam_LES_Kutta RKSymFoam_LES_4p7q)
solvers=(icoFoam RKSymFoam RKSymFoam RKSymFoam pimpleFoam RKSymFoam RKSymFoam RKSymFoam)
simulationTypes=(none laminar laminar laminar LES LES LES LES)
RKSchemes=(none BackwardEuler Kutta PS4p7q none BackwardEuler Kutta PS4p7q)

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
    
