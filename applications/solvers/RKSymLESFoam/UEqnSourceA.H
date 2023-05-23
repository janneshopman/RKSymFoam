  - fv::gaussConvectionScheme<vector>
    (
        mesh,
        phi,
        midPoint<vector>(mesh)
    ).fvmDiv(phi, U)
