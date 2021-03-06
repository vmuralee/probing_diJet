# Created by Vinaya Krishnan
# vinaya.krishna@cern.ch

import ROOT
from root_numpy import root2array, rec2array, array2root, tree2array
import pandas as pd
from sklearn.utils import shuffle
from sklearn.preprocessing import OneHotEncoder
from sklearn.compose import ColumnTransformer
from sklearn.preprocessing import StandardScaler
import numpy as np

class Data2Array:
    def __init__(self,path,filenames,var,cuts):
        self.path = path
        self.filenames = filenames
        self.var = var
        self.m_low = cuts[0]
        self.m_high = cuts[1]
        for filename in self.filenames:
            print(f"loading data from {self.path}/{filename}")
    def variable_list(self):
        variables = self.var
        return variables
    def getTree(self,filename):
        cols_list = self.var+['target']
        file_ = ROOT.TFile(self.path+'/'+filename)
        if("SMbkg" in filename):
            treename = "TreeB"
        else:
            treename = "TreeS"
        tree = file_.Get(treename)
        tree_arr = tree2array(tree)
        tree_df = pd.DataFrame(tree_arr,columns=cols_list)
        # tree_df = tree_df.loc[tree_df['invmass']>self.m_low]
        # tree_df = tree_df.loc[tree_df['invmass']<self.m_high]
        # tree_df = tree_df.loc[tree_df['ptJ1']>800]
        # tree_df = tree_df.loc[tree_df['ptJ2']>800]
        if("gg" in filename):tree_df['target']='1'
        if("ug" in filename):tree_df['target']='1'
        if("qq" in filename):tree_df['target']='1'
        if("SMbkg" in filename):tree_df['target']='0'
        return tree_df
    def load_df(self):
        cols_list = self.var+['target']
        filenames = self.filenames
        data = pd.DataFrame(columns=cols_list)
        for filename in filenames:
            tmp_df = self.getTree(filename)
            data = data.append(tmp_df,ignore_index=True,sort=False)
        for _ in range(8):data = shuffle(data)
        return data

    def save_csv(self,data,csv_filename):
        return data.to_csv(csv_filename+'.csv')

    def cat_encoder(self,data_cat):
        encoder = OneHotEncoder()
        data_hot = encoder.fit_transform(data_cat)
        return data_hot
    
class Array2Data:
    def __init__(self,score,tree_df,filename):
        self.score = score
        self.tree_df = tree_df
        self.filename = filename
        self.tree_df['nn_score']=score[:,1]
        self.Bkg_df = tree_df.loc[tree_df['target']=='0']
        self.Sig_df = tree_df.loc[tree_df['target']=='1']
        print("Creating outfile .......")
    
    def SigTree(self):
        treename = 'TreeS'
        m_inv = np.array(self.Sig_df['invmass'],dtype='float32')
        score_list = np.array(self.Sig_df['nn_score'],dtype='float32')
        zip_l = zip(m_inv,score_list)
        sig_score = list(zip_l)
        branchs = np.array(sig_score,dtype=[('m_inv','float32'),('sig_score','float32')])
        return array2root(branchs,self.filename,treename=treename,mode='update')

    def BkgTree(self):
        treename = 'TreeB_SMbkg'
        m_inv = np.array(self.Bkg_df['invmass'],dtype='float32')
        score_list = np.array(self.Bkg_df['nn_score'],dtype='float32')
        zip_l = zip(m_inv,score_list)
        sig_score = list(zip_l)
        branchs = np.array(sig_score,dtype=[('m_inv','float32'),('bkg_score','float32')])
        return array2root(branchs,self.filename,treename=treename,mode='update')
        
