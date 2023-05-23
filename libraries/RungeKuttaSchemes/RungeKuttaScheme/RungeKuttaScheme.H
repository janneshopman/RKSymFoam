#ifndef RungeKuttaScheme_H
#define RungeKuttaScheme_H

#include "fvMesh.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "EulerDdtScheme.H"
#include "runTimeSelectionTables.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                    Class RungeKuttaScheme Declaration
\*---------------------------------------------------------------------------*/

class RungeKuttaScheme
{
protected:

    // Protected data

        //- Reference to the mesh database
        const fvMesh& mesh_;

        //- Reference to the velocity field
        volVectorField& U_;

        //- Reference to the flux field
        surfaceScalarField& phi_;

        //- Reference to the pressure field
        volScalarField& p_;

        //- Runge-Kutta stage
        label stage_;

        //- Outer iteration
        label outer_;

        //- Inner iteration
        label inner_;

        //- Number of outer iterations
        label nOuter_;

        //- Number of inner iterations
        label nInner_;

        //- Solver tolerance
        scalar tolerance_;

        //- Pressure equation initial residual
        scalar residual_;

        //- Current combined stage source
        volVectorField K_;

        //- Current combined stage face source
        surfaceScalarField Kf_;

        //- Stage pressure fields
        PtrList<volScalarField> stagePressures_;

        //- Stage source A fields
        PtrList<volVectorField> stageSourcesA_;

        //- Stage source B fields
        PtrList<volVectorField> stageSourcesB_;

        //- Stage face source A fields
        PtrList<surfaceScalarField> stageFaceSourcesA_;

        //- Stage face source B fields
        PtrList<surfaceScalarField> stageFaceSourcesB_;

    // Private Member Functions

        //- Disallow copy construct
        RungeKuttaScheme(const RungeKuttaScheme&);

        //- Disallow default bitwise assignment
        void operator=(const RungeKuttaScheme&);

public:

    //- Runtime type information
    TypeName("RungeKuttaScheme");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            RungeKuttaScheme,
            dictionary,
            (
                const fvMesh& mesh,
                volVectorField& U,
                surfaceScalarField& phi,
                volScalarField& p
            ),
            (mesh, U, phi, p)
        );

    // Selectors

        //- Return a reference to the selected Runge-Kutta scheme
        static autoPtr<RungeKuttaScheme> New
        (
            const fvMesh& mesh,
            volVectorField& U,
            surfaceScalarField& phi,
            volScalarField& p
        );

    // Constructors

        //- Construct from components
        RungeKuttaScheme
        (
            const fvMesh& mesh,
            volVectorField& U,
            surfaceScalarField& phi,
            volScalarField& p
        );

    //- Destructor
    virtual ~RungeKuttaScheme()
    {}

    // Member Functions

        // Access

            //- Return the solution dictionary
            inline const dictionary& dict() const;

            //- Return the A convection coefficient matrix (pure virtual)
            virtual const List<scalarList>& a() const = 0;

            //- Return the B diffusion coefficient matrix (pure virtual)
            virtual const List<scalarList>& b() const = 0;

            //- Current stage level
            inline label stage() const;

            //- Current outer iteration level
            inline label outer() const;

            //- Current inner iteration level
            inline label inner() const;

            //- Return the number of stages
            inline label nStages() const;

            //- Return the number of outer iterations
            inline label nOuter() const;

            //- Return the number of inner iterations
            inline label nInner() const;

            //- Return the solver tolerance
            inline scalar tolerance() const;

            //- Access to the pressure equation initial residual
            inline scalar& residual();

            //- Const access to the pressure equation initial residual
            inline const scalar& residual() const;

            //- Const access to the current combined stage source
            inline const volVectorField& K() const;

            //- Const access to the current combined stage face source
            inline const surfaceScalarField& Kf() const;

            //- Access to the stage pressures
            inline PtrList<volScalarField>& stagePressures();

            //- Const access to the stage pressures
            inline const PtrList<volScalarField>& stagePressures() const;

            //- Access to the stage sources A
            inline PtrList<volVectorField>& stageSourcesA();

            //- Const access to the stage sources A
            inline const PtrList<volVectorField>& stageSourcesA() const;

            //- Access to the stage sources B
            inline PtrList<volVectorField>& stageSourcesB();

            //- Const access to the stage sources B
            inline const PtrList<volVectorField>& stageSourcesB() const;

            //- Access to the stage face sources A
            inline PtrList<surfaceScalarField>& stageFaceSourcesA();

            //- Const access to the stage face sources A
            inline const PtrList<surfaceScalarField>& stageFaceSourcesA() const;

            //- Access to the stage face sources B
            inline PtrList<surfaceScalarField>& stageFaceSourcesB();

            //- Const access to the stage face sources B
            inline const PtrList<surfaceScalarField>& stageFaceSourcesB() const;

            //- Access to the current stage pressure
            inline volScalarField& stagePressure();

            //- Const access to the current stage pressure
            inline const volScalarField& stagePressure() const;

            //- Access to the current stage source A
            inline volVectorField& stageSourceA();

            //- Const access to the current stage source A
            inline const volVectorField& stageSourceA() const;

            //- Access to the current stage source B
            inline volVectorField& stageSourceB();

            //- Const access to the current stage source B
            inline const volVectorField& stageSourceB() const;

            //- Access to the current stage face source A
            inline surfaceScalarField& stageFaceSourceA();

            //- Const access to the current stage face source A
            inline const surfaceScalarField& stageFaceSourceA() const;

            //- Access to the current stage face source B
            inline surfaceScalarField& stageFaceSourceB();

            //- Const access to the current stage face source B
            inline const surfaceScalarField& stageFaceSourceB() const;

            //- Current diagonal A coefficient
            inline scalar A() const;

            //- Current diagonal B coefficient
            inline scalar B() const;

            //- Current C coefficient
            inline scalar C() const;

        // Switches

            //- Explicit Runge-Kutta stage A
            inline Switch explicitStageA() const;

            //- Explicit Runge-Kutta stage B
            inline Switch explicitStageB() const;

            //- Explicit Runge-Kutta stage
            inline Switch explicitStage() const;

            //- Implicit Runge-Kutta stage A
            inline Switch implicitStageA() const;

            //- Implicit Runge-Kutta stage B
            inline Switch implicitStageB() const;

            //- Implicit Runge-Kutta stage
            inline Switch implicitStage() const;

            //- First Runge-Kutta stage
            inline Switch firstStage() const;

            //- Last Runge-Kutta stage
            inline Switch lastStage() const;

            //- First outer iteration
            inline Switch firstOuter() const;

            //- Last outer iteration
            inline Switch lastOuter() const;

            //- First inner iteration
            inline Switch firstInner() const;

            //- Last inner iteration
            inline Switch lastInner() const;

            //- Solve the system of equations
            inline Switch solve() const;

        // Solution control

            //- Inner loop convergence criterion
            inline Switch innerConverged();

            //- Loop over stages
            inline Switch stageLoop();

            //- Loop over outer iterations
            inline Switch outerLoop();

            //- Loop over inner iterations
            inline Switch innerLoop();

            //- Euler scheme
            template<class Type>
            inline tmp<fvMatrix<Type>> D
            (
                const GeometricField<Type, fvPatchField, volMesh>& field
            ) const;

        // IO

            //- Read coefficients
            void read();

            //- Initialize Runge-Kutta fields
            void init();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "RungeKuttaSchemeI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
