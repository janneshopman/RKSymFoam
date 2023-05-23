#include "Ascher122.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(Ascher122, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, Ascher122, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::Ascher122::Ascher122
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
    b_.setSize(3, scalarList(3, 0.0));

    a_[1][0] = 0.5;
    a_[2][1] = 1.0;

    b_[1][1] = 0.5;
    b_[2][1] = 1.0;

    init();
}

// ************************************************************************* //
