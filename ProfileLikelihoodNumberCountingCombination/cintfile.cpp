// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME cintfile
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualifies STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "RooPoisson.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void delete_RooPoisson(void *p);
   static void deleteArray_RooPoisson(void *p);
   static void destruct_RooPoisson(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooPoisson*)
   {
      ::RooPoisson *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooPoisson >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooPoisson", ::RooPoisson::Class_Version(), "RooPoisson.h", 21,
                  typeid(::RooPoisson), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooPoisson::Dictionary, isa_proxy, 4,
                  sizeof(::RooPoisson) );
      instance.SetDelete(&delete_RooPoisson);
      instance.SetDeleteArray(&deleteArray_RooPoisson);
      instance.SetDestructor(&destruct_RooPoisson);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooPoisson*)
   {
      return GenerateInitInstanceLocal((::RooPoisson*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::RooPoisson*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr RooPoisson::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooPoisson::Class_Name()
{
   return "RooPoisson";
}

//______________________________________________________________________________
const char *RooPoisson::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooPoisson*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooPoisson::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooPoisson*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooPoisson::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooPoisson*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooPoisson::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooPoisson*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void RooPoisson::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooPoisson.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(RooPoisson::Class(),this);
   } else {
      R__b.WriteClassBuffer(RooPoisson::Class(),this);
   }
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_RooPoisson(void *p) {
      delete ((::RooPoisson*)p);
   }
   static void deleteArray_RooPoisson(void *p) {
      delete [] ((::RooPoisson*)p);
   }
   static void destruct_RooPoisson(void *p) {
      typedef ::RooPoisson current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::RooPoisson

namespace {
  void TriggerDictionaryInitialization_cintfile_Impl() {
    static const char* headers[] = {
"RooPoisson.h",
0
    };
    static const char* includePaths[] = {
"/home/vinay/miniconda3/include",
"/home/vinay/miniconda3/include/",
"/mnt/e/probing_HeavyDijet/MLP/probing_heavyJet/ProfileLikelihoodNumberCountingCombination/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "cintfile dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate(R"ATTRDUMP(Your description goes here...)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooPoisson.h")))  RooPoisson;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "cintfile dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "RooPoisson.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"RooPoisson", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("cintfile",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_cintfile_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_cintfile_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_cintfile() {
  TriggerDictionaryInitialization_cintfile_Impl();
}
