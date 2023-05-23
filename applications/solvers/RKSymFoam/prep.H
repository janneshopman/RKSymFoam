fvVectorMatrix UEqnSourceA(U, dimVolume*dimVelocity/dimTime);
fvVectorMatrix UEqnSourceB(U, dimVolume*dimVelocity/dimTime);

volVectorField UEqnSourceAH(UEqnSourceA.H());
volVectorField UEqnSourceBH(UEqnSourceB.H());
