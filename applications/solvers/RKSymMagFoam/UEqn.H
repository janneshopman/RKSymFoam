tmp<fvVectorMatrix> tUEqn;

// Volumetric interpolation used, see references at top of RKSymFoam.C
const volVectorField gradpn
(
    fv::gaussGrad<scalar>::gradf(linear<scalar>(mesh).interpolate(pn), "gradpn")
);

// Set up a UEqn matrix, containing:
// D: time derivative of U
// K: weighted sum of previous stage (explicit) convection and diffusion terms
// pnPredCoef weighted gradient of previous time step pressure 
// pnPredCoef = 0 equals Chorin method
// pnPredCoef = 1 equals Van Kan method
// possible fvOptions source term
// and finally, in case of implicit scheme:
// implicit convection and diffusion terms
if (scheme.explicitStage())
{
    tUEqn =
    (
        scheme.D(U)
      ==
        scheme.K()
      - scheme.C()*gradpn*pnPredCoef
      + scheme.C()*(J^B0)/rho
      + scheme.C()*fvOptions(U)
    );
}
else
{
    UEqnSourceA =
    (
      - fv::gaussConvectionScheme<vector>
        (
            mesh,
            phi,
            midPoint<vector>(mesh)
        ).fvmDiv(phi, U)
    );

    if (noModelNewtonian)
    {
        UEqnSourceB =
        (
            fv::gaussLaplacianScheme<vector, scalar>
            (
                mesh,
                reverseLinear<scalar>(mesh),        // Scheme is not used
                fv::uncorrectedSnGrad<vector>(mesh)
            ).fvmLaplacian(nuf, U)
        );
    }
    else
    {
        UEqnSourceB = -turbulence->divDevRhoReff(U);
    }

    tUEqn =
    (
        scheme.D(U)
      ==
        scheme.A()*UEqnSourceA
      + scheme.B()*UEqnSourceB
      + scheme.K()
      - scheme.C()*gradpn*pnPredCoef
      + scheme.C()*(J^B0)/rho
      + scheme.C()*fvOptions(U)
    );
}

fvVectorMatrix& UEqn = tUEqn.ref();

fvOptions.constrain(UEqn);
