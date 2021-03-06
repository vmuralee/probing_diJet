import tensorflow as tf
from tensorflow import keras
from sklearn.model_selection import train_test_split
import numpy as np
import copy
from sklearn.preprocessing import OneHotEncoder
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.impute import SimpleImputer
from Data2Array import Data2Array,Array2Data
import matplotlib.pyplot as plt

cat_files = dict({'qq':['pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root','pp2coloron2_qq-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root'],
                  'qg':['pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root','pp2ExUp2_ug-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root'],
                  'gg':['pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root','pp2octet2_gg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root']
})
filenames = cat_files['gg']

labels = ['ptJ1','ptJ2','invmass','DRJ1J2','etaJ1','etaJ2']

print('Using JSS paramets')
#labels = labels+['pTDJ1','pTDJ2','LHAJ1','LHAJ2','e05J1','e05J2','s2J1','s2J2','pmJ1','pmJ2','tmJ1','tmJ2','widthJ1','widthJ2','girthJ1','girthJ2']

data = Data2Array('../data/Ntuples',filenames,labels,(1580,2100))
data_ar = data.load_df()


model_new = keras.models.load_model('keras_model_gg_wcut.h5')

#Creating Training,Test and Validation samples
traindataset_full,testdataset = train_test_split(data_ar,test_size=0.5,random_state=42)
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


y_pred = model_new.predict(X_test_tr)

tfile = Array2Data(y_pred,testdataset,"test_gg_wcut.root")
tfile.SigTree("gg")
tfile.BkgTree()
