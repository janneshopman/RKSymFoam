// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline const Foam::dictionary& Foam::RungeKuttaScheme::dict() const
{
    return mesh_.solutionDict().subDict("RungeKutta");
}

inline Foam::label Foam::RungeKuttaScheme::stage() const
{
    return stage_;
}

inline Foam::label Foam::RungeKuttaScheme::outer() const
{
    return outer_;
}

inline Foam::label Foam::RungeKuttaScheme::inner() const
{
    return inner_;
}

inline Foam::label Foam::RungeKuttaScheme::nStages() const
{
    return a().size();
}

inline Foam::label Foam::RungeKuttaScheme::nOuter() const
{
    return nOuter_;
}

inline Foam::label Foam::RungeKuttaScheme::nInner() const
{
    return nInner_;
}

inline Foam::scalar Foam::RungeKuttaScheme::tolerance() const
{
    return tolerance_;
}

inline Foam::scalar& Foam::RungeKuttaScheme::residual()
{
    return residual_;
}

inline const Foam::scalar& Foam::RungeKuttaScheme::residual() const
{
    return residual_;
}

inline const Foam::volVectorField&
Foam::RungeKuttaScheme::K() const
{
    return K_;
}

inline const Foam::surfaceScalarField&
Foam::RungeKuttaScheme::Kf() const
{
    return Kf_;
}

inline Foam::PtrList<Foam::volScalarField>&
Foam::RungeKuttaScheme::stagePressures()
{
    return stagePressures_;
}

inline const Foam::PtrList<Foam::volScalarField>&
Foam::RungeKuttaScheme::stagePressures() const
{
    return stagePressures_;
}

inline Foam::PtrList<Foam::volVectorField>&
Foam::RungeKuttaScheme::stageSourcesA()
{
    return stageSourcesA_;
}

inline const Foam::PtrList<Foam::volVectorField>&
Foam::RungeKuttaScheme::stageSourcesA() const
{
    return stageSourcesA_;
}

inline Foam::PtrList<Foam::volVectorField>&
Foam::RungeKuttaScheme::stageSourcesB()
{
    return stageSourcesB_;
}

inline const Foam::PtrList<Foam::volVectorField>&
Foam::RungeKuttaScheme::stageSourcesB() const
{
    return stageSourcesB_;
}

inline Foam::PtrList<Foam::surfaceScalarField>&
Foam::RungeKuttaScheme::stageFaceSourcesA()
{
    return stageFaceSourcesA_;
}

inline const Foam::PtrList<Foam::surfaceScalarField>&
Foam::RungeKuttaScheme::stageFaceSourcesA() const
{
    return stageFaceSourcesA_;
}

inline Foam::PtrList<Foam::surfaceScalarField>&
Foam::RungeKuttaScheme::stageFaceSourcesB()
{
    return stageFaceSourcesB_;
}

inline const Foam::PtrList<Foam::surfaceScalarField>&
Foam::RungeKuttaScheme::stageFaceSourcesB() const
{
    return stageFaceSourcesB_;
}

inline Foam::volScalarField&
Foam::RungeKuttaScheme::stagePressure()
{
    return stagePressures_[stage_-1];
}

inline const Foam::volScalarField&
Foam::RungeKuttaScheme::stagePressure() const
{
    return stagePressures_[stage_-1];
}

inline Foam::volVectorField&
Foam::RungeKuttaScheme::stageSourceA()
{
    return stageSourcesA_[stage_-1];
}

inline const Foam::volVectorField&
Foam::RungeKuttaScheme::stageSourceA() const
{
    return stageSourcesA_[stage_-1];
}

inline Foam::volVectorField&
Foam::RungeKuttaScheme::stageSourceB()
{
    return stageSourcesB_[stage_-1];
}

inline const Foam::volVectorField&
Foam::RungeKuttaScheme::stageSourceB() const
{
    return stageSourcesB_[stage_-1];
}

inline Foam::surfaceScalarField&
Foam::RungeKuttaScheme::stageFaceSourceA()
{
    return stageFaceSourcesA_[stage_-1];
}

inline const Foam::surfaceScalarField&
Foam::RungeKuttaScheme::stageFaceSourceA() const
{
    return stageFaceSourcesA_[stage_-1];
}

inline Foam::surfaceScalarField&
Foam::RungeKuttaScheme::stageFaceSourceB()
{
    return stageFaceSourcesB_[stage_-1];
}

inline const Foam::surfaceScalarField&
Foam::RungeKuttaScheme::stageFaceSourceB() const
{
    return stageFaceSourcesB_[stage_-1];
}

inline Foam::scalar Foam::RungeKuttaScheme::A() const
{
    return a()[stage_-1][stage_-1];
}

inline Foam::scalar Foam::RungeKuttaScheme::B() const
{
    return b()[stage_-1][stage_-1];
}

inline Foam::scalar Foam::RungeKuttaScheme::C() const
{
    return sum(a()[stage_-1]);
}

inline Foam::Switch Foam::RungeKuttaScheme::explicitStageA() const
{
    return A() == 0.0;
}

inline Foam::Switch Foam::RungeKuttaScheme::explicitStageB() const
{
    return B() == 0.0;
}

inline Foam::Switch Foam::RungeKuttaScheme::explicitStage() const
{
    return explicitStageA() && explicitStageB();
}

inline Foam::Switch Foam::RungeKuttaScheme::implicitStageA() const
{
    return A() != 0.0;
}

inline Foam::Switch Foam::RungeKuttaScheme::implicitStageB() const
{
    return B() != 0.0;
}

inline Foam::Switch Foam::RungeKuttaScheme::implicitStage() const
{
    return implicitStageA() || implicitStageB();
}

inline Foam::Switch Foam::RungeKuttaScheme::firstStage() const
{
    return stage_ == 1;
}

inline Foam::Switch Foam::RungeKuttaScheme::lastStage() const
{
    return stage_ == nStages();
}

inline Foam::Switch Foam::RungeKuttaScheme::firstOuter() const
{
    return outer_ == 1;
}

inline Foam::Switch Foam::RungeKuttaScheme::lastOuter() const
{
    return outer_ == nOuter_;
}

inline Foam::Switch Foam::RungeKuttaScheme::firstInner() const
{
    return inner_ == 1;
}

inline Foam::Switch Foam::RungeKuttaScheme::lastInner() const
{
    return inner_ == nInner_;
}

inline Foam::Switch Foam::RungeKuttaScheme::solve() const
{
    return !(firstStage() && explicitStage());
}

inline Foam::Switch Foam::RungeKuttaScheme::innerConverged()
{
    return residual_ < tolerance_ && (nOuter_ > 1 || inner_ > 2);
}

inline Foam::Switch Foam::RungeKuttaScheme::stageLoop()
{
    stage_++;

    if (stage_ > nStages())
    {
        stage_ = 0;

        return false;
    }
    else
    {
        if (firstStage())
        {
            read();
        }

        U_ = U_.oldTime();
        phi_ = phi_.oldTime();

        p_ = stagePressures_[stage_-1];

        K_ *= 0.0;
        Kf_ *= 0.0;

        for (label i = 1; i < stage_; i++)
        {
            const scalar A(a()[stage_-1][i-1]);
            const scalar B(b()[stage_-1][i-1]);

            if (A != 0.0)
            {
                K_ +=
                    A*stageSourcesA_[i-1]
                  + B*stageSourcesB_[i-1];

                Kf_ +=
                    A*stageFaceSourcesA_[i-1]
                  + B*stageFaceSourcesB_[i-1];
            }
        }

        return true;
    }
}

inline Foam::Switch Foam::RungeKuttaScheme::outerLoop()
{
    outer_++;

    if (outer_ > nOuter())
    {
        outer_ = 0;

        return false;
    }
    else
    {
        if (firstOuter())
        {
        }

        return true;
    }
}

inline Foam::Switch Foam::RungeKuttaScheme::innerLoop()
{
    inner_++;

    if (inner_ > nInner() || innerConverged())
    {
        inner_ = 0;

        residual_ = 1E99;

        return false;
    }
    else
    {
        return true;
    }
}

template<class Type>
inline Foam::tmp<Foam::fvMatrix<Type>>
Foam::RungeKuttaScheme::D
(
    const GeometricField<Type, fvPatchField, volMesh>& field
) const
{
    tmp<fv::EulerDdtScheme<Type>> scheme
    (
        new fv::EulerDdtScheme<Type>(mesh_)
    );

    return scheme->fvmDdt(field);
}

// ************************************************************************* //
