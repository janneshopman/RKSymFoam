/* Reference
Ascher, U. M., Ruuth, S. J., & Spiteri, R. J. (1997).  Implicit-explicit
Runge-Kutta methods for time-dependent partial differential equations. Applied
Numerical Mathematics, 25(2-3), 151-167. */ 

#ifndef Ascher233_H
#define Ascher233_H

#include "RungeKuttaScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{

/*---------------------------------------------------------------------------*\
                           Class Ascher233 Declaration
\*---------------------------------------------------------------------------*/

class Ascher233
:
    public RungeKuttaScheme
{
    // Private data

        //- A coefficient matrix
        List<scalarList> a_;

        //- B coefficient matrix
        List<scalarList> b_;


public:

    //- Runtime type information
    TypeName("Ascher233");


    // Constructors

        //- Construct from components
        Ascher233
        (
            const fvMesh& mesh,
            volVectorField& U,
            surfaceScalarField& phi,
            volScalarField& p
        );


    //- Destructor
    ~Ascher233()
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
                return b_;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RungeKuttaSchemes
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
