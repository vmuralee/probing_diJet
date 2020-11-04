/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
 * Copyright (c) 2000-2007, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

#ifndef ROOPOISSON
#define ROOPOISSON

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooPoisson : public RooAbsPdf {
public:
  RooPoisson(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _mean);
  RooPoisson(const RooPoisson& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooPoisson(*this,newname); }
  inline virtual ~RooPoisson() { }

  Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;


  Int_t getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t staticInitOK=kTRUE) const;
  void generateEvent(Int_t code);

protected:

  RooRealProxy x ;
  RooRealProxy mean ;
  
  Double_t evaluate() const ;

  Double_t evaluate(double k) const;


private:

  ClassDef(RooPoisson,0) // Your description goes here...
};
 
#endif
