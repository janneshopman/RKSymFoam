#!/bin/bash

source $FOAM_SRC/../bin/tools/RunFunctions

launchDir=$(pwd)
runDir="$launchDir"/runs
templateDir="$launchDir"/templateCase

solvers=(icoFoam pimpleFoam RKSymFoam)

#Only affecting RKSymFoam runs
RKSchemes=(BackwardEuler Kutta)     #set to "(<scheme1> <scheme2> ...)"
RKSimulationTypes=(LES DNS)         #set to "(LES)", "(DNS)" or "(LES DNS)"

for solver in ${solvers[@]}; do
    if [[ $solver == "RKSymFoam" ]]; then
        schemes=("${RKSchemes[@]}")
        simulationTypes=("${RKSimulationTypes[@]}")
    else
        schemes=(none)
        simulationTypes=(none)
    fi

    for scheme in ${schemes[@]}; do
        for simulationType in ${simulationTypes[@]}; do
            caseDir="$runDir"/"$solver"

            if [[ $solver == RKSymFoam ]]; then
                caseDir="$caseDir"_"$simulationType"_"$scheme"
            fi

            if [[ -d $caseDir ]]; then
                echo "$caseDir" already exists, not overwriting
            else
                echo creating "$caseDir" from "$templateDir"
                cp -r "$templateDir" "$caseDir"

                cd "$caseDir"

                source run.sh $solver $scheme $simulationType &

                cd "$launchDir"
            fi
        done
    done
done
