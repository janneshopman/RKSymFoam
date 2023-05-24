#!/bin/sh
cd "${0%/*}" || exit                                # Run from this directory
. ${WM_PROJECT_DIR:?}/bin/tools/RunFunctions        # Tutorial run functions
#------------------------------------------------------------------------------

SOLVER=$1
SCHEME=$2
SIMULATIONTYPE=$3

if [[ $SIMULATIONTYPE == "LES" ]]; then
    LESBOOL="true"
elif [[ $SIMULATIONTYPE == "DNS" ]]; then
    LESBOOL="false"
else
    LESBOOL="none"

    if [[ $SOLVER == "RKSymFOAM" ]]; then
        echo "set RKSimulationTypes to (LES), (DNS) or (LES DNS)"
    fi
fi

VARS="-DVARSOLVER=$SOLVER -DVARSCHEME=$SCHEME -DVARLESBOOL=$LESBOOL"

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
