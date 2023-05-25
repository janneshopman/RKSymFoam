/* Reference
Found for example in: 
Ascher, U. M., Ruuth, S. J., & Spiteri, R. J. (1997).  Implicit-explicit
Runge-Kutta methods for time-dependent partial differential equations. Applied
Numerical Mathematics, 25(2-3), 151-167. */ 

#ifndef ForwardEuler_H
#define ForwardEuler_H

#include "RungeKuttaScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{

/*---------------------------------------------------------------------------*\
                           Class ForwardEuler Declaration
\*---------------------------------------------------------------------------*/

class ForwardEuler
:
    public RungeKuttaScheme
{
    // Private data

        //- Coefficient matrix
        List<scalarList> a_;


public:

    //- Runtime type information
    TypeName("ForwardEuler");


    // Constructors

        //- Construct from components
        ForwardEuler
        (
            const fvMesh& mesh,
            volVectorField& U,
            surfaceScalarField& phi,
            volScalarField& p
        );


    //- Destructor
    ~ForwardEuler()
    {}


    // Member Functions

        // Access

            //- Return the A coefficient matrix
            virtual const List<scalarList>& a() const
            {
                return a_;
            }

            //- Return the B coefficient matrix
            virtual const List<scalarList>& b() const
            {
                return a_;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RungeKuttaSchemes
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
