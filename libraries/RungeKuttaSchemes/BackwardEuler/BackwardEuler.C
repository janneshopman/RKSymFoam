#include "BackwardEuler.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(BackwardEuler, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, BackwardEuler, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::BackwardEuler::BackwardEuler
(
    const fvMesh& mesh,
    volVectorField& U,
    surfaceScalarField& phi,
    volScalarField& p
)
:
    RungeKuttaScheme(mesh, U, phi, p)
{
    a_.setSize(1, scalarList(1, 0.0));

    a_[0][0] = 1.0;

    init();
}

// ************************************************************************* //
