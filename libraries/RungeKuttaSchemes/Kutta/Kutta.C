#include "Kutta.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(Kutta, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, Kutta, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::Kutta::Kutta
(
    const fvMesh& mesh,
    volVectorField& U,
    surfaceScalarField& phi,
    volScalarField& p
)
:
    RungeKuttaScheme(mesh, U, phi, p)
{
    a_.setSize(4, scalarList(4, 0.0));

    a_[1][0] = 0.5;

    a_[2][0] = -1.0;
    a_[2][1] = 2.0;

    a_[3][0] = 1.0/6.0;
    a_[3][1] = 2.0/3.0;
    a_[3][2] = 1.0/6.0;

    init();
}

// ************************************************************************* //
