#!/bin/sh
cd "${0%/*}" || exit                                # Run from this directory
. ${WM_PROJECT_DIR:?}/bin/tools/RunFunctions        # Tutorial run functions
#------------------------------------------------------------------------------

cp -r ../0.orig 0

runApplication blockMesh

#- Run serial
runApplication $(getApplication)

#- Run parallel
#runApplication decomposePar
#runParallel $(getApplication)
#runApplication reconstructPar

runApplication postChannel

#------------------------------------------------------------------------------
