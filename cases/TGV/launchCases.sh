#!/bin/bash

source $FOAM_SRC/../bin/tools/RunFunctions

launchDir=$(pwd)
runDir="$launchDir"/runs
templateDir="$launchDir"/templateCase

solvers=(icoFoam RKSymFoam)

for solver in ${solvers[@]}; do
    caseDir="$runDir"/"$solver"

    if [[ -d $caseDir ]]; then
        echo "$caseDir" already exists, not overwriting
    else
        echo creating "$caseDir" from "$templateDir"
        cp -r "$templateDir" "$caseDir"

        cd "$caseDir"

        source run.sh $solver &

        cd "$launchDir"
    fi
done
