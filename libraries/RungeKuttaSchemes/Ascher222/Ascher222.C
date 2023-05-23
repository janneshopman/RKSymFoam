#include "Ascher222.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(Ascher222, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, Ascher222, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::Ascher222::Ascher222
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
    b_.setSize(4, scalarList(4, 0.0));

    const scalar gamma(1.0-Foam::sqrt(2.0)/2.0);
    const scalar delta(1.0-0.5/gamma);

    a_[1][0] = gamma;

    a_[2][0] = delta;
    a_[2][1] = 1.0-delta;

    a_[3][0] = delta;
    a_[3][1] = 1.0-delta;

    b_[1][1] = gamma;

    b_[2][1] = 1.0-gamma;
    b_[2][2] = gamma;

    b_[3][1] = 1.0-gamma;
    b_[3][2] = gamma;

    init();
}

// ************************************************************************* //
