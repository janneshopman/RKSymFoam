// Set up source term for Poisson equation
volScalarField rAU(scheme.C()/UEqn.A());

// Volumetric interpolation used, see references at top of RKSymFoam.C
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

// Volumetric interpolation used, see references at top of RKSymFoam.C
surfaceScalarField phiHbyA
(
    "phiHbyA",
    rAUf
  / scheme.C()
  * reverseLinear<vector>(mesh).dotInterpolate(mesh.Sf(), HbyA*UEqn.A())
);

constrainPressure(pCorr, U, phiHbyA, rAU);

// Set up Poisson equation for pCorr and solve
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

// Slightly adjusted p reference value to account for the fact that pCorr is solved
// pRefCell = -1 if pRefCell is not on processor domain partition, avoid evaluating pn[-1]
// setReference function only sets pRefValue if pRefCell >= 0
double pCorrRefValue = (pRefCell >= 0) ? pRefValue - pnPredCoef*pn[pRefCell] : 0.0;
pCorrEqn.setReference(pRefCell, pCorrRefValue);

const word solver
(
    scheme.lastInner() && scheme.lastOuter()
  ? word(pCorr.name() + "Final")
  : pCorr.name()
);

const solverPerformance& perf = pCorrEqn.solve(mesh.solver(solver));

scheme.residual() = perf.initialResidual();

// Correct fields with updated pCorr
phi = phiHbyA - mesh.magSf()*rAUf*fv::uncorrectedSnGrad<scalar>(mesh).snGrad(pCorr);

#include "continuityErrs.H"

// Volumetric interpolation used, see references at top of RKSymFoam.C
U = HbyA - rAU*fv::gaussGrad<scalar>::gradf(linear<scalar>(mesh).interpolate(pCorr), "gradpCorr");
U.correctBoundaryConditions();

fvOptions.correct(U);

p = pnPredCoef*pn + pCorr;
