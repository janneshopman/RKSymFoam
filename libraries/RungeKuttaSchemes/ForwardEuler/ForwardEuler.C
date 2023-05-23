#include "ForwardEuler.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(ForwardEuler, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, ForwardEuler, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::ForwardEuler::ForwardEuler
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

    a_[1][0] = 1.0;

    init();
}

// ************************************************************************* //
