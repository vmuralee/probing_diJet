void plot_variables(TString var,int bins,float xl,float xh){

  TFile* f_signal = new TFile("../data/Ntuples/pp2coloron2_qq-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root");
  TFile* f_Bkg = new TFile("../data/Ntuples/pp2jj_SMbkg-Nev2e6-ptJetMin700-antiktR0.4-zcut0.1-beta1.0.root");

  TTree *sig = (TTree*)f_signal->Get("TreeS");
  TTree *bkg = (TTree*)f_Bkg->Get("TreeB");

  sig->SetMarkerColor(kBlue);
  sig->SetMarkerStyle(21);        // full circle
  sig->SetMarkerSize(0.5);
  bkg->SetMarkerColor(kRed);
  bkg->SetMarkerStyle(22);        // full triangle up
  bkg->SetMarkerSize(0.5);

  TString cut = "invmass>1900 && invmass<2200";
  
  TCanvas *c1 = new TCanvas("c1", "Canvas 1", 1000,950);
  TH1F* hframe = new TH1F("hframe","", bins,xl,xh);
  TH1F* hS_mjj = new TH1F("hS_mjj","", bins,xl,xh);
  TH1F* hB_mjj = new TH1F("hB_mjj","", bins,xl,xh);
  sig->Draw(var+">> hS_mjj","");
  bkg->Draw(var+">> hB_mjj","");
  hS_mjj->SetLineColor(kBlue);
  hB_mjj->SetLineColor(kRed);
  hS_mjj->Scale(1/hS_mjj->Integral());
  hB_mjj->Scale(1/hB_mjj->Integral());
  hframe->DrawCopy();
 
  hS_mjj->Draw("histo");
  hB_mjj->Draw("histosame");
  hS_mjj->SetStats(0);
  c1->SaveAs(var+"_Sig_Bkg.png");
}
void plot_All(){
  //plot_variables("invmass",100,0,4000);
  plot_variables("ptJ1",100,0,2000);
  plot_variables("ptJ2",100,0,2000);
  // plot_variables("etaJ1",100,-4,4);
  // plot_variables("etaJ2",100,-4,4);
  // plot_variables("DRJ1J2",100,0,5);
  // plot_variables("pmJ1",50,0,80);
  // plot_variables("pmJ2",50,0,80);
  // plot_variables("tmJ1",50,0,80);
  // plot_variables("tmJ2",50,0,80);
  // plot_variables("LHAJ1",100,0,1);
  // plot_variables("LHAJ2",100,0,1);
  // plot_variables("pTDJ1",100,0,1);
  // plot_variables("pTDJ2",100,0,1);
  // plot_variables("widthJ1",100,0,1);
  // plot_variables("widthJ2",100,0,1);
  // plot_variables("girthJ1",100,0,0.3);
  // plot_variables("girthJ2",100,0,0.3);
  // plot_variables("e05J1",100,0,0.5);
  // plot_variables("e05J2",100,0,0.5);
  // plot_variables("s2J1",100,0,0.1);
  // plot_variables("s2J2",100,0,0.1);
}
