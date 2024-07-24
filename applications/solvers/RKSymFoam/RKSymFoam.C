/* References
RKSymFoam paper:
Komen, E. M., Hopman, J. A., Frederix, E. M. A., Trias, F. X., & Verstappen, R.
W. (2021). A symmetry-preserving second-order time-accurate PISO-based method.
Computers & Fluids, 225, 104979.

Original Runge-Kutta implementation in OpenFoam:
Komen, E. M., Frederix, E. M., Coppen, T. H. J., D’Alessandro, V., & Kuerten, J.
G. M. (2020). Analysis of the numerical dissipation rate of different
Runge–Kutta and velocity interpolation methods in an unstructured collocated
finite volume method in OpenFOAM®. Computer Physics Communications, 253, 107145.

Symmetry-preserving method:
Trias, F. X., Lehmkuhl, O., Oliva, A., Pérez-Segarra, C. D., & Verstappen, R. W.
C. P. (2014). Symmetry-preserving discretization of Navier–Stokes equations on
collocated unstructured grids. Journal of Computational Physics, 258, 246-267.

Volumetric interpolation of variables:
Santos Serrano, D., Trias Miquel, F. X., Colomer Rey, G., & Pérez Segarra, C. D.
(2022). An energy-preserving unconditionally stable fractional step method on
collocated grids. In Collection of papers presented at the 8th European Congress
on Computational Methods in Applied Sciences and Engineering (ECCOMAS Congress
2022). Scipedia.

Butcher Tableau:
Butcher, J. C. (1964). On Runge-Kutta processes of high order. Journal of the
Australian Mathematical Society, 4(2), 179-194.

Chorin pressure predictor:
Chorin, A. J. (1968). Numerical solution of the Navier-Stokes equations.
Mathematics of computation, 22(104), 745-762.

Van Kan pressure predictor:
Van Kan, J. J. I. M. (1986). A second-order accurate pressure-correction scheme
for viscous incompressible flow. SIAM journal on scientific and statistical
computing, 7(3), 870-891. */


#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "turbulentTransportModel.H"
#include "RungeKuttaScheme.H"
#include "fvOptions.H"

// Include spatial discretisation schemes needed to preserve symmetry
#include "midPoint.H"
#include "linear.H"
#include "reverseLinear.H"
#include "gaussConvectionScheme.H"
#include "gaussLaplacianScheme.H"
#include "uncorrectedSnGrad.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Symmetry-preserving solver based on Runge-Kutta"
        " methods for temporal integration."
    );

    #include "postProcess.H"

    #include "addCheckCaseOptions.H"
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "initContinuityErrs.H"
    #include "createTimeControls.H"
    #include "createFields.H"
    #include "createFvOptions.H"
    #include "CourantNo.H"
    #include "setInitialDeltaT.H"

    // Turbulence model is required, to run DNS, set:
    // 1. <transportModel Newtonian;> in constant/transportProperties, and
    // 2. <simulationType laminar;> in constant/turbulenceProperties
    turbulence->validate();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "CourantNo.H"
        #include "setDeltaT.H"

        runTime++;

        Info << "Time = " << runTime.timeName() << endl;

        // Volumetric interpolation used, see references at top of RKSymFoam.C
        // Turbulence model is required, to run DNS, set:
        // 1. <transportModel Newtonian;> in constant/transportProperties, and
        // 2. <simulationType laminar;> in constant/turbulenceProperties
        nuf = reverseLinear<scalar>(mesh).interpolate(turbulence->nuEff());

        pn = p.oldTime();

        // Loop over stages of Butcher Tableau
        // Each line in the tableau represents a stage, e.g. ForwardEuler has 2 stages
        // stageLoop function sets K and Kf which contain convection and diffusion
        // of previous stages multiplied by the coefficient from the Butcher Tableau
        while (scheme.stageLoop())
        {
            // Initialise convection (A) and diffusion (B) source fields used in UEqn.H 
            // and pEqn.H, they are updated during this stage and stored for later stages
            fvVectorMatrix UEqnSourceA(U, dimVolume*dimVelocity/dimTime);
            fvVectorMatrix UEqnSourceB(U, dimVolume*dimVelocity/dimTime);

            // Each stage has to solve for pressure unless the stage is both:
            // 1. Explicit, and
            // 2. The first stage of the scheme
            if (scheme.solve())
            {
                // Similar to PISO outer iteration loop
                while (scheme.outerLoop())
                {
                    // Volumetric interpolation used, see references at top of RKSymFoam.C
                    // Turbulence model is required, to run DNS, set:
                    // 1. <transportModel Newtonian;> in constant/transportProperties, and
                    // 2. <simulationType laminar;> in constant/turbulenceProperties
                    nuf = reverseLinear<scalar>(mesh).interpolate(turbulence->nuEff());

                    #include "UEqn.H"

                    // Similar to PISO inner iteration loop
                    while (scheme.innerLoop())
                    {
                        #include "pEqn.H"
                    }
                }
            }

            // Stores the pressure, convection and diffusion of this stage
            // to be used in subsequent stages
            #include "store.H"
        }

        // Turbulence model is required, to run DNS, set:
        // 1. <transportModel Newtonian;> in constant/transportProperties, and
        // 2. <simulationType laminar;> in constant/turbulenceProperties
        turbulence->correct();

        runTime.write();

        Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
             << "  ClockTime = " << runTime.elapsedClockTime() << " s"
             << nl << endl;
    }

    Info << "End\n" << endl;

    return 0;
}

// ************************************************************************* //
