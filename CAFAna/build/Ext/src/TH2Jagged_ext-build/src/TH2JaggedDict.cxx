// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TH2JaggedDict

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

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "TH2Jagged.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *TH2TypeTraitslEdoublegR_Dictionary();
   static void TH2TypeTraitslEdoublegR_TClassManip(TClass*);
   static void *new_TH2TypeTraitslEdoublegR(void *p = 0);
   static void *newArray_TH2TypeTraitslEdoublegR(Long_t size, void *p);
   static void delete_TH2TypeTraitslEdoublegR(void *p);
   static void deleteArray_TH2TypeTraitslEdoublegR(void *p);
   static void destruct_TH2TypeTraitslEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TH2TypeTraits<double>*)
   {
      ::TH2TypeTraits<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TH2TypeTraits<double>));
      static ::ROOT::TGenericClassInfo 
         instance("TH2TypeTraits<double>", "TH2Jagged.h", 15,
                  typeid(::TH2TypeTraits<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TH2TypeTraitslEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::TH2TypeTraits<double>) );
      instance.SetNew(&new_TH2TypeTraitslEdoublegR);
      instance.SetNewArray(&newArray_TH2TypeTraitslEdoublegR);
      instance.SetDelete(&delete_TH2TypeTraitslEdoublegR);
      instance.SetDeleteArray(&deleteArray_TH2TypeTraitslEdoublegR);
      instance.SetDestructor(&destruct_TH2TypeTraitslEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TH2TypeTraits<double>*)
   {
      return GenerateInitInstanceLocal((::TH2TypeTraits<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TH2TypeTraits<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TH2TypeTraitslEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TH2TypeTraits<double>*)0x0)->GetClass();
      TH2TypeTraitslEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TH2TypeTraitslEdoublegR_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","TH2Jagged.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TH2TypeTraitslEfloatgR_Dictionary();
   static void TH2TypeTraitslEfloatgR_TClassManip(TClass*);
   static void *new_TH2TypeTraitslEfloatgR(void *p = 0);
   static void *newArray_TH2TypeTraitslEfloatgR(Long_t size, void *p);
   static void delete_TH2TypeTraitslEfloatgR(void *p);
   static void deleteArray_TH2TypeTraitslEfloatgR(void *p);
   static void destruct_TH2TypeTraitslEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TH2TypeTraits<float>*)
   {
      ::TH2TypeTraits<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TH2TypeTraits<float>));
      static ::ROOT::TGenericClassInfo 
         instance("TH2TypeTraits<float>", "TH2Jagged.h", 20,
                  typeid(::TH2TypeTraits<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TH2TypeTraitslEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::TH2TypeTraits<float>) );
      instance.SetNew(&new_TH2TypeTraitslEfloatgR);
      instance.SetNewArray(&newArray_TH2TypeTraitslEfloatgR);
      instance.SetDelete(&delete_TH2TypeTraitslEfloatgR);
      instance.SetDeleteArray(&deleteArray_TH2TypeTraitslEfloatgR);
      instance.SetDestructor(&destruct_TH2TypeTraitslEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TH2TypeTraits<float>*)
   {
      return GenerateInitInstanceLocal((::TH2TypeTraits<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TH2TypeTraits<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TH2TypeTraitslEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TH2TypeTraits<float>*)0x0)->GetClass();
      TH2TypeTraitslEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void TH2TypeTraitslEfloatgR_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","TH2Jagged.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TH2TypeTraitslEintgR_Dictionary();
   static void TH2TypeTraitslEintgR_TClassManip(TClass*);
   static void *new_TH2TypeTraitslEintgR(void *p = 0);
   static void *newArray_TH2TypeTraitslEintgR(Long_t size, void *p);
   static void delete_TH2TypeTraitslEintgR(void *p);
   static void deleteArray_TH2TypeTraitslEintgR(void *p);
   static void destruct_TH2TypeTraitslEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TH2TypeTraits<int>*)
   {
      ::TH2TypeTraits<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TH2TypeTraits<int>));
      static ::ROOT::TGenericClassInfo 
         instance("TH2TypeTraits<int>", "TH2Jagged.h", 25,
                  typeid(::TH2TypeTraits<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TH2TypeTraitslEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TH2TypeTraits<int>) );
      instance.SetNew(&new_TH2TypeTraitslEintgR);
      instance.SetNewArray(&newArray_TH2TypeTraitslEintgR);
      instance.SetDelete(&delete_TH2TypeTraitslEintgR);
      instance.SetDeleteArray(&deleteArray_TH2TypeTraitslEintgR);
      instance.SetDestructor(&destruct_TH2TypeTraitslEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TH2TypeTraits<int>*)
   {
      return GenerateInitInstanceLocal((::TH2TypeTraits<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TH2TypeTraits<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TH2TypeTraitslEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TH2TypeTraits<int>*)0x0)->GetClass();
      TH2TypeTraitslEintgR_TClassManip(theClass);
   return theClass;
   }

   static void TH2TypeTraitslEintgR_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","TH2Jagged.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TH2TypeTraitslEchargR_Dictionary();
   static void TH2TypeTraitslEchargR_TClassManip(TClass*);
   static void *new_TH2TypeTraitslEchargR(void *p = 0);
   static void *newArray_TH2TypeTraitslEchargR(Long_t size, void *p);
   static void delete_TH2TypeTraitslEchargR(void *p);
   static void deleteArray_TH2TypeTraitslEchargR(void *p);
   static void destruct_TH2TypeTraitslEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TH2TypeTraits<char>*)
   {
      ::TH2TypeTraits<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::TH2TypeTraits<char>));
      static ::ROOT::TGenericClassInfo 
         instance("TH2TypeTraits<char>", "TH2Jagged.h", 30,
                  typeid(::TH2TypeTraits<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TH2TypeTraitslEchargR_Dictionary, isa_proxy, 4,
                  sizeof(::TH2TypeTraits<char>) );
      instance.SetNew(&new_TH2TypeTraitslEchargR);
      instance.SetNewArray(&newArray_TH2TypeTraitslEchargR);
      instance.SetDelete(&delete_TH2TypeTraitslEchargR);
      instance.SetDeleteArray(&deleteArray_TH2TypeTraitslEchargR);
      instance.SetDestructor(&destruct_TH2TypeTraitslEchargR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TH2TypeTraits<char>*)
   {
      return GenerateInitInstanceLocal((::TH2TypeTraits<char>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TH2TypeTraits<char>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TH2TypeTraitslEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TH2TypeTraits<char>*)0x0)->GetClass();
      TH2TypeTraitslEchargR_TClassManip(theClass);
   return theClass;
   }

   static void TH2TypeTraitslEchargR_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","TH2Jagged.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *JBinId_Dictionary();
   static void JBinId_TClassManip(TClass*);
   static void *new_JBinId(void *p = 0);
   static void *newArray_JBinId(Long_t size, void *p);
   static void delete_JBinId(void *p);
   static void deleteArray_JBinId(void *p);
   static void destruct_JBinId(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JBinId*)
   {
      ::JBinId *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::JBinId));
      static ::ROOT::TGenericClassInfo 
         instance("JBinId", "TH2Jagged.h", 35,
                  typeid(::JBinId), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &JBinId_Dictionary, isa_proxy, 4,
                  sizeof(::JBinId) );
      instance.SetNew(&new_JBinId);
      instance.SetNewArray(&newArray_JBinId);
      instance.SetDelete(&delete_JBinId);
      instance.SetDeleteArray(&deleteArray_JBinId);
      instance.SetDestructor(&destruct_JBinId);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JBinId*)
   {
      return GenerateInitInstanceLocal((::JBinId*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::JBinId*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *JBinId_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::JBinId*)0x0)->GetClass();
      JBinId_TClassManip(theClass);
   return theClass;
   }

   static void JBinId_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","TH2Jagged.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TH2JaggedlEdoublegR_Dictionary();
   static void TH2JaggedlEdoublegR_TClassManip(TClass*);
   static void *new_TH2JaggedlEdoublegR(void *p = 0);
   static void *newArray_TH2JaggedlEdoublegR(Long_t size, void *p);
   static void delete_TH2JaggedlEdoublegR(void *p);
   static void deleteArray_TH2JaggedlEdoublegR(void *p);
   static void destruct_TH2JaggedlEdoublegR(void *p);
   static void directoryAutoAdd_TH2JaggedlEdoublegR(void *obj, TDirectory *dir);
   static Long64_t merge_TH2JaggedlEdoublegR(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TH2Jagged<double>*)
   {
      ::TH2Jagged<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TH2Jagged<double> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TH2Jagged<double>", ::TH2Jagged<double>::Class_Version(), "TH2Jagged.h", 197,
                  typeid(::TH2Jagged<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TH2JaggedlEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(::TH2Jagged<double>) );
      instance.SetNew(&new_TH2JaggedlEdoublegR);
      instance.SetNewArray(&newArray_TH2JaggedlEdoublegR);
      instance.SetDelete(&delete_TH2JaggedlEdoublegR);
      instance.SetDeleteArray(&deleteArray_TH2JaggedlEdoublegR);
      instance.SetDestructor(&destruct_TH2JaggedlEdoublegR);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_TH2JaggedlEdoublegR);
      instance.SetMerge(&merge_TH2JaggedlEdoublegR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TH2Jagged<double>*)
   {
      return GenerateInitInstanceLocal((::TH2Jagged<double>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TH2Jagged<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TH2JaggedlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<double>*)0x0)->GetClass();
      TH2JaggedlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void TH2JaggedlEdoublegR_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","TH2Jagged.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TH2JaggedlEfloatgR_Dictionary();
   static void TH2JaggedlEfloatgR_TClassManip(TClass*);
   static void *new_TH2JaggedlEfloatgR(void *p = 0);
   static void *newArray_TH2JaggedlEfloatgR(Long_t size, void *p);
   static void delete_TH2JaggedlEfloatgR(void *p);
   static void deleteArray_TH2JaggedlEfloatgR(void *p);
   static void destruct_TH2JaggedlEfloatgR(void *p);
   static void directoryAutoAdd_TH2JaggedlEfloatgR(void *obj, TDirectory *dir);
   static Long64_t merge_TH2JaggedlEfloatgR(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TH2Jagged<float>*)
   {
      ::TH2Jagged<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TH2Jagged<float> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TH2Jagged<float>", ::TH2Jagged<float>::Class_Version(), "TH2Jagged.h", 198,
                  typeid(::TH2Jagged<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TH2JaggedlEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(::TH2Jagged<float>) );
      instance.SetNew(&new_TH2JaggedlEfloatgR);
      instance.SetNewArray(&newArray_TH2JaggedlEfloatgR);
      instance.SetDelete(&delete_TH2JaggedlEfloatgR);
      instance.SetDeleteArray(&deleteArray_TH2JaggedlEfloatgR);
      instance.SetDestructor(&destruct_TH2JaggedlEfloatgR);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_TH2JaggedlEfloatgR);
      instance.SetMerge(&merge_TH2JaggedlEfloatgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TH2Jagged<float>*)
   {
      return GenerateInitInstanceLocal((::TH2Jagged<float>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TH2Jagged<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TH2JaggedlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<float>*)0x0)->GetClass();
      TH2JaggedlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void TH2JaggedlEfloatgR_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","TH2Jagged.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TH2JaggedlEintgR_Dictionary();
   static void TH2JaggedlEintgR_TClassManip(TClass*);
   static void *new_TH2JaggedlEintgR(void *p = 0);
   static void *newArray_TH2JaggedlEintgR(Long_t size, void *p);
   static void delete_TH2JaggedlEintgR(void *p);
   static void deleteArray_TH2JaggedlEintgR(void *p);
   static void destruct_TH2JaggedlEintgR(void *p);
   static void directoryAutoAdd_TH2JaggedlEintgR(void *obj, TDirectory *dir);
   static Long64_t merge_TH2JaggedlEintgR(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TH2Jagged<int>*)
   {
      ::TH2Jagged<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TH2Jagged<int> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TH2Jagged<int>", ::TH2Jagged<int>::Class_Version(), "TH2Jagged.h", 199,
                  typeid(::TH2Jagged<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TH2JaggedlEintgR_Dictionary, isa_proxy, 4,
                  sizeof(::TH2Jagged<int>) );
      instance.SetNew(&new_TH2JaggedlEintgR);
      instance.SetNewArray(&newArray_TH2JaggedlEintgR);
      instance.SetDelete(&delete_TH2JaggedlEintgR);
      instance.SetDeleteArray(&deleteArray_TH2JaggedlEintgR);
      instance.SetDestructor(&destruct_TH2JaggedlEintgR);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_TH2JaggedlEintgR);
      instance.SetMerge(&merge_TH2JaggedlEintgR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TH2Jagged<int>*)
   {
      return GenerateInitInstanceLocal((::TH2Jagged<int>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TH2Jagged<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TH2JaggedlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<int>*)0x0)->GetClass();
      TH2JaggedlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void TH2JaggedlEintgR_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","TH2Jagged.h");
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *TH2JaggedlEchargR_Dictionary();
   static void TH2JaggedlEchargR_TClassManip(TClass*);
   static void *new_TH2JaggedlEchargR(void *p = 0);
   static void *newArray_TH2JaggedlEchargR(Long_t size, void *p);
   static void delete_TH2JaggedlEchargR(void *p);
   static void deleteArray_TH2JaggedlEchargR(void *p);
   static void destruct_TH2JaggedlEchargR(void *p);
   static void directoryAutoAdd_TH2JaggedlEchargR(void *obj, TDirectory *dir);
   static Long64_t merge_TH2JaggedlEchargR(void *obj, TCollection *coll,TFileMergeInfo *info);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TH2Jagged<char>*)
   {
      ::TH2Jagged<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TH2Jagged<char> >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TH2Jagged<char>", ::TH2Jagged<char>::Class_Version(), "TH2Jagged.h", 200,
                  typeid(::TH2Jagged<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &TH2JaggedlEchargR_Dictionary, isa_proxy, 4,
                  sizeof(::TH2Jagged<char>) );
      instance.SetNew(&new_TH2JaggedlEchargR);
      instance.SetNewArray(&newArray_TH2JaggedlEchargR);
      instance.SetDelete(&delete_TH2JaggedlEchargR);
      instance.SetDeleteArray(&deleteArray_TH2JaggedlEchargR);
      instance.SetDestructor(&destruct_TH2JaggedlEchargR);
      instance.SetDirectoryAutoAdd(&directoryAutoAdd_TH2JaggedlEchargR);
      instance.SetMerge(&merge_TH2JaggedlEchargR);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TH2Jagged<char>*)
   {
      return GenerateInitInstanceLocal((::TH2Jagged<char>*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::TH2Jagged<char>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *TH2JaggedlEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<char>*)0x0)->GetClass();
      TH2JaggedlEchargR_TClassManip(theClass);
   return theClass;
   }

   static void TH2JaggedlEchargR_TClassManip(TClass* theClass){
      theClass->CreateAttributeMap();
      TDictAttributeMap* attrMap( theClass->GetAttributeMap() );
      attrMap->AddProperty("file_name","TH2Jagged.h");
   }

} // end of namespace ROOT

//______________________________________________________________________________
template <> atomic_TClass_ptr TH2Jagged<double>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TH2Jagged<double>::Class_Name()
{
   return "TH2Jagged<double>";
}

//______________________________________________________________________________
template <> const char *TH2Jagged<double>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<double>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TH2Jagged<double>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<double>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TH2Jagged<double>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<double>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TH2Jagged<double>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<double>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TH2Jagged<float>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TH2Jagged<float>::Class_Name()
{
   return "TH2Jagged<float>";
}

//______________________________________________________________________________
template <> const char *TH2Jagged<float>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<float>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TH2Jagged<float>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<float>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TH2Jagged<float>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<float>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TH2Jagged<float>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<float>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TH2Jagged<int>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TH2Jagged<int>::Class_Name()
{
   return "TH2Jagged<int>";
}

//______________________________________________________________________________
template <> const char *TH2Jagged<int>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<int>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TH2Jagged<int>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<int>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TH2Jagged<int>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<int>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TH2Jagged<int>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<int>*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
template <> atomic_TClass_ptr TH2Jagged<char>::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
template <> const char *TH2Jagged<char>::Class_Name()
{
   return "TH2Jagged<char>";
}

//______________________________________________________________________________
template <> const char *TH2Jagged<char>::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<char>*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
template <> int TH2Jagged<char>::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<char>*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
template <> TClass *TH2Jagged<char>::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<char>*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
template <> TClass *TH2Jagged<char>::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TH2Jagged<char>*)0x0)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TH2TypeTraitslEdoublegR(void *p) {
      return  p ? new(p) ::TH2TypeTraits<double> : new ::TH2TypeTraits<double>;
   }
   static void *newArray_TH2TypeTraitslEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::TH2TypeTraits<double>[nElements] : new ::TH2TypeTraits<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TH2TypeTraitslEdoublegR(void *p) {
      delete ((::TH2TypeTraits<double>*)p);
   }
   static void deleteArray_TH2TypeTraitslEdoublegR(void *p) {
      delete [] ((::TH2TypeTraits<double>*)p);
   }
   static void destruct_TH2TypeTraitslEdoublegR(void *p) {
      typedef ::TH2TypeTraits<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TH2TypeTraits<double>

namespace ROOT {
   // Wrappers around operator new
   static void *new_TH2TypeTraitslEfloatgR(void *p) {
      return  p ? new(p) ::TH2TypeTraits<float> : new ::TH2TypeTraits<float>;
   }
   static void *newArray_TH2TypeTraitslEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::TH2TypeTraits<float>[nElements] : new ::TH2TypeTraits<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TH2TypeTraitslEfloatgR(void *p) {
      delete ((::TH2TypeTraits<float>*)p);
   }
   static void deleteArray_TH2TypeTraitslEfloatgR(void *p) {
      delete [] ((::TH2TypeTraits<float>*)p);
   }
   static void destruct_TH2TypeTraitslEfloatgR(void *p) {
      typedef ::TH2TypeTraits<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TH2TypeTraits<float>

namespace ROOT {
   // Wrappers around operator new
   static void *new_TH2TypeTraitslEintgR(void *p) {
      return  p ? new(p) ::TH2TypeTraits<int> : new ::TH2TypeTraits<int>;
   }
   static void *newArray_TH2TypeTraitslEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TH2TypeTraits<int>[nElements] : new ::TH2TypeTraits<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TH2TypeTraitslEintgR(void *p) {
      delete ((::TH2TypeTraits<int>*)p);
   }
   static void deleteArray_TH2TypeTraitslEintgR(void *p) {
      delete [] ((::TH2TypeTraits<int>*)p);
   }
   static void destruct_TH2TypeTraitslEintgR(void *p) {
      typedef ::TH2TypeTraits<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TH2TypeTraits<int>

namespace ROOT {
   // Wrappers around operator new
   static void *new_TH2TypeTraitslEchargR(void *p) {
      return  p ? new(p) ::TH2TypeTraits<char> : new ::TH2TypeTraits<char>;
   }
   static void *newArray_TH2TypeTraitslEchargR(Long_t nElements, void *p) {
      return p ? new(p) ::TH2TypeTraits<char>[nElements] : new ::TH2TypeTraits<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TH2TypeTraitslEchargR(void *p) {
      delete ((::TH2TypeTraits<char>*)p);
   }
   static void deleteArray_TH2TypeTraitslEchargR(void *p) {
      delete [] ((::TH2TypeTraits<char>*)p);
   }
   static void destruct_TH2TypeTraitslEchargR(void *p) {
      typedef ::TH2TypeTraits<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::TH2TypeTraits<char>

namespace ROOT {
   // Wrappers around operator new
   static void *new_JBinId(void *p) {
      return  p ? new(p) ::JBinId : new ::JBinId;
   }
   static void *newArray_JBinId(Long_t nElements, void *p) {
      return p ? new(p) ::JBinId[nElements] : new ::JBinId[nElements];
   }
   // Wrapper around operator delete
   static void delete_JBinId(void *p) {
      delete ((::JBinId*)p);
   }
   static void deleteArray_JBinId(void *p) {
      delete [] ((::JBinId*)p);
   }
   static void destruct_JBinId(void *p) {
      typedef ::JBinId current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JBinId

//______________________________________________________________________________
template <> void TH2Jagged<double>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TH2Jagged<double>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TH2Jagged<double>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TH2Jagged<double>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TH2JaggedlEdoublegR(void *p) {
      return  p ? new(p) ::TH2Jagged<double> : new ::TH2Jagged<double>;
   }
   static void *newArray_TH2JaggedlEdoublegR(Long_t nElements, void *p) {
      return p ? new(p) ::TH2Jagged<double>[nElements] : new ::TH2Jagged<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TH2JaggedlEdoublegR(void *p) {
      delete ((::TH2Jagged<double>*)p);
   }
   static void deleteArray_TH2JaggedlEdoublegR(void *p) {
      delete [] ((::TH2Jagged<double>*)p);
   }
   static void destruct_TH2JaggedlEdoublegR(void *p) {
      typedef ::TH2Jagged<double> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_TH2JaggedlEdoublegR(void *p, TDirectory *dir) {
      ((::TH2Jagged<double>*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_TH2JaggedlEdoublegR(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::TH2Jagged<double>*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::TH2Jagged<double>

//______________________________________________________________________________
template <> void TH2Jagged<float>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TH2Jagged<float>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TH2Jagged<float>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TH2Jagged<float>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TH2JaggedlEfloatgR(void *p) {
      return  p ? new(p) ::TH2Jagged<float> : new ::TH2Jagged<float>;
   }
   static void *newArray_TH2JaggedlEfloatgR(Long_t nElements, void *p) {
      return p ? new(p) ::TH2Jagged<float>[nElements] : new ::TH2Jagged<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TH2JaggedlEfloatgR(void *p) {
      delete ((::TH2Jagged<float>*)p);
   }
   static void deleteArray_TH2JaggedlEfloatgR(void *p) {
      delete [] ((::TH2Jagged<float>*)p);
   }
   static void destruct_TH2JaggedlEfloatgR(void *p) {
      typedef ::TH2Jagged<float> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_TH2JaggedlEfloatgR(void *p, TDirectory *dir) {
      ((::TH2Jagged<float>*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_TH2JaggedlEfloatgR(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::TH2Jagged<float>*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::TH2Jagged<float>

//______________________________________________________________________________
template <> void TH2Jagged<int>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TH2Jagged<int>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TH2Jagged<int>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TH2Jagged<int>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TH2JaggedlEintgR(void *p) {
      return  p ? new(p) ::TH2Jagged<int> : new ::TH2Jagged<int>;
   }
   static void *newArray_TH2JaggedlEintgR(Long_t nElements, void *p) {
      return p ? new(p) ::TH2Jagged<int>[nElements] : new ::TH2Jagged<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TH2JaggedlEintgR(void *p) {
      delete ((::TH2Jagged<int>*)p);
   }
   static void deleteArray_TH2JaggedlEintgR(void *p) {
      delete [] ((::TH2Jagged<int>*)p);
   }
   static void destruct_TH2JaggedlEintgR(void *p) {
      typedef ::TH2Jagged<int> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_TH2JaggedlEintgR(void *p, TDirectory *dir) {
      ((::TH2Jagged<int>*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_TH2JaggedlEintgR(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::TH2Jagged<int>*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::TH2Jagged<int>

//______________________________________________________________________________
template <> void TH2Jagged<char>::Streamer(TBuffer &R__b)
{
   // Stream an object of class TH2Jagged<char>.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(TH2Jagged<char>::Class(),this);
   } else {
      R__b.WriteClassBuffer(TH2Jagged<char>::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TH2JaggedlEchargR(void *p) {
      return  p ? new(p) ::TH2Jagged<char> : new ::TH2Jagged<char>;
   }
   static void *newArray_TH2JaggedlEchargR(Long_t nElements, void *p) {
      return p ? new(p) ::TH2Jagged<char>[nElements] : new ::TH2Jagged<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_TH2JaggedlEchargR(void *p) {
      delete ((::TH2Jagged<char>*)p);
   }
   static void deleteArray_TH2JaggedlEchargR(void *p) {
      delete [] ((::TH2Jagged<char>*)p);
   }
   static void destruct_TH2JaggedlEchargR(void *p) {
      typedef ::TH2Jagged<char> current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around the directory auto add.
   static void directoryAutoAdd_TH2JaggedlEchargR(void *p, TDirectory *dir) {
      ((::TH2Jagged<char>*)p)->DirectoryAutoAdd(dir);
   }
   // Wrapper around the merge function.
   static Long64_t  merge_TH2JaggedlEchargR(void *obj,TCollection *coll,TFileMergeInfo *) {
      return ((::TH2Jagged<char>*)obj)->Merge(coll);
   }
} // end of namespace ROOT for class ::TH2Jagged<char>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 214,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = 0);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 214,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)0x0)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 214,
                  typeid(vector<double>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 0,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEchargR_Dictionary();
   static void vectorlEchargR_TClassManip(TClass*);
   static void *new_vectorlEchargR(void *p = 0);
   static void *newArray_vectorlEchargR(Long_t size, void *p);
   static void delete_vectorlEchargR(void *p);
   static void deleteArray_vectorlEchargR(void *p);
   static void destruct_vectorlEchargR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<char>*)
   {
      vector<char> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<char>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<char>", -2, "vector", 214,
                  typeid(vector<char>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEchargR_Dictionary, isa_proxy, 0,
                  sizeof(vector<char>) );
      instance.SetNew(&new_vectorlEchargR);
      instance.SetNewArray(&newArray_vectorlEchargR);
      instance.SetDelete(&delete_vectorlEchargR);
      instance.SetDeleteArray(&deleteArray_vectorlEchargR);
      instance.SetDestructor(&destruct_vectorlEchargR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<char> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<char>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEchargR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<char>*)0x0)->GetClass();
      vectorlEchargR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEchargR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEchargR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<char> : new vector<char>;
   }
   static void *newArray_vectorlEchargR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<char>[nElements] : new vector<char>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEchargR(void *p) {
      delete ((vector<char>*)p);
   }
   static void deleteArray_vectorlEchargR(void *p) {
      delete [] ((vector<char>*)p);
   }
   static void destruct_vectorlEchargR(void *p) {
      typedef vector<char> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<char>

namespace ROOT {
   static TClass *vectorlETAxisgR_Dictionary();
   static void vectorlETAxisgR_TClassManip(TClass*);
   static void *new_vectorlETAxisgR(void *p = 0);
   static void *newArray_vectorlETAxisgR(Long_t size, void *p);
   static void delete_vectorlETAxisgR(void *p);
   static void deleteArray_vectorlETAxisgR(void *p);
   static void destruct_vectorlETAxisgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<TAxis>*)
   {
      vector<TAxis> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<TAxis>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<TAxis>", -2, "vector", 214,
                  typeid(vector<TAxis>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlETAxisgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<TAxis>) );
      instance.SetNew(&new_vectorlETAxisgR);
      instance.SetNewArray(&newArray_vectorlETAxisgR);
      instance.SetDelete(&delete_vectorlETAxisgR);
      instance.SetDeleteArray(&deleteArray_vectorlETAxisgR);
      instance.SetDestructor(&destruct_vectorlETAxisgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<TAxis> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<TAxis>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlETAxisgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<TAxis>*)0x0)->GetClass();
      vectorlETAxisgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlETAxisgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlETAxisgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TAxis> : new vector<TAxis>;
   }
   static void *newArray_vectorlETAxisgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<TAxis>[nElements] : new vector<TAxis>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlETAxisgR(void *p) {
      delete ((vector<TAxis>*)p);
   }
   static void deleteArray_vectorlETAxisgR(void *p) {
      delete [] ((vector<TAxis>*)p);
   }
   static void destruct_vectorlETAxisgR(void *p) {
      typedef vector<TAxis> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<TAxis>

namespace ROOT {
   static TClass *maplEJBinIdcOintgR_Dictionary();
   static void maplEJBinIdcOintgR_TClassManip(TClass*);
   static void *new_maplEJBinIdcOintgR(void *p = 0);
   static void *newArray_maplEJBinIdcOintgR(Long_t size, void *p);
   static void delete_maplEJBinIdcOintgR(void *p);
   static void deleteArray_maplEJBinIdcOintgR(void *p);
   static void destruct_maplEJBinIdcOintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const map<JBinId,int>*)
   {
      map<JBinId,int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(map<JBinId,int>));
      static ::ROOT::TGenericClassInfo 
         instance("map<JBinId,int>", -2, "map", 96,
                  typeid(map<JBinId,int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &maplEJBinIdcOintgR_Dictionary, isa_proxy, 0,
                  sizeof(map<JBinId,int>) );
      instance.SetNew(&new_maplEJBinIdcOintgR);
      instance.SetNewArray(&newArray_maplEJBinIdcOintgR);
      instance.SetDelete(&delete_maplEJBinIdcOintgR);
      instance.SetDeleteArray(&deleteArray_maplEJBinIdcOintgR);
      instance.SetDestructor(&destruct_maplEJBinIdcOintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::MapInsert< map<JBinId,int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const map<JBinId,int>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *maplEJBinIdcOintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const map<JBinId,int>*)0x0)->GetClass();
      maplEJBinIdcOintgR_TClassManip(theClass);
   return theClass;
   }

   static void maplEJBinIdcOintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_maplEJBinIdcOintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<JBinId,int> : new map<JBinId,int>;
   }
   static void *newArray_maplEJBinIdcOintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) map<JBinId,int>[nElements] : new map<JBinId,int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_maplEJBinIdcOintgR(void *p) {
      delete ((map<JBinId,int>*)p);
   }
   static void deleteArray_maplEJBinIdcOintgR(void *p) {
      delete [] ((map<JBinId,int>*)p);
   }
   static void destruct_maplEJBinIdcOintgR(void *p) {
      typedef map<JBinId,int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class map<JBinId,int>

namespace {
  void TriggerDictionaryInitialization_TH2JaggedDict_Impl() {
    static const char* headers[] = {
"TH2Jagged.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/larsoft.opensciencegrid.org/products/root/v6_12_06a/Linux64bit+3.10-2.17-e15-prof/include",
"/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Ext/src/TH2Jagged_ext-build/src/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "TH2JaggedDict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
template <typename THT> struct __attribute__((annotate("$clingAutoload$TH2Jagged.h")))  TH2TypeTraits;

struct __attribute__((annotate(R"ATTRDUMP(file_name@@@TH2Jagged.h)ATTRDUMP"))) __attribute__((annotate(R"ATTRDUMP(pattern@@@*)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$TH2Jagged.h")))  JBinId;
template <typename ST> class __attribute__((annotate("$clingAutoload$TH2Jagged.h")))  TH2Jagged;

using TH2JaggedD __attribute__((annotate("$clingAutoload$TH2Jagged.h")))  = TH2Jagged<double>;
using TH2JaggedF __attribute__((annotate("$clingAutoload$TH2Jagged.h")))  = TH2Jagged<float>;
using TH2JaggedI __attribute__((annotate("$clingAutoload$TH2Jagged.h")))  = TH2Jagged<int>;
using TH2JaggedC __attribute__((annotate("$clingAutoload$TH2Jagged.h")))  = TH2Jagged<char>;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "TH2JaggedDict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TH2Jagged.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"", payloadCode, "@",
"JBinId", payloadCode, "@",
"TH2Jagged::fgIsA", payloadCode, "@",
"TH2Jagged<char>", payloadCode, "@",
"TH2Jagged<char>::fgIsA", payloadCode, "@",
"TH2Jagged<double>", payloadCode, "@",
"TH2Jagged<double>::fgIsA", payloadCode, "@",
"TH2Jagged<float>", payloadCode, "@",
"TH2Jagged<float>::fgIsA", payloadCode, "@",
"TH2Jagged<int>", payloadCode, "@",
"TH2Jagged<int>::fgIsA", payloadCode, "@",
"TH2JaggedC", payloadCode, "@",
"TH2JaggedD", payloadCode, "@",
"TH2JaggedF", payloadCode, "@",
"TH2JaggedI", payloadCode, "@",
"TH2TypeTraits<char>", payloadCode, "@",
"TH2TypeTraits<double>", payloadCode, "@",
"TH2TypeTraits<float>", payloadCode, "@",
"TH2TypeTraits<int>", payloadCode, "@",
"ToTHJaggedF", payloadCode, "@",
"operator<", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TH2JaggedDict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TH2JaggedDict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TH2JaggedDict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TH2JaggedDict() {
  TriggerDictionaryInitialization_TH2JaggedDict_Impl();
}
