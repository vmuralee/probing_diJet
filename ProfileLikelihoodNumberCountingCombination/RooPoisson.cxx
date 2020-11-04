 /***************************************************************************** 
  * Project: RooFit                                                           * 
  *                                                                           * 
  * Simple Poisson PDF
  * author: Kyle Cranmer <cranmer@cern.ch>
  *                                                                           * 
  * Redistribution and use in source and binary forms,                        * 
  * with or without modification, are permitted according to the terms        * 
  * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             * 
  *****************************************************************************/ 

 // -- CLASS DESCRIPTION [PDF] -- 
 // Your description goes here... 

 #include <iostream> 

 #include "RooPoisson.h" 
 #include "RooAbsReal.h" 
 #include "RooAbsCategory.h" 

#include "RooRandom.h"
#include "RooMath.h"
#include "TMath.h"

#include "TRandom.h"

 ClassImp(RooPoisson) 

 RooPoisson::RooPoisson(const char *name, const char *title, 
                        RooAbsReal& _x,
                        RooAbsReal& _mean) :
   RooAbsPdf(name,title), 
   x("x","x",this,_x),
   mean("mean","mean",this,_mean)
 { 
 } 


 RooPoisson::RooPoisson(const RooPoisson& other, const char* name) :  
   RooAbsPdf(other,name), 
   x("x",this,other.x),
   mean("mean",this,other.mean)
 { 
 } 


Double_t RooPoisson::evaluate(double k) const
{
  return TMath::Poisson(k, mean);
}

 Double_t RooPoisson::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
  double k = floor(x);  
  return evaluate(k);
 } 

Int_t RooPoisson::getAnalyticalIntegral(RooArgSet& allVars, 
RooArgSet& analVars, const char* rangeName) const
{
 return 0 ;
}

Double_t RooPoisson::analyticalIntegral(Int_t code, const char* rangeName) const 
{
  assert(code==1) ;

  Double_t tmp = 0.;
  double i = x.min(rangeName);
  while( i<x.max(rangeName) ){
    tmp += evaluate(i);
    i+=1.;
  }

  return tmp;
}




Int_t RooPoisson::getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t /*staticInitOK*/) const
{
  if (matchArgs(directVars,generateVars,x)) return 1 ;  
  return 0 ;
}


void RooPoisson::generateEvent(Int_t code)
{
  assert(code==1) ;
  Double_t xgen ;
  TRandom trand;
  while(1) {    
    xgen = trand.Poisson(mean);
    //xgen = poisson_gen(mean);
    if (xgen<x.max() && xgen>x.min()) {
      x = xgen ;
      break;
    }
  }
  return;
}


