#########################################
### DNN Training ########################
# vinaya.krishna@cern.ch#################
#########################################

import os
import ROOT
import tensorflow as tf
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
from scipy.stats import reciprocal
#from sklearn.model_selection import RandomizedSearchCV

parser = argparse.ArgumentParser(description='Training DNN model')
parser.add_argument('--predict',type=bool,help='predict samples',default=False)
parser.add_argument('--epoch',type=int,help='No. of epochs',default=10)
parser.add_argument('--cat',type=str,help='select categories',default='qg')
parser.add_argument('--path',type=str,help='pathe of samples',default='../data/Ntuples')
parser.add_argument('--JSS',type=bool,help='add JSS parameter',default=False)
parser.add_argument('--csv',type=bool,help='save csv file',default=False)
parser.add_argument('--outfile',type=str,help='output root file',default='test.root')
args = parser.parse_args()
if(args.cat not in ['qq','qg','gg']):
    print('Please choose either of qq,qg or gg')
    exit(-1)

cat_files = dict({'qq':['pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root','pp2coloron2_qq-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root'],
                  'qg':['pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root','pp2ExUp2_ug-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root'],
                  'gg':['pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root','pp2octet2_gg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root']
})
filenames = cat_files[args.cat]

labels = ['ptJ1','ptJ2','invmass','DRJ1J2','etaJ1','etaJ2']
if(args.JSS):
    print('Using JSS paramets')
    labels = labels+['pTDJ1','pTDJ2','LHAJ1','LHAJ2','e05J1','e05J2','s2J1','s2J2','pmJ1','pmJ2','tmJ1','tmJ2','widthJ1','widthJ2','girthJ1','girthJ2']

data = Data2Array(args.path,filenames,labels)
data_ar = data.load_df()

# Export to csv file
savefileAs = 'dijet_data_'+args.cat
if(args.JSS):
    savefileAs = 'dijet_data_JSS_'+args.cat
if(args.csv):
    data.save_csv(data_ar,savefileAs)


#Creating Training,Test and Validation samples
traindataset_full,testdataset = train_test_split(data_ar,test_size=0.1,random_state=42)
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

l_rate =  0.01304080507013371

#Tuned Model
model = build_model(n_hidden=3,n_neurons=475,learning_rate=l_rate,input_shapes=len(labels))

#checkpoint_cb = keras.callbacks.ModelCheckpoint("keras_model.h5")
prefit = model.fit(X_train_tr,y_train,epochs=args.epoch,validation_data=(X_val_tr,y_val),callbacks=[keras.callbacks.EarlyStopping(patience=10)])
if(args.JSS):
    model.save("keras_model_JSS_"+args.cat+".h5")
else:
    model.save("keras_model_"+args.cat+".h5")

#Learning Curves
pd.DataFrame(prefit.history).plot(figsize=(8,5))
plt.grid(True)
plt.gca().set_ylim(0,1)
plt.savefig('../plots/learning_Curves.png')
#plt.show()

#Prediction
if(args.predict):
    y_pred = model.predict(X_test_tr)
    
#Storing ntuple
if(args.predict):
    tfile = Array2Data(y_pred,testdataset,args.outfile)
    prefix_ = args.cat
    if(args.JSS):
        prefix_=prefix_+"JSS"
    tfile.SigTree(prefix_)
    tfile.BkgTree()

#ROC
if(args.predict):
    prefix_ = args.cat
    if(args.JSS):
        prefix_=prefix_+"JSS"
    plt.plot(figsize=(6,5))
    plt.grid(True)
    y_out = np.array(y_test,dtype='float32')
    fpr,tpr,th = roc_curve(y_true=y_out,y_score=y_pred[:,1])
    auc = roc_auc_score(y_out,y_pred[:,1])
    plt.plot(tpr,1-fpr)
    plt.text(0.15,0.35,f'auc value: {auc}')
    plt.savefig('../plots/ROC_'+prefix_+'.png')
    plt.xlabel('efficiency')
    plt.ylabel('misidentification')
    #plt.show()

    #Storing ROC
    file_ = ROOT.TFile(args.outfile,"update")
    gr = ROOT.TGraph(fpr.size,tpr,1-fpr)
    gr.Write("roc_graph")
    file_.Close()
