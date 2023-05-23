#include "DIRK2.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(DIRK2, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, DIRK2, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::DIRK2::DIRK2
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

    const scalar gamma(1.0-Foam::sqrt(2.0)/2.0);

    a_[0][0] = gamma;

    a_[1][0] = 1.0-gamma;
    a_[1][1] = gamma;

    init();
}

// ************************************************************************* //
