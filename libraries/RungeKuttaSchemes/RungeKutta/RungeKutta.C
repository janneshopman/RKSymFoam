#include "RungeKutta.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(RungeKutta, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, RungeKutta, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::RungeKutta::RungeKutta
(
    const fvMesh& mesh,
    volVectorField& U,
    surfaceScalarField& phi,
    volScalarField& p
)
:
    RungeKuttaScheme(mesh, U, phi, p)
{
    a_.setSize(5, scalarList(5, 0.0));

    a_[1][0] = 0.5;

    a_[2][1] = 0.5;

    a_[3][2] = 1.0;

    a_[4][0] = 1.0/6.0;
    a_[4][1] = 1.0/3.0;
    a_[4][2] = 1.0/3.0;
    a_[4][3] = 1.0/6.0;

    init();
}

// ************************************************************************* //
