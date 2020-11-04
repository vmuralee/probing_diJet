import matplotlib.pyplot as plt
import numpy as np
import scipy.optimize as opt;

improve = True
def func(x,a,b,c,d,e):
    return a*x**4+b*x**3+c*x**2+d*x+e

def exp_func(x,a,b,c):
    return a+b*np.exp(-x*c)
s_list_JSS = []
s_list = []
with open('sigma_JSS.txt') as f:
    for line in f:
        val = line.split()
        s_list_JSS.append(float(val[-1]))
with open('sigma.txt') as fg:
    for line in fg:
        val = line.split()
        s_list.append(float(val[-1]))        
        
bsyst = np.linspace(0.005,0.05,50)
sigma = np.array(s_list)
sigma_JSS = np.array(s_list_JSS)

sig_imp = sigma_JSS/sigma
optimizedParameters, pcov = opt.curve_fit(func,bsyst,sig_imp);

if improve==True:
    plt.scatter(bsyst*100,sig_imp,label='sigma_JSS/sigma',c='navy',alpha=0.2)
    plt.plot(bsyst*100, func(bsyst, *optimizedParameters),c='g', label="fit")
else:
    plt.scatter(bsyst*100,sigma,label='without JSS')
    plt.scatter(bsyst*100,sigma_JSS,label='with JSS')


plt.xlabel('relative uncertainity [%]')

if improve==True:
    plt.ylabel('significance improvement')
else:
    plt.ylabel('significance')
plt.ylim(0.5)
plt.legend()

if improve==True:
    plt.savefig('../plots/sigimprovement.png')
else:
    plt.savefig('../plots/sigVsuncertainity.png')
plt.show()
