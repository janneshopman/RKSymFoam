#ifndef Ascher122_H
#define Ascher122_H

#include "RungeKuttaScheme.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace RungeKuttaSchemes
{

/*---------------------------------------------------------------------------*\
                           Class Ascher122 Declaration
\*---------------------------------------------------------------------------*/

class Ascher122
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
    TypeName("Ascher122");


    // Constructors

        //- Construct from components
        Ascher122
        (
            const fvMesh& mesh,
            volVectorField& U,
            surfaceScalarField& phi,
            volScalarField& p
        );


    //- Destructor
    ~Ascher122()
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
