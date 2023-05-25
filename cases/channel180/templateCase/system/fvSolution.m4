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
    object      fvSolution;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

solvers
{
    // p{Final} and U{Final} read by OpenFOAM solvers
    // pCorr{Final} read by RKSymFoam

    "(p|pCorr)"
    {
        solver          GAMG;
        tolerance       0;
        relTol          0.001;
        smoother        GaussSeidel;
    }

    "(p|pCorr)Final"
    {
        $p;
        smoother        DICGaussSeidel;
        tolerance       1e-06;
        relTol          0;
    }

    "U"
    {
        solver          smoothSolver;
        smoother        symGaussSeidel;
        tolerance       1e-05;
        relTol          0.1;
    }

    "UFinal"
    {
        $U;
        tolerance       1e-05;
        relTol          0;
    }
}

// Read by icoFoam
PISO
{
    nCorrectors     2;
    nNonOrthogonalCorrectors 0;
    pRefCell        0;
    pRefValue       0;
}

// Read by pimpleFoam
PIMPLE
{
    nOuterCorrectors 1;
    nCorrectors     2;
    nNonOrthogonalCorrectors 0;
    pRefCell        0;
    pRefValue       0;
}

// Read by RKSymFoam
RungeKutta
{
    scheme          VARSCHEME;
    nOuter          1;
    nInner          2;
    pnPredCoef      1;
    pRefCell        0;
    pRefValue       0;
    LES             VARLESBOOL;
}


// ************************************************************************* //
