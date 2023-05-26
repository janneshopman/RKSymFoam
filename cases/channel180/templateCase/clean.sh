#!/bin/bash
cd "${0%/*}" || exit                                # Run from this directory
. ${WM_PROJECT_DIR:?}/bin/tools/CleanFunctions      # Tutorial clean functions
#------------------------------------------------------------------------------

rm -rf processor*
rm -rf graphs
rm -f log.*

for f in $(find -name *.m4); do
    rm -f ${f::-3}
done

cleanTimeDirectories

#------------------------------------------------------------------------------
