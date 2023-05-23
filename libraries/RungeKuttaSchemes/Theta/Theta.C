#include "Theta.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(Theta, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, Theta, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::Theta::Theta
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

    const dictionary& dict = mesh_.solutionDict().subDict("RungeKutta");

    const scalar theta(readScalar(dict.lookup("theta")));

    a_[1][0] = (1.0-theta);
    a_[1][1] = theta;

    init();
}

// ************************************************************************* //
