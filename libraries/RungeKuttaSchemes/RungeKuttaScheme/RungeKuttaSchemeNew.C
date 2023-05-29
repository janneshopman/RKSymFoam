#include "RungeKuttaScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::RungeKuttaScheme> Foam::RungeKuttaScheme::New
(
    const fvMesh& mesh,
    volVectorField& U,
    surfaceScalarField& phi,
    volScalarField& p
)
{
    const word schemeName
    (
        mesh.solutionDict().subDict("RungeKutta").lookup("scheme")
    );

    Info<< "Selecting Runge-Kutta scheme " << schemeName << endl;

    auto cstrIter =
        dictionaryConstructorTablePtr_->find(schemeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown RungeKuttaScheme type "
            << schemeName << nl << nl
            << "Valid RungeKuttaSchemes are : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<RungeKuttaScheme>
        (cstrIter()(mesh, U, phi, p));
}

// ************************************************************************* //
