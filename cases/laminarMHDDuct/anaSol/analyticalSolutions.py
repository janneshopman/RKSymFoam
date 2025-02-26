import numpy as np

#Eq. 26 of J.C.R. Hunt 1964 - MHD flow in rectangular ducts
def UAnaProfile(Ha, dB, U0, a, y, b, z, nu, rho, kMax):
    l = b/a         #Channel width/height ratio
    xi = z/a        #Scaled z-coordinates (-l,l)
    eta = y/a       #Scaled y-coordinates (-1,1)

    UDimless = np.zeros((len(xi), len(eta)))

    for k in range(kMax+1):
        alpha_k = (k + 0.5)*np.pi/l

        N = (Ha**2.0 + 4.0*alpha_k**2.0)**0.5

        r1_k = 0.5*(-Ha + N)
        r2_k = 0.5*( Ha + N)

        U2 =                                                                    \
        (                                                                       \
            (dB*r2_k + (1.0 - np.exp(-2.0*r2_k)) / (1.0 + np.exp(-2.0*r2_k)))   \
          * (np.exp(-r1_k*(1.0-eta)) + np.exp(-r1_k*(1.0+eta))) / 2.0           \
          / (                                                                   \
                (1.0 + np.exp(-2.0*r1_k))/2.0 * dB * N                          \
              + (1.0 + np.exp(-2.0*(r1_k+r2_k))) / (1.0 + np.exp(-2.0*r2_k))    \
            )                                                                   \
        )

        U3 =                                                                    \
        (                                                                       \
            (dB*r1_k + (1.0 - np.exp(-2.0*r1_k)) / (1.0 + np.exp(-2.0*r1_k)))   \
          * (np.exp(-r2_k*(1.0-eta)) + np.exp(-r2_k*(1.0+eta))) / 2.0           \
          / (                                                                   \
                (1.0 + np.exp(-2.0*r2_k))/2.0 * dB * N                          \
              + (1.0 + np.exp(-2.0*(r2_k+r1_k))) / (1.0 + np.exp(-2.0*r1_k))    \
            )                                                                   \
        )

        UDimless += np.outer((2.0*(-1.0)**k * np.cos(alpha_k*xi))/(l*alpha_k**3.0), (1.0 - U2 - U3))

        #Scaling for Shercliff case
        if (dB == 0.0):

            U4 =                                                                    \
            (                                                                       \
                1.0                                                                 \
              - N/(2.0*alpha_k**2.0)                                                \
              * (                                                                   \
                    (1.0 + np.exp(-2.0*N))/(1.0 - np.exp(-2.0*N))                   \
                  - np.exp(Ha - N) * (1.0 + np.exp(-2.0*Ha))/(1.0 - np.exp(-2.0*N)) \
                )                                                                   \
            )

            if (k == 0):
                U4sum =  1.0/alpha_k**4.0 * U4
            else:
                U4sum += 1.0/alpha_k**4.0 * U4

    if (dB == 0.0):
        #Scaling for Shercliff case
        dpdx = -U0/U4sum*l**2.0/2.0 *rho*nu/a**2.0

        UChar = -dpdx /rho/nu*a**2.0

    else:
        #Scaling for Hunt case
        dA = np.zeros((len(z), len(y)))

        zl = -b
        for i in range(len(z)):
            dz = (z[i]-zl)*2.0
            zl = zl+dz
        
            yl = -a
            for j in range(len(y)):
                dy = (y[j]-yl)*2.0
                yl = yl+dy

                dA[i,j] = dz*dy

        UChar = U0*(4.0*a*b)/np.sum(UDimless*dA)

        dpdx = -UChar *rho*nu/a**2.0

    return UDimless, UChar, dpdx
