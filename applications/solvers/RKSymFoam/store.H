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
    }

    if (scheme.explicitStageB())
    {
        if (noModelNewtonian)
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
        }
        else
        {
            UEqnSourceB = -turbulence->divDevRhoReff(U);
        }
    }

    // Set stageSource of convection and diffusion
    // A and H functions account for possible implicit scheme
    // See also: https://openfoamwiki.net/index.php/OpenFOAM_guide/H_operator
    scheme.stageSourceA() = - UEqnSourceA.H() + UEqnSourceA.A()*U;
    scheme.stageSourceB() = - UEqnSourceB.H() + UEqnSourceB.A()*U;
}
