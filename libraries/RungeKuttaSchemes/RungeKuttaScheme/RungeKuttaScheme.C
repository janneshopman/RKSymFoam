#include "RungeKuttaScheme.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(RungeKuttaScheme, 0);
    defineRunTimeSelectionTable(RungeKuttaScheme, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::RungeKuttaScheme::RungeKuttaScheme
(
    const fvMesh& mesh,
    volVectorField& U,
    surfaceScalarField& phi,
    volScalarField& p
)
:
    mesh_(mesh),
    U_(U),
    phi_(phi),
    p_(p),
    stage_(0),
    outer_(0),
    inner_(0),
    residual_(1E99),
    K_
    (
        IOobject
        (
            "K",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedVector
        (
            "K",
            U_.dimensions()/dimTime,
            vector::zero
        )
    ),
    Kf_
    (
        IOobject
        (
            "Kf",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar
        (
            "Kf",
            U_.dimensions()*dimArea/dimTime,
            0.0
        )
    )
{
    read();
}

// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

void Foam::RungeKuttaScheme::read()
{
    const dictionary& dict = mesh_.solutionDict().subDict("RungeKutta");

    nOuter_ = readLabel(dict.lookup("nOuter"));
    nInner_ = readLabel(dict.lookup("nInner"));

    tolerance_ = dict.lookupOrDefault<scalar>("tolerance", 0.0);
}

void Foam::RungeKuttaScheme::init()
{
    stagePressures_.setSize(nStages());

    stageSourcesA_.setSize(nStages());
    stageSourcesB_.setSize(nStages());

    stageFaceSourcesA_.setSize(nStages());
    stageFaceSourcesB_.setSize(nStages());

    for (label stage = 1; stage <= nStages(); stage++)
    {
        stagePressures_.set
        (
            stage-1,
            new volScalarField
            (
                IOobject
                (
                    IOobject::groupName
                    (
                        p_.name(),
                        std::to_string(int(stage))
                    ),
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                p_
            )
        );

        stageSourcesA_.set
        (
            stage-1,
            new volVectorField
            (
                IOobject
                (
                    IOobject::groupName
                    (
                        K_.name(),
                        std::to_string(int(stage))
                    ),
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                mesh_,
                dimensionedVector
                (
                    K_.name(),
                    K_.dimensions(),
                    vector::zero
                )
            )
        );

        stageSourcesB_.set
        (
            stage-1,
            new volVectorField
            (
                IOobject
                (
                    IOobject::groupName
                    (
                        K_.name(),
                        std::to_string(int(stage))
                    ),
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                mesh_,
                dimensionedVector
                (
                    K_.name(),
                    K_.dimensions(),
                    vector::zero
                )
            )
        );

        stageFaceSourcesA_.set
        (
            stage-1,
            new surfaceScalarField
            (
                IOobject
                (
                    IOobject::groupName
                    (
                        Kf_.name(),
                        std::to_string(int(stage))
                    ),
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                mesh_,
                dimensionedScalar
                (
                    Kf_.name(),
                    Kf_.dimensions(),
                    0.0
                )
            )
        );

        stageFaceSourcesB_.set
        (
            stage-1,
            new surfaceScalarField
            (
                IOobject
                (
                    IOobject::groupName
                    (
                        Kf_.name(),
                        std::to_string(int(stage))
                    ),
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE,
                    false
                ),
                mesh_,
                dimensionedScalar
                (
                    Kf_.name(),
                    Kf_.dimensions(),
                    0.0
                )
            )
        );
    }
}

// ************************************************************************* //
