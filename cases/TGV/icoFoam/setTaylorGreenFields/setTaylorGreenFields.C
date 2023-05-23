#include "argList.H"
#include "timeSelector.H"
#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions();

    #include "setRootCase.H"
    #include "createTime.H"

    instantList timeDirs = timeSelector::select0(runTime, args);

    #include "createNamedMesh.H"

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info << "Time = " << runTime.timeName() << "\n" << endl;

        volScalarField p
        (
            IOobject
            (
                "p",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh
        );

        volVectorField U
        (
            IOobject
            (
                "U",
                runTime.timeName(),
                mesh,
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            mesh
        );

        forAll(p, cellI)
        {
            const scalar x = mesh.C().field()[cellI][0];
            const scalar y = mesh.C().field()[cellI][1];

            U.field()[cellI] = vector
                (
                    Foam::sin(x)*Foam::cos(y),
                  - Foam::cos(x)*Foam::sin(y),
                    0.0
                );

            p.field()[cellI] = 0.25*(Foam::cos(2.0*x) + Foam::cos(2.0*y));
        }

        volScalarField magSqrUInit = U&U;
        double volAverageMagSqrUInit = magSqrUInit.weightedAverage(mesh.V()).value();

        Info<< "Initial volume averaged MagSqr(U): " << volAverageMagSqrUInit << "\n" << endl;

        U.write();
        p.write();
    }
}
