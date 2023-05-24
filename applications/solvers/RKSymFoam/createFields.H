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

const bool LES(readBool(scheme.dict().lookup("LES")));

if (!LES)
{
    Info<<endl;
    Info<<"--------------------------------------------------------------------------------"<<endl;
    Info<<"|Running DNS, initialising a dummy turbulence model                            |"<<endl;
    Info<<"|OpenFoam will read from constant/transportProperties                          |"<<endl;
    Info<<"|and constant/turbulenceProperties                                             |"<<endl;
    Info<<"|                                                                              |"<<endl;
    Info<<"|Example steps to set the dummies:                                             |"<<endl;
    Info<<"|                                                                              |"<<endl;
    Info<<"|1. In constant/transportProperties, add a line:                               |"<<endl;
    Info<<"|transportModel  Newtonian;                                                    |"<<endl;
    Info<<"|                                                                              |"<<endl;
    Info<<"|2. Create a file constant/turbulenceProperties, with:                         |"<<endl;
    Info<<"|<begin of file>                                                               |"<<endl;
    Info<<"|FoamFile                                                                      |"<<endl;
    Info<<"|{                                                                             |"<<endl;
    Info<<"|    version     2.0;                                                          |"<<endl;
    Info<<"|    format      ascii;                                                        |"<<endl;
    Info<<"|    class       dictionary;                                                   |"<<endl;
    Info<<"|    location    \"constant\";                                                   |"<<endl;
    Info<<"|    object      turbulenceProperties;                                         |"<<endl;
    Info<<"|}                                                                             |"<<endl;
    Info<<"|                                                                              |"<<endl;
    Info<<"|simulationType laminar;                                                       |"<<endl;
    Info<<"|<end of file>                                                                 |"<<endl;
    Info<<"|                                                                              |"<<endl;
    Info<<"--------------------------------------------------------------------------------"<<endl;
    Info<<endl;
}

singlePhaseTransportModel laminarTransport(U, phi);

autoPtr<incompressible::turbulenceModel> turbulence
(
    incompressible::turbulenceModel::New(U, phi, laminarTransport)
);

if (!LES)
{
    Info<<endl;
    Info<<"--------------------------------------------------------------------------------"<<endl;
    Info<<"|End of initialising dummy turbulence model                                    |"<<endl;
    Info<<"--------------------------------------------------------------------------------"<<endl;
    Info<<endl;
}
