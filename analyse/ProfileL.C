#include "TFile.h"
#include "TROOT.h"
#include "TSystem.h"
#include "RooWorkspace.h"
#include "RooAbsData.h"
#include "RooRealVar.h"
 
#include "RooStats/ModelConfig.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"

using namespace RooFit;
using namespace RooStats;

struct ProfileLikelihoodOptions {
 
   double confLevel = 0.95;
   int nScanPoints = 50;
   bool plotAsTF1 = false;
   double poiMinPlot = 1;
   double poiMaxPlot = 0;
   bool doHypoTest = false;
   double nullValue = 0;
};

ProfileLikelihoodOptions optPL;
 
void StandardProfileLikelihoodDemo(const char *infile = "model.root", const char *workspaceName = "combined",
                                   const char *modelConfigName = "ModelConfig", const char *dataName = "obsData")
{ 
  double confLevel = optPL.confLevel;
  double nScanPoints = optPL.nScanPoints;
  bool plotAsTF1 = optPL.plotAsTF1;
  double poiXMin = optPL.poiMinPlot;
  double poiXMax = optPL.poiMaxPlot;
  bool doHypoTest = optPL.doHypoTest;
  double nullParamValue = optPL.nullValue;

  const char* filename = infile;
  TFile *file = TFile::Open(filename);

  // get the workspace out of the file
  RooWorkspace *w = (RooWorkspace *)file->Get(workspaceName);
  
}

void create_model(TString infile="NN_tuple_gg_JSS.root"){
  TFile* file = new TFile("../output/"+infile);
  TH1F* h_sig = (TH1F*)file->Get("sig_score");
  TH1F* h_bkg = (TH1F*)file->Get("bkg_score");
  TTree* treeS = (TTree*)file->Get("TreeS_ggJSS");

  h_sig->Add(h_bkg);
  RooRealVar sig_score("sig_score","sig_score",0.0,1.0);
  RooRealVar mean("mean","mean",0.7,0.5,1.0);
  RooDataHist sigdataset("sigdataset","sigdataset",sig_score,h_sig);
  
  
  RooArgusBG model("model","model",sig_score,RooConst(1.2),mean);
  model.fitTo(sigdataset);

  RooPlot* sbframe = sig_score.frame();
  sigdataset.plotOn(sbframe);
  //model.plotOn(sbframe);
  sbframe->Draw();
}
