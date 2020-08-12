#include "CAFAna/Core/LoadFromFile.h"

#include "OscLib/OscCalc.h"
#include "OscLib/OscCalcDumb.h"
#include "OscLib/OscCalcGeneral.h"
#include "OscLib/OscCalcPMNS.h"
#include "OscLib/OscCalcPMNSOpt.h"
#include "OscLib/OscCalcSterile.h"
#include "OscLib/OscCalcPMNS_NSI.h"

#include "TObjString.h"
#include "TH1.h"
#include "TVectorD.h"
#include <vector>

namespace ana
{
  //----------------------------------------------------------------------
  template<> std::unique_ptr<osc::IOscCalc>
  LoadFrom<osc::IOscCalc>(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    // There are a lot of ways to exit this function. Make sure they all write
    // and delete the directory.
    struct DirCleaner
    {
      ~DirCleaner(){fDir->Write(); delete fDir;}
      TDirectory* fDir;
    } cleaner{dir};

    TObjString* ptag = (TObjString*)dir->Get("type");
    assert(ptag);
    const TString tag = ptag->GetString();

    if(tag == "NoOscillations") return std::unique_ptr<osc::IOscCalc>(new osc::NoOscillations);
    if(tag == "OscCalcDumb") return std::unique_ptr<osc::IOscCalc>(new osc::OscCalcDumb);

    osc::IOscCalcAdjustable* ret = 0;

    if(tag == "OscCalculator") ret = new osc::OscCalc;
    if(tag == "OscCalculatorGeneral") ret = new osc::OscCalcGeneral;
    if(tag == "OscCalculatorPMNS") ret = new osc::OscCalcPMNS;
    if(tag == "OscCalculatorPMNSOpt") ret = new osc::OscCalcPMNSOpt;
    if(tag == "OscCalculatorSterile") ret = new osc::OscCalcSterile;
    if(tag == "OscCalculatorPMNS_NSI") ret = new osc::OscCalcPMNS_NSI;

    if(!ret){
      std::cout << "LoadFrom not implemented for " << tag << std::endl;
      abort();
    }

    TVectorD* params = (TVectorD*)dir->Get("params");
    assert(params);
    //special case how OscCalcSterile is initialized
    if(tag == "OscCalculatorSterile") {
      std::vector<double> state;
      for (int i = 0; i < params->GetNrows(); ++i) {
	state.push_back( (*params)[i] );
      }
      dynamic_cast<osc::OscCalcSterile*>(ret)->SetState(state);
      return std::unique_ptr<osc::IOscCalcAdjustable>(ret);
    }
    //special case how OscCalcPMNS_NSI is initialized
    if(tag == "OscCalculatorPMNS_NSI") {
      std::vector<double> state;
      for (int i = 0; i < params->GetNrows(); ++i) {
	state.push_back( (*params)[i] );
      }
      dynamic_cast<osc::OscCalcPMNS_NSI*>(ret)->SetState(state);
      return std::unique_ptr<osc::IOscCalcAdjustable>(ret);
    }

    assert(params->GetNrows() == 8);

    ret->SetL     ((*params)[0]);
    ret->SetRho   ((*params)[1]);
    ret->SetDmsq21((*params)[2]);
    ret->SetDmsq32((*params)[3]);
    ret->SetTh12  ((*params)[4]);
    ret->SetTh13  ((*params)[5]);
    ret->SetTh23  ((*params)[6]);
    ret->SetdCP   ((*params)[7]);

    return std::unique_ptr<osc::IOscCalcAdjustable>(ret);
  }

  //----------------------------------------------------------------------
  template<> void SaveTo(const osc::IOscCalc& x, TDirectory* dir, const std::string& name)
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    // There are a lot of ways to exit this function. Make sure they all write
    // and delete the directory.
    struct DirCleaner
    {
      ~DirCleaner(){fDir->Write(); delete fDir;}
      TDirectory* fDir;
    } cleaner{dir};


    if(dynamic_cast<const osc::NoOscillations*>(&x)){
      TObjString("NoOscillations").Write("type");
      tmp->cd();
      return;
    }

    if(dynamic_cast<const osc::OscCalcDumb*>(&x)){
      TObjString("OscCalculatorDumb").Write("type");
      tmp->cd();
      return;
    }

    const osc::IOscCalcAdjustable* y = dynamic_cast<const osc::IOscCalcAdjustable*>(&x);
    if(!y){
      std::cout << "Unknown calculator in SaveTo " << typeid(x).name() << std::endl;
      abort();
    }
    
    const osc::OscCalcSterile* tmpSterile = dynamic_cast<const osc::OscCalcSterile*>(&x);
    if(tmpSterile) {
      TObjString("OscCalculatorSterile").Write("type");      
      std::vector<double> state = tmpSterile->GetState();
      TVectorD params(state.size());
      for (unsigned int i = 0; i < state.size(); ++i) params[i] = state[i];
      params.Write("params");
      tmp->cd();
      return;
    }

    //for the implementation of OscCalcPMNS_NSI
    const osc::OscCalcPMNS_NSI* tmpNSI = dynamic_cast<const osc::OscCalcPMNS_NSI*>(&x);
    if(tmpNSI) {
      TObjString("OscCalculatorPMNS_NSI").Write("type");      
      std::vector<double> state = tmpNSI->GetState();
      TVectorD params(state.size());
      for (unsigned int i = 0; i < state.size(); ++i) params[i] = state[i];
      params.Write("params");
      tmp->cd();
      return;
    }

    /* */if(dynamic_cast<const osc::OscCalc*>(&x)) TObjString("OscCalculatorPMNS").Write("type");
    else if(dynamic_cast<const osc::OscCalcGeneral*>(&x)) TObjString("OscCalculatorGeneral").Write("type");
    else if(dynamic_cast<const osc::OscCalcPMNS*>(&x)) TObjString("OscCalculatorPMNS").Write("type");
    else if(dynamic_cast<const osc::OscCalcPMNSOpt*>(&x)) TObjString("OscCalculatorPMNSOpt").Write("type");
    else{
      std::cout << "Unimplemented calculator in SaveTo " << typeid(x).name() << std::endl;
      abort();
    }
    
    TVectorD params(8);

    params[0] = y->GetL();
    params[1] = y->GetRho();
    params[2] = y->GetDmsq21();
    params[3] = y->GetDmsq32();
    params[4] = y->GetTh12();
    params[5] = y->GetTh13();
    params[6] = y->GetTh23();
    params[7] = y->GetdCP();

    params.Write("params");

    tmp->cd();
  }
}
