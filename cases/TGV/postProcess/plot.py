import numpy as np
import matplotlib.pyplot as plt

runDir = '../validation'

cases = ['icoFoam', 'RKSymFoam']

nu = 0

# Make plots
for case in cases:

    # Retrieve kinetic energy of initial field
    EKinFile = open(runDir + '/' + case + '/log.setTaylorGreenFields', 'r')

    for line in EKinFile:

        words = line.split(' ')

        if words[0] == 'Initial':

            EKinInit = float(words[-1])/2    

    # Import run data
    EKinData = np.loadtxt(runDir + '/' + case + '/postProcessing/kineticEnergy/0/volFieldValue.dat', skiprows = 4)
    EKinData[:, 1] = EKinData[:, 1]/2

    EKinData = np.vstack(([0, EKinInit], EKinData))

    EKinAna = EKinInit*np.exp(-4.0*nu*EKinData[:, 0])

    plt.figure('EKin')
    plt.plot(EKinData[:, 0], EKinData[:, 1]/EKinAna, label=case)

    # Finalise plot
    if case == cases[-1]:

        plt.xlabel('t')
        plt.ylabel(r'$E_k/E_{k,Ana}$')
        plt.title('Kinetic energy loss in Taylor-Green vortex')
        plt.legend()
        plt.savefig('results/TGV.png')
