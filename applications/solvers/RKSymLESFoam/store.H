scheme.stagePressure() = pCorr;

if (!scheme.lastStage())
{
    if (scheme.explicitStageA())
    {
        UEqnSourceA =
        (
            #include "UEqnSourceA.H"
        );

        UEqnSourceAH = UEqnSourceA.H();
    }

    if (scheme.explicitStageB())
    {
        UEqnSourceB =
        (
            #include "UEqnSourceB.H"
        );

        UEqnSourceBH = UEqnSourceB.H();
    }

    scheme.stageSourceA() = - UEqnSourceAH + UEqnSourceA.A()*U;
    scheme.stageSourceB() = - UEqnSourceBH + UEqnSourceB.A()*U;
}
