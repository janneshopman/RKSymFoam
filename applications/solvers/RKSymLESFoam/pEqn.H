volScalarField rAU(scheme.C()/UEqn.A());

surfaceScalarField rAUf
(
    "rAUf",
    scheme.C()
  / reverseLinear<scalar>(mesh).interpolate(UEqn.A())
);

volVectorField HbyA("HbyA", U);

HbyA = UEqn.H()/UEqn.A();

UEqnSourceAH = UEqnSourceA.H();
UEqnSourceBH = UEqnSourceB.H();

surfaceScalarField phiHbyA
(
    "phiHbyA",
    rAUf
  / scheme.C()
  * reverseLinear<vector>(mesh).dotInterpolate(mesh.Sf(), HbyA*UEqn.A())
);

constrainPressure(pCorr, U, phiHbyA, rAU);

fvScalarMatrix pCorrEqn
(
    fv::gaussLaplacianScheme<scalar, scalar>
    (
        mesh,
        midPoint<scalar>(mesh),
        fv::uncorrectedSnGrad<scalar>(mesh)
    ).fvmLaplacian(rAUf, pCorr)
 ==
    fvc::div(phiHbyA)
);

pCorrEqn.setReference(pRefCell, pRefValue-pn[pRefCell]);

const word solver
(
    scheme.lastInner() && scheme.lastOuter()
  ? word(pCorr.name() + "Final")
  : pCorr.name()
);

const solverPerformance& perf = pCorrEqn.solve(mesh.solver(solver));

scheme.residual() = perf.initialResidual();

p = pnPredCoef*pn + pCorr;

const volVectorField gradpCorr
(
    fv::gaussGrad<scalar>::gradf(linear<scalar>(mesh).interpolate(pCorr), "gradpCorr")
);

U = HbyA - rAU*gradpCorr;

U.correctBoundaryConditions();

fvOptions.correct(U);

if (scheme.lastInner() || scheme.innerConverged())
{
    phi = phiHbyA - mesh.magSf()*rAUf*fv::uncorrectedSnGrad<scalar>(mesh).snGrad(pCorr);
}
