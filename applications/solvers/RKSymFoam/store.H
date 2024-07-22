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
        UEqnSourceB =
        (
            fv::gaussLaplacianScheme<vector, scalar>
            (
                mesh,
                reverseLinear<scalar>(mesh),        // Scheme is not used
                fv::uncorrectedSnGrad<vector>(mesh)
            ).fvmLaplacian(nuf, U)
          + fvc::surfaceIntegrate
            (
                nuf
              * reverseLinear<tensor>(mesh).dotInterpolate
                (
                    mesh.Sf(),
                    dev2(T
                    (
                        fv::gaussGrad<vector>::gradf(linear<vector>(mesh).interpolate(U), "gradU")
                    ))
                )
            )
        );
    }

    // Set stageSource of convection and diffusion
    // A and H functions account for possible implicit scheme
    // See also: https://openfoamwiki.net/index.php/OpenFOAM_guide/H_operator
    scheme.stageSourceA() = - UEqnSourceA.H() + UEqnSourceA.A()*U;
    scheme.stageSourceB() = - UEqnSourceB.H() + UEqnSourceB.A()*U;
}
