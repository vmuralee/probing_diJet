///////////////////////////////////////////////////
//
//  ProfileCombination:
//  Author: Kyle Cranmer <cranmer@cern.ch>
//  Date: May 10, 2007
//
//  Usage: ProfileCombination s1 s2 .. sN b1 b2 ... bN db1 db2 .. dbN
//         Where s_i = expected number of signal events
//               b_i = expected number of background events
//               db_i= relative background uncertainty (use 0.05 for a 5% uncertainty)
//
//  Description: Calculates expected significance for N number counting channels 
//               with uncorrelated systematic errors.
//               Each channel is composed of a main  measurement (x) and a sideband measurement (y).
//               x is modeled as a Poisson with mean masterSignal*s_i + b_i
//               y is modeled as a Poisson with mean b_i*tau_i
//               tau_i is the factor corresponding to the relative background uncertainty db_i
//
///////////////////////////////////////////////////

#include <stdio.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooAddition.h"
//#include "RooAdditionFixed.h"
#include "RooProduct.h"
#include "RooDataSet.h"
#include "RooProdPdf.h"
#include "RooPlot.h"
#include "TMath.h"
#include "TCanvas.h"
#include "RooFitResult.h"
#include "RooNLLVar.h"
#include "RooPoisson.h"
#include "RooGaussian.h"
#include "RooMinuit.h"
#include "RooGlobalFunc.h"
#include "RooCmdArg.h"
#include "TH2F.h"
#include "TTree.h"

using namespace std;

double ProfileCombination(double* sig, double* back, double* sig_syst, 
			  double* back_syst, int nSigTypes, int nBackTypes, int nbins){
  using namespace RooFit;
  using std::vector;

  vector<RooRealVar*> backVec, tauVec, xVec, yVec;
  vector<RooProduct*> sigVec;
  vector<RooFormulaVar*> splusbVec;
  vector<RooPoisson*> sigRegions, sidebands;
  TList likelihoodFactors;
  TList observablesCollection;

  TTree* tree = new TTree();
  double* xForTree = new double[nbins];
  double* yForTree = new double[nbins];

  double MaxSigma = 8;

  RooRealVar*   masterSignal = 
    new RooRealVar("masterSignal","masterSignal",1., 0., 3.);
  for(int i=0; i<nbins; ++i){
    std::stringstream str;
    str<<"_"<<i;
    RooRealVar*   expectedSignal = 
      new RooRealVar(("expected_s"+str.str()).c_str(),("expected_s"+str.str()).c_str(),sig[i], 0., 2*sig[i]);
    expectedSignal->setConstant(kTRUE);

    RooProduct*   s = 
      new RooProduct(("s"+str.str()).c_str(),("s"+str.str()).c_str(), RooArgSet(*masterSignal, *expectedSignal)); 

    RooRealVar*   b = 
      new RooRealVar(("b"+str.str()).c_str(),("b"+str.str()).c_str(),back[i],  0., 1.2*back[i]+MaxSigma*(sqrt(back[i])+back[i]*back_syst[i]));
    b->Print();
    double _tau = 1./back[i]/back_syst[i]/back_syst[i];
    RooRealVar*  tau = 
      new RooRealVar(("tau"+str.str()).c_str(),("tau"+str.str()).c_str(),_tau,0,2*_tau); 
    tau->setConstant(kTRUE);

    // need a patched version of RooAddition for this to work.  Or use the next lines instead
    //    RooFormulaVar*  splusb = 
    //      new RooFormulaVar(("splusb"+str.str()).c_str(),("s"+str.str()+"+"+"b"+str.str()).c_str(),   
    //    			RooArgSet(*s,*b)); 

    RooAddition*  splusb = 
      new RooAddition(("splusb"+str.str()).c_str(),("s"+str.str()+"+"+"b"+str.str()).c_str(),   
		      RooArgSet(*s,*b)); 
    //    RooAdditionFixed*  splusb = 
    //      new RooAdditionFixed(("splusb"+str.str()).c_str(),("s"+str.str()+"+"+"b"+str.str()).c_str(),   
    //			   RooArgSet(*s,*b)); 

    RooProduct*   bTau = 
      new RooProduct(("bTau"+str.str()).c_str(),("b*tau"+str.str()).c_str(),   RooArgSet(*b, *tau)); 
    RooRealVar*   x = 
      new RooRealVar(("x"+str.str()).c_str(),("x"+str.str()).c_str(),  sig[i]+back[i], 0., 1.2*sig[i]+back[i]+MaxSigma*sqrt(sig[i]+back[i]));
    RooRealVar*   y = 
      new RooRealVar(("y"+str.str()).c_str(),("y"+str.str()).c_str(),  back[i]*_tau,  0., 1.2*back[i]*_tau+MaxSigma*sqrt(back[i]*_tau));


    RooPoisson* sigRegion = 
      new RooPoisson(("sigRegion"+str.str()).c_str(),("sigRegion"+str.str()).c_str(), *x,*splusb);
    RooPoisson* sideband = 
      new RooPoisson(("sideband"+str.str()).c_str(),("sideband"+str.str()).c_str(), *y,*bTau);

    sigVec.push_back(s);
    backVec.push_back(b);
    tauVec.push_back(tau);
    xVec.push_back(x);
    yVec.push_back(y);
    sigRegions.push_back(sigRegion);
    sidebands.push_back(sideband);

    likelihoodFactors.Add(sigRegion);
    likelihoodFactors.Add(sideband);
    observablesCollection.Add(x);
    observablesCollection.Add(y);
    
    // print to see range on variables
    //    x->Print();
    //    y->Print();
    //    b->Print();


    xForTree[i] = sig[i]+back[i];
    yForTree[i] = back[i]*_tau;
    tree->Branch(("x"+str.str()).c_str(), xForTree+i ,("x"+str.str()+"/D").c_str());
    tree->Branch(("y"+str.str()).c_str(), yForTree+i ,("y"+str.str()+"/D").c_str());
  }
  tree->Fill();
  //  tree->Print();
  //  tree->Scan();

  RooArgSet likelihoodFactorSet(likelihoodFactors);
  RooProdPdf joint("joint","joint", likelihoodFactorSet );
  //  likelihoodFactorSet.Print();

  cout << "\n print model" << endl;
  joint.Print();
  //  joint.printCompactTree();

  RooArgSet* observableSet = new RooArgSet(observablesCollection);
  RooArgList* observableList = new RooArgList(observablesCollection);

  //  observableSet->Print();
  //  observableList->Print();


  cout << "Make hypothetical dataset:" << endl;

  RooDataSet* toyMC = new RooDataSet("data","data", tree, *observableList); // one experiment
  //toyMC->Scan();

  //  cout << "about to do fit \n\n" << endl;


  //RooFitResult* fit = joint.fitTo(*toyMC,Extended(kFALSE),Hesse(kFALSE),Save(kTRUE),PrintLevel(-1));
  RooFitResult* fit = joint.fitTo(*toyMC,Extended(kFALSE),Strategy(0),Hesse(kFALSE),Save(kTRUE),PrintLevel(-1));

  //RooFitResult* fit = joint.fitTo(*toyMC,"sr");
  //  fit->Print();

  //  joint.Print("v");

  ////////////////////////////////////////
  /// Calculate significance
  //////////////////////////////
  cout << "\nFit to signal plus background:" << endl;
  masterSignal->Print();
  for(int i=0; i<nbins; ++i) backVec.at(i)->Print();
  fit->Print();
  double NLLatMLE= fit->minNll();



  cout << "\nFit to background only:" << endl;
  masterSignal->setVal(0);
  masterSignal->setConstant();
  //  RooFitResult* fit2 = joint.fitTo(*toyMC,Extended(kFALSE), Save(kTRUE),PrintLevel(-1));
  //RooFitResult* fit2 = joint.fitTo(*toyMC,Extended(kFALSE),Hesse(kFALSE), Minos(kFALSE), Save(kTRUE),PrintLevel(-1));
  RooFitResult* fit2 = joint.fitTo(*toyMC,Extended(kFALSE),Hesse(kFALSE),Strategy(0), Minos(kFALSE), Save(kTRUE),PrintLevel(-1));

  masterSignal->Print();
  for(int i=0; i<nbins; ++i) backVec.at(i)->Print();
  double NLLatCondMLE= fit2->minNll();
  fit2->Print();

  cout << "Profile log-likelihood ratio (-2 log lambda) = " << 2*(NLLatCondMLE-NLLatMLE) << endl;
  cout << "corresponding to a probability of " << TMath::Prob(2*(NLLatCondMLE-NLLatMLE),1)/2. << endl;

  // good approx at high significance where ErfcInverse returns 0
  if(  2*(NLLatCondMLE-NLLatMLE) > 50 ) return sqrt( 2*(NLLatCondMLE-NLLatMLE)); 

  // otherwise return sigma from pvalue
  return TMath::ErfcInverse(TMath::Prob(2*(NLLatCondMLE-NLLatMLE),1))*sqrt(2.);
  
}

//_____________________________batch only_____________________
#ifndef __CINT__

int main(int argc, char** argv) {
  int nbins      = (argc-1)/3;  // sum over channels of nbins_chan
  int nSigTypes  = nbins;  // max number sig types in a channel
  int nBackTypes = nbins; // max number back types in a channel
  double* sig =  new double[nSigTypes];
  double* back = new double[nBackTypes];
  double* sig_syst = new double[nSigTypes];
  double* back_syst = new double[nBackTypes];
  double sigma = 0, power = 0;

  assert((argc-1)%3 == 0); // need multiple of 3 for syst errors

  srandom(2);

  for (int i = 0; i<nbins; i++){
    sig[i]  = atof(argv[i+1]);
    back[i] = atof(argv[i+nbins+1]);
    // Median of s+b basically unchanged by sig_syst.
    // Keep small for now, or change as you will
    sig_syst[i] = .0000001;  
    back_syst[i] = atof(argv[i+2*nbins+1]);

  }

  sigma = ProfileCombination(sig, back, sig_syst, back_syst, 
  				     nSigTypes, nBackTypes, nbins);

  fprintf(stderr, "\nConfidence Level is = \n");
  fprintf(stdout, "%f\n", sigma);



  return 0;
}

#endif
