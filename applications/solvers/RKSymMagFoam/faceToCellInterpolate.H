#include "fvMesh.H"
#include "extrapolatedCalculatedFvPatchFields.H"
#include "surfaceFields.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Symmetry-preserving face-to-cell interpolation, see references in RKSymMagFoam.C
volVectorField
faceToCellInterpolate
(
    const surfaceScalarField& Issf
)
{
    const fvMesh& mesh = Issf.mesh();

    tmp<volVectorField> Otvf
    (
        new volVectorField
        (
            IOobject
            (
                "faceToCellInterpolate("+Issf.name()+')',
                Issf.instance(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensionedVector(Issf.dimensions(), Zero),
            extrapolatedCalculatedFvPatchField<vector>::typeName
        )
    );
    volVectorField& Orvf = Otvf.ref();
    Field<vector>& Oivf = Orvf.primitiveFieldRef();

    const labelUList& owner = mesh.owner();
    const labelUList& neighbour = mesh.neighbour();

    const surfaceVectorField SfHat(mesh.Sf()/mesh.magSf());
    const Field<scalar>& Iissf = Issf;

    forAll(owner, facei)
    {
        const double faceVolOwn = ((mesh.Cf()[facei] - mesh.C()[owner[facei]]) & mesh.Sf()[facei]);
        const double faceVolNei = ((mesh.C()[neighbour[facei]] - mesh.Cf()[facei]) & mesh.Sf()[facei]);

        Oivf[owner[facei]] += Iissf[facei]*SfHat[facei]*faceVolOwn;
        Oivf[neighbour[facei]] += Iissf[facei]*SfHat[facei]*faceVolNei;
    }

    forAll(mesh.boundary(), patchi)
    {
        const labelUList& pFaceCells = mesh.boundary()[patchi].faceCells();

        const fvsPatchField<vector>& pCf = mesh.Cf().boundaryField()[patchi];
        const fvsPatchField<vector>& pSf = mesh.Sf().boundaryField()[patchi];
        const fvsPatchField<vector>& pSfHat = SfHat.boundaryField()[patchi];
        const fvsPatchField<scalar>& Ipssf = Issf.boundaryField()[patchi];

        forAll(mesh.boundary()[patchi], faceiInPatch)
        {
            const double boundaryFaceVol = ((pCf[faceiInPatch] - mesh.C()[pFaceCells[faceiInPatch]]) & pSf[faceiInPatch]);

            Oivf[pFaceCells[faceiInPatch]] += Ipssf[faceiInPatch]*pSfHat[faceiInPatch]*boundaryFaceVol;
        }
    }

    Oivf /= mesh.Vsc();

    Orvf.correctBoundaryConditions();

    return Otvf;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
