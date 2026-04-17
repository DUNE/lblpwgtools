#include "CAFAna/Vars/FitVarsNSI.h"
#include "OscLib/OscCalcPMNS_NSI.h"

#include <cassert>
#include <cmath>

namespace ana 
{
    
  // declared as 'extern' in FitVarsNSI.h
  
  // Eps_ee
  const FitEpsEE kFitEpsEE;
  // Eps_mumu
  const FitEpsMuMu kFitEpsMuMu;
  // Eps_tautau
  const FitEpsTauTau kFitEpsTauTau;   
    
  // Eps_mutau
  const FitEpsMuTau kFitEpsMuTau;
  const FitDeltaMuTauInPiUnits kFitDeltaMuTauInPiUnits;
  // Eps_emu
  const FitEpsEMu kFitEpsEMu;
  const FitDeltaEMuInPiUnits kFitDeltaEMuInPiUnits;
  // Eps_etau
  const FitEpsETau kFitEpsETau;
  const FitDeltaETauInPiUnits kFitDeltaETauInPiUnits;
  
  
  // Eps_mutau Real
  const FitEpsMuTauReal kFitEpsMuTauReal;
  // Eps_emu Real
  const FitEpsEMuReal kFitEpsEMuReal;
  // Eps_etau Real
  const FitEpsETauReal kFitEpsETauReal;
    
//#####################################################################
//                        Diagonal parameters                         #
//#####################################################################  
  
//#*******************************************************************#
//#                                                                   #
//#                 		    Eps_ee                 	                  #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitEpsEE::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToNSI(osc)->GetEps_ee();
  }

  //----------------------------------------------------------------------
  void FitEpsEE::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetEps_ee(Clamp(val));
  }  



//#*******************************************************************#
//#                                                                   #
//#                 	    	Eps_mumu               	                  #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitEpsMuMu::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToNSI(osc)->GetEps_mumu();
  }

  //----------------------------------------------------------------------
  void FitEpsMuMu::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetEps_mumu(Clamp(val));
  }
  
  
//#*******************************************************************#
//#                                                                   #
//#                 	    	Eps_tautau            	                  #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitEpsTauTau::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToNSI(osc)->GetEps_tautau();
  }

  //----------------------------------------------------------------------
  void FitEpsTauTau::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetEps_tautau(Clamp(val));
  }  

  

//#####################################################################
//                     Off-diagonal parameters                        #
//#####################################################################  
  
//#*******************************************************************#
//#                                                                   #
//#                     		|Eps_mutau|            	                  #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitEpsMuTau::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToNSI(osc)->GetEps_mutau();
  }

  //----------------------------------------------------------------------
  void FitEpsMuTau::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetEps_mutau(Clamp(val));
  }
  

  
//#*******************************************************************#
//#                                                                   #
//#               		         delta_mutau    	                      #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitDeltaMuTauInPiUnits::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    double ret_mutau = osc::DowncastToNSI(osc)->GetDelta_mutau()/M_PI;

    // convert to the range 0-2
    long long int a_mutau = (ret_mutau)/2+1;
    ret_mutau -= 2*(a_mutau);
    // Instead of figuring out all the rounding just nudge the last little bit
    while(ret_mutau < 0) ret_mutau += 2;
    while(ret_mutau > 2) ret_mutau -= 2;

    return ret_mutau;
  }

  //----------------------------------------------------------------------
  void FitDeltaMuTauInPiUnits::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetDelta_mutau(M_PI*val);
  }

  
  
//#*******************************************************************#
//#                                                                   #
//#                 	    	|Eps_emu|             	                  #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitEpsEMu::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToNSI(osc)->GetEps_emu();
  }

  //----------------------------------------------------------------------
  void FitEpsEMu::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetEps_emu(Clamp(val));
  }
  
//#*******************************************************************#
//#                                                                   #
//#               		        delta_emu           	                  #
//#                                                                   #
//#*******************************************************************#  
  
  //---------------------------------------------------------------------------                   
  double FitDeltaEMuInPiUnits::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    double ret_emu = osc::DowncastToNSI(osc)->GetDelta_emu()/M_PI;

    // convert to the range 0-2
    long long int a_emu = (ret_emu)/2+1;
    ret_emu -= 2*(a_emu);
    // Instead of figuring out all the rounding just nudge the last little bit
    while(ret_emu < 0) ret_emu += 2;
    while(ret_emu > 2) ret_emu -= 2;

    return ret_emu;
  }

  //----------------------------------------------------------------------
  void FitDeltaEMuInPiUnits::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetDelta_emu(M_PI*val);
  }
  
//#*******************************************************************#
//#                                                                   #
//#                 	    	|Eps_etau|            	                  #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitEpsETau::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToNSI(osc)->GetEps_etau();
  }

  //----------------------------------------------------------------------
  void FitEpsETau::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetEps_etau(Clamp(val));
  }
  
//#*******************************************************************#
//#                                                                   #
//#               		       delta_etau           	                  #
//#                                                                   #
//#*******************************************************************#  
  
  //---------------------------------------------------------------------------                   
  double FitDeltaETauInPiUnits::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    double ret_etau = osc::DowncastToNSI(osc)->GetDelta_etau()/M_PI;

    // convert to the range 0-2
    long long int a_etau = (ret_etau)/2+1;
    ret_etau -= 2*(a_etau);
    // Instead of figuring out all the rounding just nudge the last little bit
    while(ret_etau < 0) ret_etau += 2;
    while(ret_etau > 2) ret_etau -= 2;

    return ret_etau;
  }

  //----------------------------------------------------------------------
  void FitDeltaETauInPiUnits::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetDelta_etau(M_PI*val);
  }   

//#####################################################################
//                      NSI as real parameters                        #
//#####################################################################  
  
//#*******************************************************************#
//#                                                                   #
//#                 		   Eps_mutau Real         	                  #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitEpsMuTauReal::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToNSI(osc)->GetEps_mutau();
  }

  //----------------------------------------------------------------------
  void FitEpsMuTauReal::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetEps_mutau(Clamp(val));
  }  
  
//#*******************************************************************#
//#                                                                   #
//#                 		    Eps_emu Real           	                  #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitEpsEMuReal::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToNSI(osc)->GetEps_emu();
  }

  //----------------------------------------------------------------------
  void FitEpsEMuReal::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetEps_emu(Clamp(val));
  }  
  
//#*******************************************************************#
//#                                                                   #
//#                 	    	 Eps_etau Real         	                  #
//#                                                                   #
//#*******************************************************************#
  
  //---------------------------------------------------------------------------                   
  double FitEpsETauReal::GetValue(const osc::IOscCalcAdjustable* osc) const
  {
    return osc::DowncastToNSI(osc)->GetEps_etau();
  }

  //----------------------------------------------------------------------
  void FitEpsETauReal::SetValue(osc::IOscCalcAdjustable* osc, double val) const
  {
    osc::DowncastToNSI(osc)->SetEps_etau(Clamp(val));
  }  
  
} // namespace