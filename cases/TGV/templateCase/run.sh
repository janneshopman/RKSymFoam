#!/bin/sh
cd "${0%/*}" || exit                                # Run from this directory
. ${WM_PROJECT_DIR:?}/bin/tools/RunFunctions        # Tutorial run functions
#------------------------------------------------------------------------------

SOLVER=$1

VARS="-DVARSOLVER=$SOLVER"

find $CASEPATH -name '*.m4' | while read IN; do
    OUT=$(echo $IN | rev | cut -c 4- | rev)

    m4 -P $VARS $IN > $OUT
done

runApplication blockMesh

runApplication setTaylorGreenFields

runApplication $(getApplication)

#------------------------------------------------------------------------------
