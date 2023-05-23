    fv::gaussLaplacianScheme<vector, scalar>
    (
        mesh,
        midPoint<scalar>(mesh),
        fv::uncorrectedSnGrad<vector>(mesh)
    ).fvmLaplacian(nuf, U)
