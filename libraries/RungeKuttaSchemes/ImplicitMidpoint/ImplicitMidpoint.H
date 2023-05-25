/* Reference
Found for example in: 
Ascher, U. M., Ruuth, S. J., & Spiteri, R. J. (1997).  Implicit-explicit
Runge-Kutta methods for time-dependent partial differential equations. Applied
Numerical Mathematics, 25(2-3), 151-167. */ 

#ifndef ImplicitMidpoint_H
#define ImplicitMidpoint_H

#include "RungeKuttaScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{

/*---------------------------------------------------------------------------*\
                           Class ImplicitMidpoint Declaration
\*---------------------------------------------------------------------------*/

class ImplicitMidpoint
:
    public RungeKuttaScheme
{
    // Private data

        //- Coefficient matrix
        List<scalarList> a_;


public:

    //- Runtime type information
    TypeName("ImplicitMidpoint");


    // Constructors

        //- Construct from components
        ImplicitMidpoint
        (
            const fvMesh& mesh,
            volVectorField& U,
            surfaceScalarField& phi,
            volScalarField& p
        );


    //- Destructor
    ~ImplicitMidpoint()
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
