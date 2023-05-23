#include "Alexander3.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(Alexander3, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, Alexander3, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::Alexander3::Alexander3
(
    const fvMesh& mesh,
    volVectorField& U,
    surfaceScalarField& phi,
    volScalarField& p
)
:
    RungeKuttaScheme(mesh, U, phi, p)
{
    a_.setSize(3, scalarList(3, 0.0));

    a_[0][0] = 0.435866521;

    a_[1][0] = 0.2820667392;
    a_[1][1] = 0.435866521;

    a_[2][0] = 1.208496649;
    a_[2][1] = -0.644363171;
    a_[2][2] = 0.435866521;

    init();
}

// ************************************************************************* //
