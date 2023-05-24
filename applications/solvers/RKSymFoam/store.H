scheme.stagePressure() = pCorr;

if (!scheme.lastStage())
{
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

    scheme.stageSourceA() = - UEqnSourceAH + UEqnSourceA.A()*U;
    scheme.stageSourceB() = - UEqnSourceBH + UEqnSourceB.A()*U;
}
