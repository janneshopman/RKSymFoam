#include "ImplicitMidpoint.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(ImplicitMidpoint, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, ImplicitMidpoint, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::ImplicitMidpoint::ImplicitMidpoint
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

    a_[0][0] = 0.5;

    a_[1][0] = 1.0;

    init();
}

// ************************************************************************* //
