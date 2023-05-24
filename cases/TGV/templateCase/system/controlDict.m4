/*--------------------------------*- C++ -*----------------------------------*\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  v2012                                 |
|   \\  /    A nd           | Website:  www.openfoam.com                      |
|    \\/     M anipulation  |                                                 |
\*---------------------------------------------------------------------------*/
FoamFile
{
    version     2.0;
    format      ascii;
    class       dictionary;
    location    "system";
    object      controlDict;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

application     VARSOLVER;

startFrom       startTime;

startTime       0;

stopAt          endTime;

endTime         10.0;

deltaT          0.01;

writeControl    adjustableRunTime;

writeInterval   1;

purgeWrite      0;

writeFormat     ascii;

writePrecision  6;

writeCompression off;

timeFormat      general;

timePrecision   6;

runTimeModifiable true;

functions
{
    #includeFunc magSqr(U)

    kineticEnergy
    {
        type            volFieldValue;

        operation       volAverage;
        regionType      all;
    
        executeControl  writeTime;
        writeControl    writeTime;
        writeFields     false;
        log             false;

        fields
        (
            magSqr(U)
        );
    }
}


// ************************************************************************* //
