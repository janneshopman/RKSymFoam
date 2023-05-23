#include "Ascher121.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(Ascher121, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, Ascher121, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::Ascher121::Ascher121
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

    a_[1][0] = 1.0;
    a_[2][1] = 1.0;

    b_[1][1] = 1.0;
    b_[2][1] = 1.0;

    init();
}

// ************************************************************************* //
