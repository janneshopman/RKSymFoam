#include "DIRK4.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(DIRK4, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, DIRK4, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::DIRK4::DIRK4
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

    a_[0][0] = 0.5;

    a_[1][0] = 1.0/6.0;
    a_[1][1] = 0.5;

    a_[2][0] = -0.5;
    a_[2][1] = 0.5;
    a_[2][2] = 0.5;

    a_[3][0] = 3.0/2.0;
    a_[3][1] = -3.0/2.0;
    a_[3][2] = 0.5;
    a_[3][3] = 0.5;

    init();
}

// ************************************************************************* //
