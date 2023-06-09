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
    // p{Final} and U read by icoFOAM
    // pCorr{Final} read by RKSymFoam

    "(p|pCorr)"
    {
        solver          PCG;
        preconditioner  DIC;
        tolerance       1e-04;
        relTol          0.01;
    }

    "(p|pCorr)Final"
    {
        $p;
        relTol          0;
    }

    U
    {
        solver          smoothSolver;
        smoother        symGaussSeidel;
        tolerance       1e-05;
        relTol          0;
    }
}

// Read by icoFoam
PISO
{
    nCorrectors     2;
    nNonOrthogonalCorrectors 0;
    pRefPoint       (3.141528 3.141528 0);
    pRefValue       0.5;
}

// Read by RKSymFoam
RungeKutta
{
    scheme          VARRKSCHEME;
    nOuter          1;
    nInner          2;
    pnPredCoef      1;
    pRefPoint       (3.141528 3.141528 0);
    pRefValue       0.5;
}


// ************************************************************************* //
