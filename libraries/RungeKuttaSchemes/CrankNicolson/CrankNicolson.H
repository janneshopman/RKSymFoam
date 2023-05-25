/* Reference
Crank, J., & Nicolson, P. (1947, January). A practical method for numerical
evaluation of solutions of partial differential equations of the heat-conduction
type. In Mathematical proceedings of the Cambridge philosophical society (Vol.
43, No. 1, pp. 50-67). Cambridge University Press. */

#ifndef CrankNicolson_H
#define CrankNicolson_H

#include "RungeKuttaScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{

/*---------------------------------------------------------------------------*\
                           Class CrankNicolson Declaration
\*---------------------------------------------------------------------------*/

class CrankNicolson
:
    public RungeKuttaScheme
{
    // Private data

        //- Coefficient matrix
        List<scalarList> a_;


public:

    //- Runtime type information
    TypeName("CrankNicolson");


    // Constructors

        //- Construct from components
        CrankNicolson
        (
            const fvMesh& mesh,
            volVectorField& U,
            surfaceScalarField& phi,
            volScalarField& p
        );


    //- Destructor
    ~CrankNicolson()
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
