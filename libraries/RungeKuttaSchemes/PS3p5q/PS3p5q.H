/* Reference
Capuano, F., Coppola, G., Rández, L., and de Luca, L. (2017), Explicit Runge-Kutta
schemes for incompressible flow with improved energy-conservation properties.
Journal of Computational Physics 328, pp. 86-94 */

#ifndef PS3p5q_H
#define PS3p5q_H

#include "RungeKuttaScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{

/*---------------------------------------------------------------------------*\
                           Class PS3p5q Declaration
\*---------------------------------------------------------------------------*/

class PS3p5q
:
    public RungeKuttaScheme
{
    // Private data

        //- Coefficient matrix
        List<scalarList> a_;


public:

    //- Runtime type information
    TypeName("PS3p5q");


    // Constructors

        //- Construct from components
        PS3p5q
        (
            const fvMesh& mesh,
            volVectorField& U,
            surfaceScalarField& phi,
            volScalarField& p
        );


    //- Destructor
    ~PS3p5q()
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
