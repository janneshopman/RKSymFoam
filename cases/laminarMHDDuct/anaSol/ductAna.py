import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import analyticalSolutions as ana

# dB = 0: Shercliff
# db = infinity, or e.g. 10E12: Hunt
dB = 0
Ha = 10
U0 = 1.0
nu = 0.001
rho = 1.0

# Number of terms in the series approximation of the analytical solution
kMax = 1000

# Set the duct geometry and mesh
a = 1.0
ny = 1001
dy = 2.0*a/ny
y = np.linspace(-a+dy/2.0, a-dy/2.0, ny)

b = 1.0
nz = 1001
dz = 2.0*b/nz
z = np.linspace(-b+dz/2.0, b-dz/2.0, nz)

#Eq. 26 of J.C.R. Hunt 1964 - MHD flow in rectangular ducts
UDimless, UChar, dpdx = ana.UAnaProfile(Ha, dB, U0, a, y, b, z, nu, rho, kMax)

yCenter = int(np.floor(len(y)/2.0))
zCenter = int(np.floor(len(z)/2.0))

#Rescale U
U = UDimless*UChar

print(f'Peak velocity: {np.max(U)}')
print(f'Analytical pressure gradient: {dpdx}')

np.savetxt("U_z0.txt", np.column_stack((y, U[zCenter, :])))
np.savetxt("U_y0.txt", np.column_stack((z, U[:, yCenter])))

plt.figure("anaProfiles")
plt.plot(y, U[zCenter, :], label="Hart")
plt.plot(z, U[:, yCenter], label="Side")
plt.legend()
plt.savefig("anaProfiles.pdf")
