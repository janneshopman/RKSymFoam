import numpy as np
import matplotlib.pyplot as plt

runDir = 'refRuns'

solvers = \
[ \
    'icoFoam', 'RKSymFoam_DNS_BackwardEuler', 'RKSymFoam_DNS_Kutta', \
    'pimpleFoam', 'RKSymFoam_LES_BackwardEuler', 'RKSymFoam_LES_Kutta' \
]
lineStyles = ['-', '-', '-', '--', '--', '--']

timeDir = '60'

ReTau = 180

dataSets = ['UMean', 'Urms', 'Vrms', 'Wrms', 'k']

refData = np.loadtxt('refData/Chan180_S2a_basic_few.txt', skiprows = 36, usecols = (0, 1, 4, 5, 6, 9))

# Make plots
for iSolver, solver in enumerate(solvers):

    fileNameBase = runDir + '/' + solver + '/graphs/' + timeDir + '/'

    # Import run data
    UMeanData = np.loadtxt(fileNameBase + 'Uf.xy')
    UTau = np.sqrt(UMeanData[0, 1]/UMeanData[0, 0]/ReTau)

    yPlus = UMeanData[:, 0]*ReTau
    UMean = UMeanData[:, 1]/UTau
    uRMS  = np.loadtxt(fileNameBase + 'u.xy', usecols = 1)/UTau**2
    vRMS  = np.loadtxt(fileNameBase + 'v.xy', usecols = 1)/UTau**2
    wRMS  = np.loadtxt(fileNameBase + 'w.xy', usecols = 1)/UTau**2
    k     = np.loadtxt(fileNameBase + 'k.xy', usecols = 1)/UTau**2

    runData = np.column_stack((UMean, uRMS, vRMS, wRMS, k))

    # Plot run data
    for iDataSet, dataSet in enumerate(dataSets):

        label = solver + r', $u_{\tau}=$' + str(round(UTau, 2)) if (iDataSet == 0) else solver

        plt.figure(dataSet)
        plt.plot(yPlus, runData[:, iDataSet], lineStyles[iSolver], label=label)

        # Plot reference and finalise 
        if solver == solvers[-1]:

            plt.plot(refData[:, 0], refData[:, iDataSet+1], '-k', label='Vreman')
            plt.xlabel(r'$y^+$')
            plt.ylabel(dataSet)
            plt.title(r'Channel flow at $Re_{\tau}=$' + str(ReTau))
            plt.legend()
            plt.savefig('results/ref_' + dataSet + '.png')
