// Set up terms for Poisson equation
volScalarField rAU(scheme.C()/UEqn.A());

// Volumetric interpolation used, see references at top of RKSymFoam.C
surfaceScalarField rAUf(reverseLinear<scalar>(mesh).interpolate(rAU));

volVectorField HbyA(constrainHbyA(UEqn.H()/UEqn.A(), U, p));

// Volumetric interpolation used, see references at top of RKSymFoam.C
surfaceScalarField phiHbyA(reverseLinear<vector>(mesh).dotInterpolate(mesh.Sf(), HbyA));

constrainPressure(pCorr, U, phiHbyA, rAU);

// Set up Poisson equation for pCorr and solve
fvScalarMatrix pCorrEqn
(
    fv::gaussLaplacianScheme<scalar, scalar>
    (
        mesh,
        reverseLinear<scalar>(mesh),        // Scheme is not used
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
