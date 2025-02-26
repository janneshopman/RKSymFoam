import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import sys
import os

BC = sys.argv[1]

scriptDir = os.path.dirname(os.path.abspath(__file__))

UAna_y0 = np.loadtxt(os.path.join(scriptDir, "reference", f"{BC}_Ha10_U_y0.txt"))
UAna_z0 = np.loadtxt(os.path.join(scriptDir, "reference", f"{BC}_Ha10_U_z0.txt"))

U_y0 = np.loadtxt(os.path.join(scriptDir, "..", "postProcessing", "lineSample", "100", "horizontal_U.xy"))
U_z0 = np.loadtxt(os.path.join(scriptDir, "..", "postProcessing", "lineSample", "100", "vertical_U.xy"))

plt.figure("compare_solutions")
plt.figure(f"U Profiles {BC}'s case")
plt.plot(U_z0[:, 0], U_z0[:, 1], label="y-profile(z=0)_RKSMF")
plt.plot(U_y0[:, 0], U_y0[:, 1], label="z-profile(y=0)_RKSMF")
plt.plot(UAna_z0[:, 0], UAna_z0[:, 1], ':k')
plt.plot(UAna_y0[:, 0], UAna_y0[:, 1], ':k')
plt.plot([], [], ':k', label='Analytical')
plt.xlabel("y,z")
plt.ylabel("U", rotation=0, labelpad=20)
plt.savefig(os.path.join(scriptDir, "results/UProfiles.pdf"))
