tmp<fvVectorMatrix> tUEqn;

const volVectorField gradpn
(
    fv::gaussGrad<scalar>::gradf(linear<scalar>(mesh).interpolate(pn), "gradpn")
);


if (scheme.explicitStage())
{
    tUEqn =
    (
        scheme.D(U)
      ==
        scheme.K()
      - scheme.C()*gradpn*pnPredCoef
      + scheme.C()*fvOptions(U)
    );
}
else
{
    UEqnSourceA =
    (
        #include "UEqnSourceA.H"
    );

    UEqnSourceB =
    (
        #include "UEqnSourceB.H"
    );

    tUEqn =
    (
        scheme.D(U)
      ==
        scheme.A()*UEqnSourceA
      + scheme.B()*UEqnSourceB
      + scheme.K()
      - scheme.C()*gradpn*pnPredCoef
      + scheme.C()*fvOptions(U)
    );
}

fvVectorMatrix& UEqn = tUEqn.ref();

fvOptions.constrain(UEqn);
