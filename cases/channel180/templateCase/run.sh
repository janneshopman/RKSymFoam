#!/bin/bash
cd "${0%/*}" || exit                                # Run from this directory
. ${WM_PROJECT_DIR:?}/bin/tools/RunFunctions        # Tutorial run functions
#------------------------------------------------------------------------------

SOLVER=$1
SIMTYPE=$2
RKSCHEME=$3

# Checking input arguments - solver
if [[ $SOLVER == "icoFoam" ]]; then
    echo "Running with icoFoam"
    SIMTYPEREQUIRED="false"
    RKSCHEMEREQUIRED="false"
elif [[ $SOLVER == "pimpleFoam" ]]; then
    echo "Running with pimpleFoam"
    SIMTYPEREQUIRED="true"
    RKSCHEMEREQUIRED="false"
elif [[ $SOLVER == "RKSymFoam" ]]; then
    echo "Running with RKSymFoam"
    SIMTYPEREQUIRED="true"
    RKSCHEMEREQUIRED="true"
else
    echo "Please enter solver <icoFoam>, <pimpleFoam> or <RKSymFoam> as first argument"
    exit
fi

# Checking input arguments - simulation type
if [[ $SIMTYPEREQUIRED == "true" ]]; then
    if [[ $SIMTYPE != "LES" ]] && [[ $SIMTYPE != "laminar" ]]; then
        echo "Please enter simulation type <LES> or <laminar> as second argument, use <laminar> for DNS"
        exit
    fi
else
    if [[ $SIMTYPE != "" ]]; then
        echo "Simulation type input not needed, set to <none>"
    fi
    SIMTYPE="none"
fi

# Checking input arguments - Runge-Kutta scheme
if [[ $RKSCHEMEREQUIRED == "true" ]]; then
    if [[ $RKSCHEME != "BackwardEuler" && $RKSCHEME != "Kutta" ]]; then
        echo "Please enter Runge-Kutta scheme <BackwardEuler> or <Kutta> as third argument"
        exit
    fi
else
    if [[ $RKSCHEME != "" ]]; then
        echo "Runge-Kutta scheme input not needed, set to <none>"
    fi
    RKSCHEME="none"
fi

VARS="-DVARSOLVER=$SOLVER -DVARSIMTYPE=$SIMTYPE -DVARRKSCHEME=$RKSCHEME"

find $CASEPATH -name '*.m4' | while read IN; do
    OUT=$(echo $IN | rev | cut -c 4- | rev)

    m4 -P $VARS $IN > $OUT
done

runApplication blockMesh

#- Run serial
runApplication $(getApplication)

#- Run parallel 
#runApplication decomposePar
#runParallel $(getApplication)
#runApplication reconstructPar

runApplication postChannel

#------------------------------------------------------------------------------
