#include "PS4p7q.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{
    defineTypeNameAndDebug(PS4p7q, 0);
    addToRunTimeSelectionTable(RungeKuttaScheme, PS4p7q, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaSchemes::PS4p7q::PS4p7q
(
    const fvMesh& mesh,
    volVectorField& U,
    surfaceScalarField& phi,
    volScalarField& p
)
:
    RungeKuttaScheme(mesh, U, phi, p)
{
    a_.setSize(7, scalarList(7, 0.0));

    a_[1][0] = 0.23593376536651968050;

    a_[2][0] = 0.347507356584235168;
    a_[2][1] = -0.135619353983464433;

    a_[3][0] = -0.20592852403227;
    a_[3][1] = 1.891790766221084;
    a_[3][2] = -0.89775024478958;

    a_[4][0] = -0.094354932814554;
    a_[4][1] = 1.756171412237619;
    a_[4][2] = -0.967078504769475;
    a_[4][3] = 0.069328259979890148;
    
    a_[5][0] = 0.14157883255197;
    a_[5][1] = -1.17039696277833;
    a_[5][2] = 1.30579112376331;
    a_[5][3] = -2.203541368552894;
    a_[5][4] = 2.9265683750159476;

    a_[6][0] = 0.07078941627598264;
    a_[6][1] = 0.87808570611880957;
    a_[6][2] = -0.448875122394792210;
    a_[6][3] = a_[6][2];
    a_[6][4] = a_[6][1];
    a_[6][5] = a_[6][0]; 

    init();
}

// ************************************************************************* //
