import numpy as np
import corner

f = open('Pis_MCMC.dat')
lines = f.readlines()

listA = []
listMu = []
listSigma = []

for x in lines:
    listA.append(x.split(' ')[0])
    listMu.append(x.split(' ')[1])
    listSigma.append(x.split(' ')[2])
f.close()

A = []
Mu = []
Sigma = []

for item in listA:
    A.append(float(item))
    
for item in listMu:
    Mu.append(float(item))
    
for item in listSigma:
    Sigma.append(float(item))
    

A_avg = 0
A_sum = 0

Mu_avg = 0
Mu_sum = 0

Sigma_avg = 0
Sigma_sum = 0

for i in range(len(A)):
    A_sum += A[i]
    A_avg = A_sum/len(A)

for i in range(len(Mu)):
    Mu_sum += Mu[i]
    Mu_avg = Mu_sum/len(Mu)
    
for i in range(len(Sigma)):
    Sigma_sum += Sigma[i]
    Sigma_avg = Sigma_sum/len(Sigma)

Parameters = [A_avg, Mu_avg, Sigma_avg]
print(Parameters)

A_std = np.std(A)
Mu_std = np.std(Mu)
Sigma_std = np.std(Sigma)


Test = np.vstack((A,Mu,Sigma))

print(np.cov(Test))
print(np.corrcoef(Test))

figure = corner.corner(Test.T)
figure.savefig("piCorner.pdf")


#print(cov)



