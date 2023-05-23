#include "CrankNicolson.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(CrankNicolson, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, CrankNicolson, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::CrankNicolson::CrankNicolson
(
    const fvMesh& mesh,
    volVectorField& U,
    surfaceScalarField& phi,
    volScalarField& p
)
:
    RungeKuttaScheme(mesh, U, phi, p)
{
    a_.setSize(2, scalarList(2, 0.0));

    a_[1][0] = 0.5;
    a_[1][1] = 0.5;

    init();
}

// ************************************************************************* //
