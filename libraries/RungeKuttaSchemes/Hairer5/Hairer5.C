#include "Hairer5.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(Hairer5, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, Hairer5, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::Hairer5::Hairer5
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

    a_[0][0] = 0.25;

    a_[1][0] = 0.5;
    a_[1][1] = 0.25;

    a_[2][0] = 17.0/50.0;
    a_[2][1] = -1.0/25.0;
    a_[2][2] = 0.25;

    a_[3][0] = 371.0/1360.0;
    a_[3][1] = -137.0/2720.0;
    a_[3][2] = 15.0/544.0;
    a_[3][3] = 0.25;

    a_[4][0] = 25.0/24.0;
    a_[4][1] = -49.0/48.0;
    a_[4][2] = 125.0/16.0;
    a_[4][3] = -85.0/12.0;
    a_[4][4] = 0.25;

    init();
}

// ************************************************************************* //
