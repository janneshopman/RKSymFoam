#include "fvCFD.H"
#include "singlePhaseTransportModel.H"
#include "turbulentTransportModel.H"
#include "RungeKuttaScheme.H"
#include "fvOptions.H"

#include "midPoint.H"
#include "linear.H"
#include "reverseLinear.H"
#include "gaussConvectionScheme.H"
#include "gaussLaplacianScheme.H"
#include "uncorrectedSnGrad.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createTimeControls.H"
    #include "createFields.H"
    #include "createFvOptions.H"

    if (LES)
    {
        turbulence->validate();
    }

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< "\nStarting time loop\n" << endl;

    while (runTime.run())
    {
        #include "readTimeControls.H"
        #include "CourantNo.H"
        #include "setDeltaT.H"

        runTime++;

        Info << "Time = " << runTime.timeName() << endl;

        if (LES)
        {
            nuf =  midPoint<scalar>(mesh).interpolate(turbulence->nuEff());
        }

        pn = p.oldTime();

        while (scheme.stageLoop())
        {
            #include "prep.H"

            if (scheme.solve())
            {
                while (scheme.outerLoop())
                {
                    #include "UEqn.H"

                    while (scheme.innerLoop())
                    {
                        #include "pEqn.H"
                    }
                }
            }

            #include "store.H"
        }

        if (LES)
        {    
            turbulence->correct();
        }

        runTime.write();

        Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
             << "  ClockTime = " << runTime.elapsedClockTime() << " s"
             << nl << endl;
    }

    Info << "End\n" << endl;

    return 0;
}

// ************************************************************************* //
