#!/bin/sh
cd "${0%/*}" || exit                                # Run from this directory
. ${WM_PROJECT_DIR:?}/bin/tools/CleanFunctions      # Tutorial clean functions
#------------------------------------------------------------------------------

rm -rf 0
rm -rf postProcessing
rm -f  log.*
cleanTimeDirectories

#------------------------------------------------------------------------------
