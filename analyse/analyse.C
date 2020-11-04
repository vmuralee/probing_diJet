void scores_roc(TString filename1,TString filename2,TString cat,TCut cut){
  TFile* filewithoutJSS = new TFile("../output/"+filename1);
  TFile* filewithJSS = new TFile("../output/"+filename2);

  TTree* t_sig = (TTree*)filewithoutJSS->Get("TreeS_"+cat);
  TTree* t_sigJSS = (TTree*)filewithJSS->Get("TreeS_"+cat+"JSS");
 
  TTree* t_bkg = (TTree*)filewithoutJSS->Get("TreeB_SMbkg");
  TTree* t_bkgJSS = (TTree*)filewithJSS->Get("TreeB_SMbkg");

  TGraph* gr = (TGraph*)filewithoutJSS->Get("roc_graph");
  TGraph* gr_JSS = (TGraph*)filewithJSS->Get("roc_graph");

  TCanvas* c1 = new TCanvas("c1","",500,500);
  //c1->SetLogy();
  TH1F* hS_score = new TH1F("hS_score","",10,0.0,1.0);
  TH1F* hB_score = new TH1F("hB_score","",10,0.0,1.0);
  TH1F* hSJSS_score = new TH1F("hSJSS_score","",10,0.0,1.0);
  TH1F* hBJSS_score = new TH1F("hBJSS_score","",10,0.0,1.0);

  cout<<"Drawing ......................"<<endl;
  t_sig->Draw("sig_score>>hS_score",cut);
  t_bkg->Draw("bkg_score>>hB_score",cut);
  t_sigJSS->Draw("sig_score>>hSJSS_score",cut);
  t_bkgJSS->Draw("bkg_score>>hBJSS_score",cut);

  hS_score->Scale(1/hS_score->Integral());
  hB_score->Scale(1/hB_score->Integral());
  hSJSS_score->Scale(1/hSJSS_score->Integral());
  hBJSS_score->Scale(1/hBJSS_score->Integral());
  float maxb = hBJSS_score->GetMaximum();
  
  hS_score->SetFillStyle(3005);
  hS_score->SetFillColor(3);
  hB_score->SetFillStyle(3004);
  hB_score->SetFillColor(9);

  hSJSS_score->SetLineColor(2);
  hSJSS_score->SetLineWidth(2);
  hBJSS_score->SetLineColor(4);
  hBJSS_score->SetLineWidth(2);

  hS_score->Draw("histo");
  hB_score->Draw("histosame");
  hSJSS_score->Draw("histosame");
  hBJSS_score->Draw("histosame");

  hS_score->SetMaximum(1.5*maxb);
  hS_score->GetXaxis()->SetNdivisions(5);
  hS_score->GetXaxis()->SetTitle("NN Score");
  hS_score->SetStats(0);

  auto legend = new TLegend(0.309237,0.772632,0.690763,0.890526);
  legend->SetBorderSize(0);
  legend->SetNColumns(2);
  legend->AddEntry(hS_score,cat,"f");
  legend->AddEntry(hB_score,"SMbkg","f");
  legend->AddEntry(hSJSS_score,cat+" JSS","f");
  legend->AddEntry(hBJSS_score,"SMbkg JSS","f");
  legend->Draw();
  
  c1->SaveAs("../output/NN_score_"+cat+".png");
  //c1->delete;

  // TCanvas* c2 = new TCanvas("c2","",500,500);
  // gr->SetLineColor(kBlack);
  // gr_JSS->SetLineColor(kRed);
  // gr->Draw();
  // gr_JSS->Draw("same");
  // gr->GetXaxis()->SetRangeUser(0,1.1);
  // gr->GetYaxis()->SetRangeUser(0,1.1);
  // gr->GetXaxis()->SetTitle("signal accptance");
  // gr->GetYaxis()->SetTitle("signal rejection");
  // gr->SetTitle(cat+" VS SMbkg");

  // auto legend1 = new TLegend(0.307229,0.454737,0.48996,0.532632);
  // legend1->SetBorderSize(0);
  // legend1->AddEntry(gr,cat,"l");
  // legend1->AddEntry(gr_JSS,cat+" JSS","l");
  // legend1->Draw();
  
  // c2->SaveAs("../output/roc_curve_"+cat+".png");
    
}
void plot(){
  // scores_roc("NN_tuple_qg.root","NN_tuple_qg_JSS.root","qg","m_inv>1600&&m_inv<2120");
  // scores_roc("NN_tuple_qq.root","NN_tuple_qq_JSS.root","qq","m_inv>1680&&m_inv<2180");
  // scores_roc("NN_tuple_gg.root","NN_tuple_gg_JSS.root","gg","");
  scores_roc("test_gg.root","test_ggJSS.root","gg","");
}