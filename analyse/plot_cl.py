import matplotlib.pyplot as plt
import numpy as np
import scipy.optimize as opt;
import math
import sys


improve = sys.argv[1]
cat = sys.argv[2]
def func(x,a,b,c,d,e):
    return a*x**4+b*x**3+c*x**2+d*x+e

def exp_func(x,a,b,c):
    return a+b*np.exp(-x*c)

def check_nonzero(vals):
    good_list = []
    for i in range(len(vals)-1):
        val = vals[i]
        if (val == -0):
            val = 0.5*(vals[i-1]+vals[i+1])
        good_list.append(val)
    if(vals[-1]== -0):
        good_list.append(good_list[-1])
    else:
        good_list.append(vals[-1])
    return good_list

s_list_JSS = []
s_list = []
with open('sigma_JSS.txt') as f:
    for line in f:
        val = line.split()
        s_list_JSS.append(float(val[-1]))
with open('sigma.txt') as fg:
    for line in fg:
        val = line.split()
        #print(val)
        s_list.append(float(val[-1]))        

s_list = check_nonzero(s_list)
s_list_JSS = check_nonzero(s_list_JSS)

bsyst = np.linspace(0.005,0.1,20)#np.array([i/100 for i in range(1,21)])
sigma = np.array(s_list)
sigma_JSS = np.array(s_list_JSS)

sig_imp = sigma_JSS/sigma
optimizedParameters, pcov = opt.curve_fit(func,bsyst,sig_imp);
optimizedParameters1, pcov1 = opt.curve_fit(exp_func,bsyst,sigma_JSS);
optimizedParameters2, pcov2 = opt.curve_fit(exp_func,bsyst,sigma);

sig_imp_fit = exp_func(bsyst,*optimizedParameters1)/exp_func(bsyst,*optimizedParameters2)
if improve=='i':
    #plt.scatter(bsyst*100,sig_imp,label='sigma_JSS/sigma',c='g')
    plt.plot(bsyst*100, sig_imp_fit,c='g', label="sigma_JSS/sigma")
    #plt.plot(bsyst*100,func(bsyst,*optimizedParameters),c='g', label="sigma_JSS/sigma")
else:
    #plt.scatter(bsyst*100,sigma,label='without JSS')
    #plt.scatter(bsyst*100,sigma_JSS,label='with JSS')
    plt.plot(bsyst*100,exp_func(bsyst,*optimizedParameters1),c='b', label="wtih JSS")
    plt.plot(bsyst*100,exp_func(bsyst,*optimizedParameters2),c='r', label="wtihout JSS")

plt.xlabel('relative uncertainity [%]')

if improve=='i':
    plt.ylabel('significance improvement')
else:
    plt.ylabel('significance')
plt.ylim(0.5)
plt.legend()

if improve=='i':
    plt.savefig('../plots/sigimprovement_'+cat+'.png')
else:
    plt.savefig('../plots/sigVsuncertainity_'+cat+'.png')
plt.show()
