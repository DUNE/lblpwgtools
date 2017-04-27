#include "CAFAna/Systs/DUNEXSecSysts.h"

#include "StandardRecord/StandardRecord.h"

#include "Utilities/func/MathUtil.h"

#include "TFile.h"
#include "TH2.h"

namespace ana
{
  //----------------------------------------------------------------------
  const std::string valor_categories[33] = { "f_int_nu_ccqe_1",       // 0
                                             "f_int_nu_ccqe_2",       // 1
                                             "f_int_nu_ccqe_3",       // 2
                                             "f_int_nubar_ccqe_1",    // 3
                                             "f_int_nubar_ccqe_2",    // 4
                                             "f_int_nubar_ccqe_3",    // 5
                                             "f_int_nu_MEC_dummy",    // 6
                                             "f_int_nubar_MEC_dummy", // 7
                                             "f_int_nu_cc1piz_1",     // 8
                                             "f_int_nu_cc1piz_2",     // 9
                                             "f_int_nu_cc1piz_3",     // 10
                                             "f_int_nu_cc1pic_1",     // 11
                                             "f_int_nu_cc1pic_2",     // 12
                                             "f_int_nu_cc1pic_3",     // 13
                                             "f_int_nubar_cc1piz_1",  // 14
                                             "f_int_nubar_cc1piz_2",  // 15
                                             "f_int_nubar_cc1piz_3",  // 16
                                             "f_int_nubar_cc1pic_1",  // 17
                                             "f_int_nubar_cc1pic_2",  // 18
                                             "f_int_nubar_cc1pic_3",  // 19
                                             "f_int_nu_2pi",          // 20
                                             "f_int_nubar_2pi",       // 21
                                             "f_int_nu_dis_1",        // 22
                                             "f_int_nu_dis_2",        // 23
                                             "f_int_nu_dis_3",        // 24
                                             "f_int_nubar_dis_1",     // 25
                                             "f_int_nubar_dis_2",     // 26
                                             "f_int_nubar_dis_3",     // 27
                                             "f_int_nu_coh",          // 28
                                             "f_int_nubar_coh",       // 29
                                             "f_int_nu_nc",           // 30
                                             "f_int_nubar_nc",        // 31
                                             "f_int_nuenumu_dummy" }; // 32

  //----------------------------------------------------------------------
  std::string VALORCategoryName(EVALORCategory c)
  {
    return valor_categories[c];
  }

  //----------------------------------------------------------------------
  EVALORCategory GetVALORCategory(const caf::StandardRecord* sr)
  {
    // TODO interpret FD modes

    const int lep = sr->dune.LepPDG;
    int scat = sr->dune.mode;
    const int npiz = sr->dune.nipi0;
    const int npic = sr->dune.nipip + sr->dune.nipim;
    const float Enu = sr->dune.Ev;
    const float Q2 = sr->dune.Q2;

    // These are the FD codes
    // kUnknownMode               = -1
    // kQE                        = 0
    // kRes                       = 1
    // kDIS                       = 2
    // kCoh                       = 3
    // kCohElastic                = 4
    // kElectronScattering        = 5
    // kIMDAnnihilation           = 6
    // kInverseBetaDecay          = 7
    // kGlashowResonance          = 8
    // kAMNuGamma                 = 9
    // kMEC                       = 10
    // kDiffractive               = 11
    // kEM                        = 12
    // kWeakMix                   = 13

    // ND codes
    // 0 Null,
    // 1 QE
    // 2 SingleKaon
    // 3 DIS
    // 4 Resonant
    // 5 COH
    // 6 Diffractive
    // 7 nu+e
    // 8 IMD
    // 9 AMNuGamma
    // 10 MEC
    // 11 coherent elastic
    // 12 IBD
    // 13 Glashow resonance
    // 14 IMD annihilation

    // Different conventions ND (weird (new?)) and FD (like NOvA)
    if(sr->dune.isFD){
      switch(scat){
      case 0: scat = 1; break; // QE -> QE
      case 1: scat = 4; break; // RES -> RES
      case 2: scat = 3; break; // DIS -> DIS
      case 3: scat = 5; break; // COH -> COH
      case 4: scat = 11; break; // COHElastic
      case 5: scat = 7; break; // nu-on-e
      case 6: scat = 8; break; // IMD
        // InverseBetaDecay can never happen
      case 8: scat = 13; break; // Glashow
      case 9: scat = 9; break; // AMNuGamma
      case 10: scat = 10; break; // MEC
      case 11: scat = 6; break; // diffractive
        // EM and WeakMix can never happen
      default:
        std::cout << "Unhandled GENIE FD code " << scat << std::endl;
        abort();
      }
    }

    int vc = -1;

    // Email from Chris Marshall
    if( lep == 13 ) { // numu CC
      if( scat == 1 ) { // CCQE
        if     ( Q2 < 0.2  ) vc = 0;
        else if( Q2 < 0.55 ) vc = 1;
        else                 vc = 2;
      } else if( scat == 10 ) { // CC MEC
        vc = 6;
      } else if( scat == 4 ) { // CC resonance
        if( npiz == 1 && npic == 0 ) { // CC1pi0
          if      ( Q2 < 0.35 ) vc = 8;
          else if ( Q2 < 0.90 ) vc = 9;
          else                  vc = 10;
        } else if( npiz == 0 && npic == 1 ) { // CC1pic
          if      ( Q2 < 0.30 ) vc = 11;
          else if ( Q2 < 0.80 ) vc = 12;
          else                  vc = 13;
        } else { // CC multi-pi
          vc = 20;
        }
      } else if( scat == 5 ) { // CC Coherent
        vc = 28;
      } else if( scat == 3 ) { // CC DIS
        if     ( Enu < 7.5 ) vc = 22;
        else if( Enu < 15. ) vc = 23;
        else                 vc = 24;
      }
    } else if( lep == -13 ) {
      if( scat == 1 ) { // CCQE
        if     ( Q2 < 0.20 ) vc = 3;
        else if( Q2 < 0.55 ) vc = 4;
        else                 vc = 5;
      } else if( scat == 10 ) { // CC MEC
        vc = 7;
      } else if( scat == 4 ) { // CC resonance
        if( npiz == 1 && npic == 0 ) { // CC1pi0
          if      ( Q2 < 0.35 ) vc = 14;
          else if ( Q2 < 0.90 ) vc = 15;
          else                  vc = 16;
        } else if( npiz == 0 && npic == 1 ) { // CC1pic
          if      ( Q2 < 0.30 ) vc = 17;
          else if ( Q2 < 0.80 ) vc = 18;
          else                  vc = 19;
        } else { // CC multi-pi
          vc = 21;
        }
      } else if( scat == 5 ) { // CC Coherent
        vc = 29;
      } else if( scat == 3 ) { // CC DIS
        if     ( Enu < 7.5 ) vc = 25;
        else if( Enu < 15. ) vc = 26;
        else                 vc = 27;
      }
    } else if( lep == 12 || lep == 14 ) { // nu NC
      vc = 30;
    } else if( lep == -12 || lep == -14 ) { // nubar NC
      vc = 31;
    } else if( lep == 11 ) { // nue CC
      vc = 32;
    } else if( lep == -11 ) { // nuebar CC
      vc = 32;
    }

    assert(vc >= 0);

    return EVALORCategory(vc);
  }

  //----------------------------------------------------------------------
  DUNEXSecSyst::DUNEXSecSyst(EVALORCategory cat)
    : SystComponentScale(VALORCategoryName(cat)+"_scale",
                         VALORCategoryName(cat)+" Scale",
                         kVALORCategory == cat, 0)
  {
    // TODO - official location
    TFile f("/dune/data/users/marshalc/total_covariance_XS.root");

    TH2* h = (TH2*)f.Get("xs_covariance");

    fOneSigma = h->GetBinContent(int(cat)+1, int(cat)+1);
  }


  //----------------------------------------------------------------------
  DUNEXSecCorrelation::DUNEXSecCorrelation(const PredictionScaleComp* psc)
    : fMatrix(32, 32)//, fSysts(psc->GetSysts()) // TODO TODO TODO
  {
    assert(fSysts.size() == 32);

    TFile f("/dune/data/users/marshalc/total_covariance_XS.root");

    TH2* h = (TH2*)f.Get("xs_covariance");

    for(int i = 0; i < 32; ++i){
      fScales.push_back(sqrt(h->GetBinContent(i+1, i+1)));
      for(int j = 0; j < 32; ++j){
        fMatrix(i, j) = h->GetBinContent(i+1, j+1);
      }
    }

    fMatrix.Invert();
  }

  //----------------------------------------------------------------------
  double DUNEXSecCorrelation::ChiSq(osc::IOscCalculatorAdjustable* osc,
                                    const SystShifts& syst) const
  {
    double ret = 0;
    // Don't double-count diagonals
    for(int i = 0; i < 32; ++i) ret -= util::sqr(syst.GetShift(fSysts[i]));

    TVectorD v(32);
    for(int i = 0; i < 32; ++i)
      v[i] = syst.GetShift(fSysts[i]) * fScales[i];

    return ret + fMatrix.Similarity(v);
  }
} // namespace
