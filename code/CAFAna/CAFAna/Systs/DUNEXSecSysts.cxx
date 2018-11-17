#include "CAFAna/Systs/DUNEXSecSysts.h"

#include "StandardRecord/StandardRecord.h"

#include "Utilities/func/MathUtil.h"

#include "TFile.h"
#include "TH2.h"
#include "TObjString.h"

namespace ana
{
  //----------------------------------------------------------------------
  const std::string valor_categories[33] = { 
    "nu_ccqe_1",       // 0
    "nu_ccqe_2",       // 1
    "nu_ccqe_3",       // 2
    "nubar_ccqe_1",    // 3
    "nubar_ccqe_2",    // 4
    "nubar_ccqe_3",    // 5
    "nu_MEC_dummy",    // 6
    "nubar_MEC_dummy", // 7
    "nu_cc1piz_1",     // 8
    "nu_cc1piz_2",     // 9
    "nu_cc1piz_3",     // 10
    "nu_cc1pic_1",     // 11
    "nu_cc1pic_2",     // 12
    "nu_cc1pic_3",     // 13
    "nubar_cc1piz_1",  // 14
    "nubar_cc1piz_2",  // 15
    "nubar_cc1piz_3",  // 16
    "nubar_cc1pic_1",  // 17
    "nubar_cc1pic_2",  // 18
    "nubar_cc1pic_3",  // 19
    "nu_2pi",          // 20
    "nubar_2pi",       // 21
    "nu_dis_1",        // 22
    "nu_dis_2",        // 23
    "nu_dis_3",        // 24
    "nubar_dis_1",     // 25
    "nubar_dis_2",     // 26
    "nubar_dis_3",     // 27
    "nu_coh",          // 28
    "nubar_coh",       // 29
    "nu_nc",           // 30
    "nubar_nc",        // 31
    "nuenumu_dummy" }; // 32

  const std::string valor_categories_long[33] =
    { "#nu CCQE 1",       // 0
      "#nu CCQE 2",       // 1
      "#nu CCQE 3",       // 2
      "#bar{#nu} CCQE 1",    // 3
      "#bar{#nu} CCQE 2",    // 4
      "#bar{#nu} CCQE 3",    // 5
      "#nu MEC dummy",    // 6
      "#bar{#nu} MEC dummy", // 7
      "#nu CC1#pi^{0} 1",     // 8
      "#nu CC1#pi^{0} 2",     // 9
      "#nu CC1#pi^{0} 3",     // 10
      "#nu CC1#pi^{#pm} 1",     // 11
      "#nu CC1#pi^{#pm} 2",     // 12
      "#nu CC1#pi^{#pm} 3",     // 13
      "#bar{#nu} CC1#pi^{0} 1",  // 14
      "#bar{#nu} CC1#pi^{0} 2",  // 15
      "#bar{#nu} CC1#pi^{0} 3",  // 16
      "#bar{#nu} CC1#pi^{#pm} 1",  // 17
      "#bar{#nu} CC1#pi^{#pm} 2",  // 18
      "#bar{#nu} CC1#pi^{#pm} 3",  // 19
      "#nu 2#pi",          // 20
      "#bar{#nu} 2#pi",       // 21
      "#nu DIS 1",        // 22
      "#nu DIS 2",        // 23
      "#nu DIS 3",        // 24
      "#bar{#nu} DIS 1",     // 25
      "#bar{#nu} DIS 2",     // 26
      "#bar{#nu} DIS 3",     // 27
      "#nu COH",          // 28
      "#bar{#nu} COH",       // 29
      "#nu NC",           // 30
      "#bar{#nu} NC",        // 31
      "#nu_{e}/#nu_{#mu} dummy" }; // 32

  //----------------------------------------------------------------------
  std::string VALORCategoryName(EVALORCategory c)
  {
    return valor_categories[c];
  }

  //----------------------------------------------------------------------
  std::string VALORCategoryLatexName(EVALORCategory c)
  {
    return valor_categories_long[c];
  }

  //----------------------------------------------------------------------
  EVALORCategory GetVALORCategory(const caf::StandardRecord* sr)
  {
    int lep = sr->dune.LepPDG;
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

    // Treat nutaus as if they were numus for now
    if(lep == -16) lep = -14;
    if(lep == -15) lep = -13;
    if(lep == +15) lep = +13;
    if(lep == +16) lep = +14;

    // Treat nues as numus too. So basically we shouldn't be switching on
    // category at all. The 32 at the bottom there is the totally independent
    // nue scale factor.
    if(lep == -12) lep = -14;
    if(lep == -11) lep = -13;
    if(lep == +11) lep = +13;
    if(lep == +12) lep = +14;

    // these have no cross section uncertainty, so it's OK to leave them as -1
    if(scat == 7) return EVALORCategory(-1);
    if(scat == 8) return EVALORCategory(-1);

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

    if(vc == -1){
      std::cout << "Could not determine VALOR category for event with\n"
                << "lep:  " << lep << std::endl
                << "scat: " << scat << std::endl
                << "npiz: " << npiz << std::endl
                << "npic: " << npic << std::endl
                << "Enu:  " << Enu << std::endl
                << "Q2:   " << Q2 << std::endl
                << "Returning -1. Please come and fix this!" << std::endl;
    }

    return EVALORCategory(vc);
  }

  //----------------------------------------------------------------------
  DUNEXSecSyst::DUNEXSecSyst(EVALORCategory cat)
    : SystComponentScale(VALORCategoryName(cat)+"_scale",
                         VALORCategoryLatexName(cat)+" Scale",
                         kVALORCategory == cat, 0)
  {
    // explicitly set sigma to 0 for category -1
    // currently used for pure EW processes with ~0 XS uncertainty
    if( cat == EVALORCategory(-1) ) fOneSigma = 0.;
    else {

      // TODO - official location
      TFile f("/dune/data/users/marshalc/total_covariance_XS.root");

      TH2* h = (TH2*)f.Get("xs_covariance");

      fOneSigma = sqrt( h->GetBinContent(int(cat)+1, int(cat)+1) );
    }
  }

  //----------------------------------------------------------------------
  void DUNEXSecSyst::SaveTo(TDirectory* dir) const
  {
    TDirectory* tmp = gDirectory;
    dir->cd();

    TObjString("DUNEXSecSyst").Write("type");
    TObjString(ShortName().c_str()).Write("name");

    tmp->cd();
  }

  //----------------------------------------------------------------------
  std::unique_ptr<DUNEXSecSyst> DUNEXSecSyst::LoadFrom(TDirectory* dir)
  {
    TObjString* pname = (TObjString*)dir->Get("name");
    assert(pname);

    for(int i = 0; i < 32; ++i){
      const DUNEXSecSyst* s = GetDUNEXSecSyst(EVALORCategory(i));
      if(s->ShortName() == pname->GetString())
        return std::unique_ptr<DUNEXSecSyst>((DUNEXSecSyst*)s);
    }
    std::cout << "DUNEXSecSyst::LoadFrom(): unknown name " << pname->GetString() << std::endl;
    abort();
  }

  //----------------------------------------------------------------------
  const DUNEXSecSyst* GetDUNEXSecSyst(EVALORCategory cat)
  {
    // Make sure we always give the same one back
    static std::vector<const DUNEXSecSyst*> scs(32);
    if(!scs[cat]) scs[cat] = new DUNEXSecSyst(cat);
    return scs[cat];
  }

  //----------------------------------------------------------------------
  SystVector<DUNEXSecSyst> GetDUNEXSecSysts()
  {
    SystVector<DUNEXSecSyst> ret;
    for(int i = 0; i < 32; ++i) ret.push_back(GetDUNEXSecSyst(EVALORCategory(i)));
    return ret;
  }

  const int N = 32;

  //----------------------------------------------------------------------
  DUNEXSecCorrelation::DUNEXSecCorrelation()
    : fMatrix(N, N)
  {
    fSysts = GetDUNEXSecSysts();
    assert(fSysts.size() == N);

    TFile f("/dune/data/users/marshalc/total_covariance_XS.root");

    TH2* h = (TH2*)f.Get("xs_covariance");
    EnsurePositiveDefinite(h);

    for(int i = 0; i < N; ++i){
      fScales.push_back(sqrt(h->GetBinContent(i+1, i+1)));
      for(int j = 0; j < N; ++j){
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
    // Don't double-count diagonal terms which SystShifts::Penalty() already
    // applied
    for(int i = 0; i < N; ++i) ret -= util::sqr(syst.GetShift(fSysts[i]));

    // Scale the shift vector from sigmas into fractional shifts
    TVectorD v(N);
    for(int i = 0; i < N; ++i)
      v[i] = syst.GetShift(fSysts[i]) * fScales[i];

    return ret + fMatrix.Similarity(v);
  }
} // namespace
