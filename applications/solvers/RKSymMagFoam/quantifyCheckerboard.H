#include "fvMesh.H"
#include "surfaceFields.H"
#include "volFields.H"
#include "linear.H"
#include "reverseLinear.H"
#include "uncorrectedSnGrad.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

surfaceScalarField
createOmegaS
(
    const fvMesh& mesh
)
{
    surfaceScalarField OmegaS
    (
        IOobject
        (
            "OmegaS",
            mesh.time().constant(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar(dimensionSet(0, 3, 0, 0, 0, 0, 0), -1)
    );

    forAll(mesh.owner(), facei)
    {
        OmegaS[facei] = mesh.magSf()[facei] / mesh.nonOrthDeltaCoeffs()[facei];
    }

    forAll(OmegaS.boundaryField(), patchi)
    {
        const fvsPatchField<vector>& pCf = mesh.Cf().boundaryField()[patchi];
        const fvsPatchField<vector>& pSf = mesh.Sf().boundaryField()[patchi];
        const labelUList& pFaceCells = mesh.boundary()[patchi].faceCells();

        forAll(mesh.boundary()[patchi], faceiInPatch)
        {
            OmegaS.boundaryFieldRef()[patchi][faceiInPatch] =
                (pCf[faceiInPatch] - mesh.C()[pFaceCells[faceiInPatch]]) & pSf[faceiInPatch];
        }
    }

    return OmegaS;
}


// Checkerboard coefficient is ratio of norms of face gradient and collocated gradient
// See references at top of RKSymMagFoam.C
double
quantifyCheckerboard
(
    const volScalarField& Ivsf,
    const surfaceScalarField& OmegaS
)
{
    const fvMesh& mesh = Ivsf.mesh();

    // Norm of face gradient
    const volVectorField GcIvsf = fv::gaussGrad<scalar>::gradf(linear<scalar>(mesh).interpolate(Ivsf), "GcIvsf");
    const volScalarField GcIvsfSq = GcIvsf & GcIvsf;
    dimensionedScalar normGc(pow2(GcIvsf.dimensions())*dimVolume, gSum((mesh.V() * GcIvsfSq.internalField())())); 

    // Norm of collocated gradient
    const surfaceScalarField GIvsf = fv::uncorrectedSnGrad<scalar>(mesh).snGrad(Ivsf);
    dimensionedScalar normGIvsfBf
    (
        pow2(GIvsf.dimensions())*dimVolume, 
        gSum(GIvsf.boundaryField() * OmegaS.boundaryField() * GIvsf.boundaryField())
    );
    dimensionedScalar normGIvsfIf(pow2(GIvsf.dimensions())*dimVolume, gSum((GIvsf * OmegaS * GIvsf)()));
    dimensionedScalar normG = normGIvsfIf + normGIvsfBf;

    // Ratio of norms
    const double Ccb = (normG.value() == 0.0) ? 0.0 : 1.0 - normGc.value()/normG.value();

    return Ccb;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
