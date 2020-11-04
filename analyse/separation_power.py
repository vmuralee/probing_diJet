import ROOT

sample_list = {'qq':'NN_tuple_qq.root','qg':'NN_tuple_qg.root','gg':'NN_tuple_gg.root',
               'qqJSS':'NN_tuple_qq_JSS.root','qgJSS':'NN_tuple_qg_JSS.root','ggJSS':'NN_tuple_gg_JSS.root'}

def get_histo(cat):
    file1 = ROOT.TFile('../output/'+sample_list[cat])
    treename = "TreeS_"+cat
    t = file1.Get(treename)
    h = ROOT.TH1F("h","",20,0.0,1.0)
    t.Draw("sig_score>>h")
    h = ROOT.gROOT.FindObject('h')
    return h

get_histo('qq')

