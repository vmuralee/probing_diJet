void add_hist(TString filename,TString treename,TString branch){
  TFile* file = new TFile(filename,"update");
  TH1F* h = new TH1F("h","",10,0.05,0.95);
  TTree* t = (TTree*)file->Get(treename);
  t->Draw(branch+">>h");
  h->Write(branch);
  file->Close();
}
void createfile(){
  add_hist("NN_tuple_gg_JSS.root","TreeS_ggJSS","sig_score");
  add_hist("NN_tuple_gg_JSS.root","TreeB_SMbkg","bkg_score");
}
