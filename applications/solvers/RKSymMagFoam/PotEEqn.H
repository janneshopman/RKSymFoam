// Volumetric interpolation used, see references at top of RKSymFoam.C
const volVectorField gradPotEn
(
    fv::gaussGrad<scalar>::gradf(linear<scalar>(mesh).interpolate(PotEn), "gradPotEn")
);

J = sigma * ((U^B0) - PotEnPredCoef*gradPotEn);

surfaceScalarField Jf(reverseLinear<vector>(mesh).dotInterpolate(mesh.Sf()/mesh.magSf(), J));

// Set up Poisson equation for PotECorr and solve
fvScalarMatrix PotECorrEqn
(
    fv::gaussLaplacianScheme<scalar, scalar>
    (
        mesh,
        reverseLinear<scalar>(mesh),
        fv::uncorrectedSnGrad<scalar>(mesh)
    ).fvmLaplacian(sigmaf, PotECorr)

  - fvc::surfaceIntegrate(Jf*mesh.magSf())
);

// Slightly adjusted PotE reference value to account for the fact that pCorr is solved
// pRefCell = -1 if PotERefCell is not on processor domain partition, avoid evaluating PotEn[-1]
// setReference function only sets PotERefValue if PotERefCell >= 0
double PotECorrRefValue = (PotERefCell >= 0) ? PotERefValue - PotEnPredCoef*PotEn[PotERefCell] : 0.0;
PotECorrEqn.setReference(PotERefCell, PotECorrRefValue);
PotECorrEqn.solve(mesh.solver("PotECorr"));

// Correct fields with updated PotECorr
Jf -= sigma*fv::uncorrectedSnGrad<scalar>(mesh).snGrad(PotECorr);

// Volumetric interpolation used, see references at top of RKSymFoam.C
// Interpolator not implemented in OpenFOAM
J = fvc::faceToCellInterpolate(Jf);
J.correctBoundaryConditions();

PotE = PotEnPredCoef*PotEn + PotECorr;
