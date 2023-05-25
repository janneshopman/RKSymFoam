scheme.stagePressure() = pCorr;

if (!scheme.lastStage())
{
    // Convection (UEqnSourceA) and diffusion (UEqnSourceB) are stored for later stages

    // If the scheme is in an implicit stage, these terms are already calculated
    if (scheme.explicitStageA())
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

        UEqnSourceAH = UEqnSourceA.H();
    }

    if (scheme.explicitStageB())
    {
        UEqnSourceB =
        (
            fv::gaussLaplacianScheme<vector, scalar>
            (
                mesh,
                midPoint<scalar>(mesh),
                fv::uncorrectedSnGrad<vector>(mesh)
            ).fvmLaplacian(nuf, U)
        );

        UEqnSourceBH = UEqnSourceB.H();
    }

    // Set stageSource of convection and diffusion
    // A and H functions account for possible implicit scheme
    // See also: https://openfoamwiki.net/index.php/OpenFOAM_guide/H_operator
    scheme.stageSourceA() = - UEqnSourceAH + UEqnSourceA.A()*U;
    scheme.stageSourceB() = - UEqnSourceBH + UEqnSourceB.A()*U;
}
