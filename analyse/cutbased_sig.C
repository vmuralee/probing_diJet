
#include <fstream>

void SsqrtB(TString cat,TString filename,float xsecS,float NS,float xsecB,float NB,int Nbins){
  TFile* file = TFile::Open("../output/"+filename);
  TH1F* hS = new TH1F("hS","",Nbins,0.,1.0);hS->Sumw2();
  TH1F* hB = new TH1F("hB","",Nbins,0.,1.0);hB->Sumw2();
  float lumi = 100.0;//units of fb-1
  
  TString sigtree = "TreeS";
  TTree* tS = (TTree*)file->Get(sigtree);
  TTree* tB = (TTree*)file->Get("TreeB_SMbkg");
  tS->Draw("sig_score>>hS");
  tB->Draw("bkg_score>>hB");
  //hS->Scale(1/hS->Integral());  hB->Scale(1/hB->Integral());
  float normS = lumi*xsecS/NS;
  float normB = lumi*xsecB/NB;
  hS->Scale(normS);  hB->Scale(normB);
  cout<<normS<<"  "<<normB<<endl;

  hB->Draw("histo");
  hS->Draw("histosame");
  float min = 1000.0;
  int Bmin =0;
  float yB[Nbins],yS[Nbins],xB[Nbins],xS[Nbins];
  ofstream fout;
  fout.open("events.txt");
  for(int ib=1;ib<hB->GetNbinsX()+1;ib++){
    
    int Sevents = hS->GetBinContent(ib);
    int Bevents = hB->GetBinContent(ib);
    xS[ib]= hS->GetBinWidth(ib);
    xB[ib]= hB->GetBinError(ib);
    yS[ib] = Sevents;
    yB[ib] = Bevents;
    cout<<xS[ib]<<"  "<<xB[ib]<<endl;
    fout<<yS[ib]<<","<<yB[ib]<<endl;//<<","<<xB[ib]<<endl;
  }
  //cout<<"minimum value: "<<hB->GetEntries()<<" "<<Bmin<<endl;
 
}
void cutbased_sig(int N =50){
  //SsqrtB("gg","NN_tuple_gg_Nocut.root",80.39,28331,N);
  //SsqrtB("gg","NN_tuple_ggJSS_Nocut.root",80.39,28331,N);

  SsqrtB("gg","NN_tuple_pt5_gg_Nocut.root",80.39,264268,28331,324450,N);
  //SsqrtB("gg","NN_tuple_pt5_ggJSS_Nocut.root",80.39,28331,N);
  
  // SsqrtB("qg","NN_tuple_qg.root");
  /* SsqrtB("qgJSS","NN_tuple_qg_JSS.root"); */
  /* SsqrtB("qq","NN_tuple_qq.root"); */
  /* SsqrtB("qqJSS","NN_tuple_qq_JSS.root"); */
}
TGraph* SoverB_histo(TString cat="gg",TString filename="NN_tuple_gg.root",int Nbins=50){
  TFile* file = TFile::Open("../output/"+filename);
  TH1F* hS = new TH1F("hS","",Nbins,0.,1.0);hS->Sumw2();
  TH1F* hB = new TH1F("hB","",Nbins,0.,1.0);hB->Sumw2();

  TString sigtree = "TreeS";
  TTree* tS = (TTree*)file->Get(sigtree);
  TTree* tB = (TTree*)file->Get("TreeB_SMbkg");
  tS->Draw("sig_score>>hS");
  tB->Draw("bkg_score>>hB");

  float normS = 493.9/hS->GetSumOfWeights();
  float normB = 5168/hB->GetSumOfWeights();
  hS->Scale(normS);  hB->Scale(normB);
  float sig[Nbins],points[Nbins];
  for(int i=0; i<=Nbins;i++)sig[i]=0;
  for(int i=0; i<=Nbins;i++)points[i]=0;
  for(int ib=1;ib<hB->GetNbinsX()+1;ib++){
    if(hB->GetBinContent(ib)==0)continue;
    sig[ib] = hS->GetBinContent(ib)/sqrt(hB->GetBinContent(ib));
    points[ib] = hS->GetBinCenter(ib);
    cout<<sig[ib]<<" "<<points[ib]<<endl;
  }
  TGraph* gr = new TGraph(Nbins,points,sig);
  //gr->Draw("C");
  return gr;
}
void compare_sig(){
  TGraph* gr = SoverB_histo("gg","NN_tuple_gg.root",10);
  TGraph* gr_JSS = SoverB_histo("ggJSS","NN_tuple_gg_JSS.root",10);
  
  gr->Draw("");
  gr_JSS->Draw("same");
  gr->SetLineColor(kRed);
  gr_JSS->SetLineColor(kBlue);
  //gr->SetRange(0,0.5);
}
