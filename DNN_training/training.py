#########################################
### DNN Training ########################
# vinaya.krishna@cern.ch#################
#########################################

import os
import ROOT
from tensorflow import keras
from sklearn.model_selection import train_test_split
import numpy as np
import copy
import pandas as pd
import matplotlib.pyplot as plt
from Data2Array import Data2Array,Array2Data
import argparse
from sklearn.metrics import roc_curve,roc_auc_score
from sklearn.preprocessing import OneHotEncoder
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.impute import SimpleImputer
#from sklearn.model_selection import RandomizedSearchCV

parser = argparse.ArgumentParser(description='Training DNN model')
parser.add_argument('--predict',type=bool,help='predict samples',default=False)
parser.add_argument('--epoch',type=int,help='No. of epochs',default=10)
parser.add_argument('--cat',type=str,help='select categories',default='qg')
parser.add_argument('--path',type=str,help='pathe of samples',default='../data/Ntuples')
parser.add_argument('--JSS',type=bool,help='add JSS parameter',default=False)
parser.add_argument('--csv',type=bool,help='save csv file',default=False)
parser.add_argument('--outfile',type=str,help='output root file',default='test')
parser.add_argument('--cut',type=int,help='Invariant mass cut',default=(0,10000),nargs=2)
args = parser.parse_args()
if(args.cat not in ['qq','qg','gg']):
    print('Please choose either of qq,qg or gg')
    exit(-1)

cat_files = dict({'qq':['pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root','pp2coloron2_qq-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root'],
                  'qg':['pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root','pp2ExUp2_ug-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root'],
                  'gg':['pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root','pp2octet2_gg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root']
})
filenames = cat_files[args.cat]

prefix = '_'+args.cat

labels = ['ptJ1','ptJ2','invmass','DRJ1J2','etaJ1','etaJ2']
if(args.JSS):
    prefix = prefix+'JSS'
    print('Using JSS paramets')
    labels = labels+['pTDJ1','pTDJ2','LHAJ1','LHAJ2','e05J1','e05J2','s2J1','s2J2','pmJ1','pmJ2','tmJ1','tmJ2','widthJ1','widthJ2','girthJ1','girthJ2']

m_cut = args.cut
if(m_cut[0]==0 and m_cut[1] == 10000):
    prefix = prefix+'_Nocut'
    print('No invariant mass cut')
else:
    prefix = prefix+'minv_'+str(m_cut[0])+'_'+str(m_cut[1])
    print('Invariant mass cut of ',m_cut[0],' < m_inv < ',m_cut[1],' applied')
data = Data2Array(args.path,filenames,labels,m_cut)
data_ar = data.load_df()

# Export to csv file
savefileAs = 'dijet_data_'+args.cat
if(args.JSS):
    savefileAs = 'dijet_data_JSS_'+args.cat
if(args.csv):
    data.save_csv(data_ar,savefileAs)

    
#Creating Training,Test and Validation samples
traindataset_full,testdataset = train_test_split(data_ar,test_size=0.3,random_state=42)
traindataset,valdataset = train_test_split(traindataset_full,test_size=0.2,random_state=42)
trainData = copy.deepcopy(traindataset)
valData = copy.deepcopy(valdataset)


X_train,X_val = trainData.drop('target',axis=1).to_numpy(dtype='float64'),valData.drop('target',axis=1).to_numpy(dtype='float64')
y_train,y_val = trainData['target'].to_numpy(dtype='float64'),valData['target'].to_numpy(dtype='float64')
X_test,y_test = testdataset.drop('target',axis=1).to_numpy(dtype='float64'),testdataset['target']

#TransForming Data
num_pipeline = Pipeline([
    ('imputer',SimpleImputer(strategy="median")),
    ('std_scaler',StandardScaler()),
])
encoder = OneHotEncoder()

X_train_tr,X_val_tr,X_test_tr = num_pipeline.fit_transform(X_train),num_pipeline.fit_transform(X_val),num_pipeline.fit_transform(X_test)
y_train_tr,y_val_tr = encoder.fit_transform(trainData[['target']]).toarray(),encoder.fit_transform(valData[['target']]).toarray()

print('Label dims: ',X_train_tr.shape,'   Output dims: ',y_train_tr.shape)

#DNN model
def build_model(n_hidden=1,n_neurons=30,learning_rate=3e-3,input_shapes=[8]):
    model = keras.models.Sequential()
    model.add(keras.layers.InputLayer(input_shape=input_shapes))
    for layer in range(n_hidden):
        model.add(keras.layers.Dense(n_neurons,activation='relu',kernel_initializer="he_normal"))
    he_avg_init = keras.initializers.VarianceScaling(scale=2.,mode='fan_avg',distribution='uniform')
    model.add(keras.layers.Dense(2,activation='sigmoid',kernel_initializer=he_avg_init))
    optimizer = keras.optimizers.SGD(lr=learning_rate)
    model.compile(loss="sparse_categorical_crossentropy",optimizer=optimizer,metrics=['accuracy'])
    return model

l_rate =  0.02023431713920855
if(args.JSS):
    l_rate = 0.01304080507013371
#Tuned Model
model = build_model(n_hidden=2,n_neurons=347,learning_rate=l_rate,input_shapes=len(labels))
if(args.JSS):
    model = build_model(n_hidden=3,n_neurons=475,learning_rate=l_rate,input_shapes=len(labels))

print('training model for '+prefix+' ..............................')
#checkpoint_cb = keras.callbacks.ModelCheckpoint("keras_model.h5")
prefit = model.fit(X_train_tr,y_train,epochs=args.epoch,validation_data=(X_val_tr,y_val),callbacks=[keras.callbacks.EarlyStopping(patience=10)])

model_dir = "../output/saved_models/model_"+prefix
try:
    os.mkdir(model_dir)
except:  
    print('Folder already created')
model.save(model_dir+"/keras_model_"+prefix+".h5")


#Learning Curves
pd.DataFrame(prefit.history).plot(figsize=(8,5))
plt.figure(1)
plt.grid(True)
plt.gca().set_ylim(0,1)
plt.savefig(model_dir+"/learning_curve.png")
#plt.show()

rootfile = model_dir+'/'+args.outfile+prefix+'.root'

#Training ROC
y_tr_pred = model.predict(X_train_tr)
plt.figure(2)
plt.plot(figsize=(6,5))
plt.grid(True)
y_out_tr = np.array(y_train,dtype='float32')
fpr_tr,tpr_tr,th_tr = roc_curve(y_true=y_out_tr,y_score=y_tr_pred[:,1])
auc = roc_auc_score(y_out_tr,y_tr_pred[:,1])
plt.plot(tpr_tr,1-fpr_tr)
plt.text(0.15,0.35,f'auc value: {auc}')
plt.xlabel('efficiency')
plt.ylabel('misidentification')
plt.savefig(model_dir+'/training_ROC_'+prefix+'.png')

file_ = ROOT.TFile(rootfile,"update")
gr_tr = ROOT.TGraph(fpr_tr.size,tpr_tr,1-fpr_tr)
gr_tr.Write("training_roc")
file_.Close()

#Prediction
if(args.predict):
    y_pred = model.predict(X_test_tr)
    
#Storing ntuple
if(args.predict):
    print('Creating outfile file '+rootfile+' .......................................')
    tfile = Array2Data(y_pred,testdataset,rootfile)
    tfile.SigTree()
    tfile.BkgTree()

#ROC
if(args.predict):
    plt.figure(3)
    plt.plot(figsize=(6,5))
    plt.grid(True)
    y_out = np.array(y_test,dtype='float32')
    fpr,tpr,th = roc_curve(y_true=y_out,y_score=y_pred[:,1])
    auc = roc_auc_score(y_out,y_pred[:,1])
    plt.plot(tpr,1-fpr)
    plt.text(0.15,0.35,f'auc value: {auc}')
    plt.xlabel('efficiency')
    plt.ylabel('misidentification')
    plt.savefig(model_dir+'/ROC_'+prefix+'.png')
    #plt.show()

    #Storing ROC
    file_ = ROOT.TFile(rootfile,"update")
    gr = ROOT.TGraph(fpr.size,tpr,1-fpr)
    gr.Write("test_roc")
    file_.Close()
