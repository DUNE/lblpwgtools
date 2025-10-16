#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Extrap/ModularExtrapComponent.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Decomp/IDecomp.h"
#include "CAFAna/Core/Ratio.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Vars/Vars.h"

#include "TH1.h"
#include "TH3.h"
#include "TDirectory.h"
#include "TObjString.h"

#include <iostream>

namespace ana
{
  //---------------------------------------------------------------------------
  // Definition to satisfy declaration in Core/LoadFromFile.h
  template<> std::unique_ptr<ModularExtrapComponent>
    LoadFrom<ModularExtrapComponent>(TDirectory* dir, const std::string& name)
  {
    TObjString* ptag = (TObjString*)dir->Get((name+"/type").c_str());
    assert(ptag);

    const TString tag = ptag->GetString();
    delete ptag;

    if(tag == "NoReweight")
      return NoReweight::LoadFrom(dir, name);
    if(tag == "TruthReweight")
      return TruthReweight::LoadFrom(dir, name);
    if(tag == "RecoReweight")
      return RecoReweight::LoadFrom(dir, name);

    std::cerr << "Unknown Component Extrapolation type '"
              << tag << "'" << std::endl;
    abort();
  }

  //---------------------------------------------------------------------------

  ModularExtrapComponent::DivByZeroCounter::~DivByZeroCounter()
  {
    if (fQuiet)
      return;

    if (fBins.empty())
      return;

    std::cerr << "\nWARNING: There were attempted divisions by empty bins (for which a fallback to no reweighting was used)"
              << "\n         during extrapolation in the following bins (check your MC stats):"
              << std::endl;

    for (const auto & tuple : this->fBins)
    {
      std::cerr << "\t Channel: "  << std::get<0>(tuple)
                << "\t\t Location: "   << std::get<1>(tuple)
                << "\t\t Bin Index: " << std::get<2>(tuple)
                << std::endl;
    }

  }

  //---------------------------------------------------------------------------

  const OscillatableSpectrum& ModularExtrapComponent::Return() const
  {
    if (!fEvaluated)
    {
      fCache = Eval();
      fEvaluated = true;
    }
    return fCache;
  }

  //---------------------------------------------------------------------------

  Spectrum ModularExtrapComponent::GetDecompResult(
    const IDecomp& decomp,
    const DecompResult dr
  ){
    switch (dr){
      case DecompResult::nue     : return decomp.NueComponent();
      case DecompResult::numu    : return decomp.NumuComponent();
      case DecompResult::nuebar  : return decomp.AntiNueComponent();
      case DecompResult::numubar : return decomp.AntiNumuComponent();
      case DecompResult::NCtot      : return decomp.NCTotalComponent();
      case DecompResult::NCbar      : return decomp.NCAntiComponent();
      case DecompResult::NC      : return decomp.NCComponent();
    }
    assert( 0 && "Bad DecompResult" );
    throw;
  }

  //---------------------------------------------------------------------------

  std::string ModularExtrapComponent::DRToString(
    const DecompResult dr
  ){
    switch (dr){
      case DecompResult::nue     : return "Nue";
      case DecompResult::numu    : return "Numu";
      case DecompResult::nuebar  : return "NueBar";
      case DecompResult::numubar : return "NumuBar";
      case DecompResult::NC      : return "NC";
      case DecompResult::NCbar      : return "NCbar";
      case DecompResult::NCtot      : return "NCtot";
    }
    assert( 0 && "Bad DecompResult" );
    throw;
  }

  //---------------------------------------------------------------------------

  DecompResult ModularExtrapComponent::StringToDR(
    const std::string str
  ){
    if      (str=="Nue")     return DecompResult::nue;
    else if (str=="Numu")    return DecompResult::numu;
    else if (str=="NueBar")  return DecompResult::nuebar;
    else if (str=="NumuBar") return DecompResult::numubar;
    else if (str=="NC")      return DecompResult::NC;
    else if (str=="NCbar")      return DecompResult::NCbar;
    else if (str=="NCtot")      return DecompResult::NCtot;
    else assert( 0 && "Bad DecompResult String" );
    throw;
  }

  //---------------------------------------------------------------------------

  Ratio ModularExtrapComponent::FormSmartRatio(
    const Spectrum& num,
    const Spectrum& denom,
    const std::string component,
    const std::string location,
    const Spectrum& mult
  ){
    const Eigen::ArrayXd numa = num.GetEigen(1e20);
    const Eigen::ArrayXd denoma = denom.GetEigen(1e20);
    const Eigen::ArrayXd multa = mult.GetEigen(1e20);

    Eigen::ArrayXd ratioa = numa;
    ratioa.setZero();

    assert( (numa.size() == denoma.size()) && "Bin Mismatch" );
    assert( (numa.size() == multa.size()) && "Bin Mismatch" );

    static DivByZeroCounter counter;
    for (int bin = 0; bin < numa.size(); ++bin)
    {
      if ( denoma[bin] != 0 ){
        ratioa[bin] = numa[bin] / denoma[bin];
      } else {
        ratioa[bin] = 1;
        if (    numa[bin] != 0
             || multa[bin] != 0 )
          counter.fBins.insert(std::make_tuple(component, location, bin));
      }
    }

    return Ratio( std::move(ratioa), num.GetLabels(), num.GetBinnings() );
  }

  bool ModularExtrapComponent::fQuiet = false;

  //---------------------------------------------------------------------------
  NoReweight::NoReweight(IInteractionSource& src,
                         const HistAxis& axis,
                         const Cut& fdcut,
                         const SystShifts& shiftMC,
                         const Weight& weight,
                         const Cut& flavors,
                         IInteractionSource& srcExtra1,
                         IInteractionSource& srcExtra2)
   : fRecoFD(src[fdcut && flavors], axis)
  {
    // TODO: Reintroduce.
    /*
    extraloaderswap.AddReweightableSpectrum(
      fRecoFD,     axis.GetVar1D(), kTrueE, fdcut && flavors, kNoShift, kUnweighted );
    extraloadertau.AddReweightableSpectrum(
      fRecoFD,     axis.GetVar1D(), kTrueE, fdcut && flavors, kNoShift, kUnweighted );
    */
  }

  void NoReweight::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("NoReweight").Write("type");
    fRecoFD.SaveTo(dir, "RecoFD");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  std::unique_ptr<NoReweight> NoReweight::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    std::unique_ptr<OscillatableSpectrum> s = OscillatableSpectrum::LoadFrom(dir, "RecoFD");

    delete dir;

    return std::unique_ptr<NoReweight>(new NoReweight(*s));
  }

  //---------------------------------------------------------------------------
  TruthReweight::TruthReweight(
    SpectrumLoader& ndloader,
    const HistAxis& axisFD,
    const HistAxis& axisND,
    const Cut& fdcut,
    ana::RecoType recoFDIxnType,
    const SystShifts& shiftMC,
    const Weight& weight,
    std::string label,
    std::string latex,
    const Cut& ndcut,
    ana::RecoType recoNDIxnType,
    const IDecomp& decomposition,
    const DecompResult dr,
    const Cut& ndflavor,
    SpectrumLoader& fdloader,
    const Cut& fdflavors
  )
    : fRecoToTrueND( ndloader.Interactions(recoNDIxnType)[ndcut && ndflavor], axisND ),
      fTrueToRecoFD( fdloader.Interactions(recoFDIxnType)[fdcut && fdflavors], axisFD ),
      fDecomp(decomposition),
      fOwnDecomp(false),
      fDecompRes(dr),
      fLabel(label),
      fLatex(latex)
  {}

  TruthReweight::~TruthReweight()
  {
    if(fOwnDecomp) delete &fDecomp;
  }

  OscillatableSpectrum TruthReweight::Eval() const
  {

    //Copy to local variables because reweighting is in-place
    OscillatableSpectrum recoToTrueND(fRecoToTrueND);
    OscillatableSpectrum trueToRecoFD(fTrueToRecoFD);

    //Get ND data from Decomp
    Spectrum decompresult(GetDecompResult(fDecomp,fDecompRes));

    //Compute Data/MC Ratio in reco energy bins to get divide-by-zero warnings
    FormSmartRatio(
      decompresult, fRecoToTrueND.Unoscillated(),
      fLabel, "MC ND Reco",
      fRecoToTrueND.Unoscillated() );

    //ND Reco->True
    recoToTrueND.ReweightToRecoSpectrum( decompresult );

    //Compute Data/MC Ratio in true energy bins
    Ratio dataMCtrue = FormSmartRatio(
      recoToTrueND.TrueEnergy(), fRecoToTrueND.TrueEnergy(),
      fLabel, "MC ND Truth",
      fTrueToRecoFD.TrueEnergy() );

    // Multiply by Data/MC Ratio and add in FD truth information
    trueToRecoFD.ReweightToTrueSpectrum(   fTrueToRecoFD.TrueEnergy()
                                         * dataMCtrue );

    return trueToRecoFD;

  }

  void TruthReweight::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("TruthReweight").Write("type");
    fRecoToTrueND.SaveTo(dir, "RecoToTrueND");
    fTrueToRecoFD.SaveTo(dir, "TrueToRecoFD");
    fDecomp.SaveTo(dir, "Decomp");
    TObjString(DRToString(fDecompRes).c_str()).Write("DecompRes");
    TObjString(fLabel.c_str()).Write("Label");
    TObjString(fLatex.c_str()).Write("Latex");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  std::unique_ptr<TruthReweight>
    TruthReweight::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    TObjString* dr = (TObjString*)dir->Get("DecompRes");
    assert(dr);
    TObjString* label = (TObjString*)dir->Get("Label");
    TObjString* latex = (TObjString*)dir->Get("Latex");
    assert(label);
    assert(latex);

    TruthReweight* ret = new TruthReweight(
      *(OscillatableSpectrum::LoadFrom(dir, "RecoToTrueND")),
      *(OscillatableSpectrum::LoadFrom(dir, "TrueToRecoFD")),
      *(ana::LoadFrom<IDecomp>(dir, "Decomp").release()),
      StringToDR(dr->GetString().Data()),
      label->GetString().Data(),
      latex->GetString().Data()
    );
    // We know we have the only copy because we just loaded it
    ret->fOwnDecomp = true;

    delete dir;

    delete label;
    delete latex;
    delete dr;
    return std::unique_ptr<TruthReweight>(ret);
  }

  //---------------------------------------------------------------------------

  RecoReweight::RecoReweight(
    SpectrumLoader& ndloader,
    const HistAxis& axis,
    const Cut& fdcut,
    const SystShifts& shiftMC,
    const Weight& weight,
    std::string label,
    std::string latex,
    const Cut& ndcut,
    const IDecomp& decomposition,
    const DecompResult dr,
    const Cut& ndflavor,
    ana::RecoType recoNDIxnType,
    SpectrumLoader& fdloader,
    const Cut& fdflavors,
    ana::RecoType recoFDIxnType,
    SpectrumLoader& extrafdloaderswap,
    SpectrumLoader& extrafdloadertau
  )
    : fRecoND( ndloader.Interactions(recoNDIxnType)[ndcut && ndflavor], axis ),
      fTrueToRecoFD( fdloader.Interactions(recoFDIxnType)[fdcut && fdflavors], axis ),
      fDecomp(&decomposition),
      fOwnDecomp(false),
      fDecompRes(dr),
      fLabel(label),
      fLatex(latex)
  {
    // TODO: Reintroduce.
    /*
    extrafdloaderswap.AddReweightableSpectrum(
      fTrueToRecoFD, axis.GetVar1D(), kTrueE, fdcut && fdflavors, shiftMC, weight);
    extrafdloadertau.AddReweightableSpectrum(
      fTrueToRecoFD, axis.GetVar1D(), kTrueE, fdcut && fdflavors, shiftMC, weight);
    */
  }

  RecoReweight::~RecoReweight()
  {
    if(fOwnDecomp) delete fDecomp;
  }

  OscillatableSpectrum RecoReweight::Eval() const
  {

    //Copy to local variable because reweighting is in-place
    OscillatableSpectrum result(fTrueToRecoFD);

    //Get ND data from Decomp
    Spectrum decompresult(GetDecompResult(*fDecomp,fDecompRes));

    //Compute Data/MC Ratio
    Ratio dataMC = FormSmartRatio(
      decompresult, fRecoND,
      fLabel, "MC ND Reco",
      fTrueToRecoFD.Unoscillated() );

    // Multiply by Data/MC Ratio and add in FD truth information
    result.ReweightToRecoSpectrum( fTrueToRecoFD.Unoscillated() * dataMC );

    return result;

  }

  void RecoReweight::SaveTo(TDirectory* dir, const std::string& name) const
  {
    TDirectory* tmp = gDirectory;

    dir = dir->mkdir(name.c_str()); // switch to subdir
    dir->cd();

    TObjString("RecoReweight").Write("type");
    fRecoND.SaveTo(dir, "RecoND");
    fTrueToRecoFD.SaveTo(dir, "TrueToRecoFD");
    fDecomp->SaveTo(dir, "Decomp");
    TObjString(DRToString(fDecompRes).c_str()).Write("DecompRes");
    TObjString(fLabel.c_str()).Write("Label");
    TObjString(fLatex.c_str()).Write("Latex");

    dir->Write();
    delete dir;

    tmp->cd();
  }

  std::unique_ptr<RecoReweight>
    RecoReweight::LoadFrom(TDirectory* dir, const std::string& name)
  {
    dir = dir->GetDirectory(name.c_str()); // switch to subdir
    assert(dir);

    assert(dir->GetDirectory("RecoND"));
    assert(dir->GetDirectory("TrueToRecoFD"));
    assert(dir->GetDirectory("Decomp"));
    TObjString* dr = (TObjString*)dir->Get("DecompRes");
    assert(dr);
    TObjString* label = (TObjString*)dir->Get("Label");
    TObjString* latex = (TObjString*)dir->Get("Latex");
    assert(label);
    assert(latex);

    RecoReweight* ret = new RecoReweight(
      *(Spectrum::LoadFrom(dir, "RecoND")),
      *(OscillatableSpectrum::LoadFrom(dir, "TrueToRecoFD")),
      *(ana::LoadFrom<IDecomp>(dir, "Decomp")).release(),
      StringToDR(dr->GetString().Data()),
      label->GetString().Data(),
      latex->GetString().Data()
    );
    // We know we have the only copy because we just loaded it
    ret->fOwnDecomp = true;
    delete dr;
    delete label;
    delete latex;
    delete dir;
    return std::unique_ptr<RecoReweight>(ret);
  }
  
}
