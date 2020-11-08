import numpy as np

Sevent_list = []
Bevent_list = []
with open('events.txt') as f:
    for line in f:
        Sevent_list.append(line.split(',')[0])
        Bevent_list.append(line.split(',')[1].strip())

#print(Sevent_list)
nbins = len(Bevent_list)
bsyst = np.linspace(0.005,0.05,20)

def createline(list_):
    line = ''
    for l in list_:
        l_ = l + ' '
        line += l_
    return line

s_list = createline(Sevent_list)
b_list = createline(Bevent_list)

ofile = open("run.sh","w")
for ite in range(bsyst.size):
   syst_list =  [str(bsyst[ite]) for i in range(nbins)]
   syst_line = createline(syst_list)
   each_line = './ProfileCombination '
   each_line += s_list
   each_line += b_list
   each_line += syst_line
   ofile.write(each_line+'\n')
   #print(each_line)

ofile.close()
print(len(Sevent_list),' ',len(Bevent_list),' ',len(syst_list))
