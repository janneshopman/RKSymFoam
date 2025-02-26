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

dimensionedScalar rho
(
    "rho",
    dimDensity,
    transportProperties
);

dimensionedScalar sigma
(
    "sigma",
    dimensionSet(-1, -3, 3, 0, 0, 2, 0),
    transportProperties
);

surfaceScalarField sigmaf
(
    IOobject
    (
        "sigmaf",
        runTime.constant(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    sigma
);

Info<< "Reading electromagneticProperties\n" << endl;
IOdictionary electromagneticProperties
(
    IOobject
    (
        "electromagneticProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

dimensionedVector B0
(
    "B0",
    dimensionSet(1, 0, -2, 0, 0, -1, 0),
    electromagneticProperties
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

Info<< "Reading field PotE\n" << endl;
volScalarField PotE
(
    IOobject
    (
        "PotE",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    mesh,
    dimensionedScalar(dimPower/dimCurrent, Zero),
    p.boundaryField().types()
);

volScalarField PotECorr
(
    IOobject
    (
        "PotECorr",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar(dimPower/dimCurrent, Zero),
    PotE.boundaryField().types()
);

// Extra electric potential field to store electric potential at the previous time step
volScalarField PotEn
(
    IOobject
    (
        "PotEn",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::NO_WRITE
    ),
    mesh,
    dimensionedScalar(dimPower/dimCurrent, Zero),
    PotE.boundaryField().types()
);

Info<< "Reading field J\n" << endl;
volVectorField J
(
    IOobject
    (
        "J",
        runTime.timeName(),
        mesh,
        IOobject::READ_IF_PRESENT,
        IOobject::AUTO_WRITE
    ),
    sigma * ((U^B0) - fv::gaussGrad<scalar>::gradf(linear<scalar>(mesh).interpolate(PotE), "gradPotE"))
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
// pnPredCoef < 0 activates dynamic predictor, using checkerboard coefficient
double pnPredCoef(readScalar(scheme.dict().lookup("pnPredCoef")));
bool dynamicpnPredCoef(false);
double pnPredCoefOld(0.0);

if (pnPredCoef < 0.0)
{
    dynamicpnPredCoef = true;
    pnPredCoef = 1.0;
    Info<< "Running with dynamic predictor pressure" << endl;
}
else
{
    Info<< "Running with predictor pressure = " << pnPredCoef << " * previous time step pressure" << endl;
}

label pRefCell = 0;
scalar pRefValue = 0.0;

setRefCell(p, scheme.dict(), pRefCell, pRefValue);

// Read the coefficient to determine the weight of the previous time step
// in setting the electric potential predictor
// pnPredCoef = 0 equals Chorin method, see references at the top of RKSymFoam.C
// pnPredCoef = 1 equals Van Kan method, see references at the top of RKSymFoam.C
// pnPredCoef < 0 activates dynamic predictor, using checkerboard coefficient
double PotEnPredCoef(readScalar(mesh.solutionDict().subDict("PotE").lookup("PotEnPredCoef")));
bool dynamicPotEnPredCoef(false);
double PotEnPredCoefOld(0.0);

if (PotEnPredCoef < 0.0)
{
    dynamicPotEnPredCoef = true;
    PotEnPredCoef = 1.0;
    Info<< "Running with dynamic predictor electric potential" << endl;
}
else
{
    Info<< "Running with predictor electric potential = " << PotEnPredCoef << " * previous time step electric potential" << endl;
}

label PotERefCell = 0;
scalar PotERefValue = 0.0;

setRefCell(PotE, mesh.solutionDict().subDict("PotE"), PotERefCell, PotERefValue);

// Suppressing some warnings
if (dynamicpnPredCoef && pnPredCoefOld == 0.0 && dynamicPotEnPredCoef && PotEnPredCoefOld == 0.0){}

// Turbulence model is required, to run DNS, set:
// 1. <transportModel Newtonian;> in constant/transportProperties, and
// 2. <simulationType laminar;> in constant/turbulenceProperties
singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);

IOdictionary turbulenceProperties
(
    IOobject
    (
        "turbulenceProperties",
        runTime.constant(),
        mesh,
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE
    )
);

bool noModelNewtonian;

if ((transportProperties.get<word>("transportModel") == "Newtonian") && (turbulenceProperties.get<word>("simulationType") == "laminar"))
{
    noModelNewtonian = true;
}else
{
    noModelNewtonian = false;
}

if (!noModelNewtonian)
{
    Info<< "Using turbulence model, N.B. fvSchemes entry used for turbulence->divDevRhoReff(U)" << endl;
}
