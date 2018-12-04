// NDRecoSysts.h
// Systematics to simulate reconstruction systematics in the ND
#pragma once

#include "CAFAna/Core/ISyst.h"
#include "StandardRecord/StandardRecord.h"
#include "CAFAna/Core/Utilities.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TRandom3.h"

#include <cassert>

namespace ana {

  // Take ND events which pass the CC selection cuts but are NC and reweight by 20%
  class RecoNCSyst: public ISyst
  {
  public:
  RecoNCSyst() : ISyst("RecoNCSyst", "ND Neutral Current Reconstruction Syst") {}
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      // Is ND
      if(!sr->dune.isFD) {
	// CC event selection but is NC
	if((sr->dune.reco_numu || sr->dune.reco_nue) && (sr->dune.muon_contained || sr->dune.muon_tracker) && (sr->dune.reco_q == -1 || sr->dune.reco_q == 1) && sr->dune.Ehad_veto<30 && !sr->dune.isCC) {
	  weight *= 1 + .2*sigma;
	}
      }
    }
  };
  extern const RecoNCSyst kRecoNCSyst;

  // Systematic designed to mimic effect of lepton acceptance in the ND
  class LeptonContSyst: public ISyst
  {
  public:
  LeptonContSyst() : ISyst("LeptonContSyst", "ND Lepton Containment Syst") 
      {
	TFile *f = new TFile("/dune/app/users/marshalc/ND_syst/ND_eff_syst.root", "read");
	hist = (TH2*)f->Get("unc");
      }
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {
      const double m_mu = 0.105658;
      // Is ND
      if (!sr->dune.isFD) {
	// Is a true numu CC event
	if (sr->dune.isCC && abs(sr->dune.nuPDG) == 14) {
	  double LepE = sr->dune.LepE;
	  int bin = hist->FindBin(sqrt(LepE*LepE - m_mu*m_mu) * cos(sr->dune.LepNuAngle), sqrt(LepE*LepE - m_mu*m_mu) * sin(sr->dune.LepNuAngle));
	  double w = hist->GetBinContent(bin);
	  weight *= 1. + w*sigma;
	}
      }
    }
  protected:
    TH2 *hist;
    TFile *f;
  };
  extern const LeptonContSyst kLeptonContSyst;

  // Systematic designed to mimic effect of hadron acceptance in the ND
  class HadronContSyst: public ISyst
  {
  public:
  HadronContSyst() : ISyst("HadronContSyst", "ND Hadron Containment Syst") 
      {
	TFile *f = new TFile("/dune/app/users/marshalc/ND_syst/ND_eff_syst.root", "read");
	hist = (TH1*)f->Get("hunc");
      }
    void Shift(double sigma,
	       Restorer& restore,
	       caf::StandardRecord* sr, double& weight) const override
    {

      // All ND events
      if (!sr->dune.isFD) {
	double HadE = sr->dune.Ev - sr->dune.LepE;
	int bin = hist->FindBin(HadE);
	double w = hist->GetBinContent(bin);
	weight *= 1. + w*sigma;
      }
    }
  protected:
    TH1 *hist;
    TFile *f;
  };
  extern const HadronContSyst kHadronContSyst;
}
