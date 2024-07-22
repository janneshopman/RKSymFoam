Info<< "Reading transportProperties\n" << endl;

IOdictionary transportProperties
(
    IOobject
    (
        "transportProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

dimensionedScalar nu
(
    "nu",
    dimViscosity,
    transportProperties
);

surfaceScalarField nuf
(
    IOobject
    (
        "nuf",
        runTime.constant(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    nu
);

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

// Poisson solver solves only for the pressure correction
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

// Extra pressure field to store pressure at the previous time step
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

// Volumetric interpolation used, see references at top of RKSymFoam.C
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
    reverseLinear<vector>(mesh).dotInterpolate(mesh.Sf(), U)
);

autoPtr<RungeKuttaScheme> schemePtr
(
    RungeKuttaScheme::New(mesh, U, phi, pCorr)
);

RungeKuttaScheme& scheme = schemePtr();

// Read the coefficient to determine the weight of the previous time step
// in setting the pressure predictor
// pnPredCoef = 0 equals Chorin method, see references at the top of RKSymFoam.C
// pnPredCoef = 1 equals Van Kan method, see references at the top of RKSymFoam.C
const scalar pnPredCoef(readScalar(scheme.dict().lookup("pnPredCoef")));

Info<< "Running with predictor pressure = " << pnPredCoef << " * previous time step pressure" << endl;

label pRefCell = 0;
scalar pRefValue = 0.0;

setRefCell(p, scheme.dict(), pRefCell, pRefValue);

// Turbulence model is required, to run DNS, set:
// 1. <transportModel Newtonian;> in constant/transportProperties, and
// 2. <simulationType laminar;> in constant/turbulenceProperties
singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);
