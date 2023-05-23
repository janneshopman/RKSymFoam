Info<< "Reading field p\n" << endl;

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

volScalarField pCorr
(
    IOobject
    (
        "pCorr",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("pCorr", p.dimensions(), 0),
    p.boundaryField().types()
);

volScalarField pn
(
    IOobject
    (
        "pn",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar("pn", p.dimensions(), 0),
    p.boundaryField().types()
);

Info<< "Reading field U\n" << endl;

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

surfaceScalarField phi
(
    IOobject
    (
        "phi",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    midPoint<vector>(mesh).dotInterpolate(mesh.Sf(), U)
);

autoPtr<RungeKuttaScheme> schemePtr
(
    RungeKuttaScheme::New(mesh, U, phi, pCorr)
);

RungeKuttaScheme& scheme = schemePtr();

const scalar pnPredCoef(readScalar(scheme.dict().lookup("pnPredCoef")));

label pRefCell = 0;
scalar pRefValue = 0.0;

setRefCell(p, scheme.dict(), pRefCell, pRefValue);

singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);
