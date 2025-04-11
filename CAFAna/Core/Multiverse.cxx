#include "CAFAna/Core/Multiverse.h"

#include "CAFAna/Core/Registry.h"

#include "TDirectory.h"
#include "TMD5.h"
#include "TObjString.h"

#include <iostream>
#include <random>

namespace ana
{
  // Multiverses live the lifetime of the process, but let's not leak them at
  // shutdown
  std::vector<std::unique_ptr<FitMultiverse>> gFitMultiverses;

  namespace{
    // This is "group separator" and appears to print as a space. It is
    // unlikely to occur in a systematic name by accident, unlike more usual
    // separators.
    const char kSepChar(29);
    const std::string kSep(1, kSepChar);
  }

  //----------------------------------------------------------------------
  bool CompareSystsByShortName(const IFitSyst* a, const IFitSyst* b)
  {
    return a->ShortName() < b->ShortName();
  }

  //----------------------------------------------------------------------
  std::vector<const IFitSyst*> SortedSystList(const std::vector<const IFitSyst*> systs)
  {
    std::vector<const IFitSyst*> ret = systs;
    std::sort(ret.begin(), ret.end(), CompareSystsByShortName);
    return ret;
  }

  //----------------------------------------------------------------------
  std::vector<std::string> Tokenize(const std::string& str)
  {
    std::vector<std::string> ret;

    size_t i0 = 0;
    while(true){
      const size_t i1 = str.find(kSepChar, i0);
      ret.push_back(str.substr(i0, i1-i0));
      if(i1 == std::string::npos) return ret;
      i0 = i1+1;
    }
  }

  //----------------------------------------------------------------------
  std::string GetLabelValue(const std::string& str, const std::string& label)
  {
    if(str.substr(0, label.size()+1) != label+"="){
      std::cout << "FitMultiverse::LoadFrom(): expected '"
                << label << "=value' "
                << "got '" << str << "'" << std::endl;
      abort();
    }
    return str.substr(label.size()+1);
  }

  //----------------------------------------------------------------------
  std::string ConsumeLabelValue(std::vector<std::string>& tokens,
                                const std::string& label)
  {
    const std::string ret = GetLabelValue(tokens.front(), label);
    tokens.erase(tokens.begin()); // effectively pop_front()
    return ret;
  }

  //----------------------------------------------------------------------
  unsigned int GetLabelIntValue(const std::string& str, const std::string& label)
  {
    const std::string valuestr = GetLabelValue(str, label);

    unsigned int ret;
    size_t pos = -1;
    try{
      ret = std::stoi(valuestr, &pos);
    }
    catch(const std::exception& ex){
      pos = -1;
      ret = -1; // silence uninitialized variable warning
    }

    if(pos != valuestr.size()){ // need to consume all the string
      std::cout << "FitMultiverse::LoadFrom(): failed to parse integer parameter '" << str << "'" << std::endl;
      abort();
    }

    return ret;
  }

  //----------------------------------------------------------------------
  unsigned int ConsumeLabelIntValue(std::vector<std::string>& tokens,
                                    const std::string& label)
  {
    const unsigned int ret = GetLabelIntValue(tokens.front(), label);
    tokens.erase(tokens.begin()); // effectively pop_front()
    return ret;
  }

  //----------------------------------------------------------------------
  std::vector<const IFitSyst*> SystNamesToSysts(const std::vector<std::string>& names)
  {
    std::vector<const IFitSyst*> ret;
    ret.reserve(names.size());

    for(const std::string& name: names){
      ret.push_back(Registry<ISyst>::ShortNameToPtr(name, true));
      //ret.push_back(Registry<ISyst<caf::SRInteractionProxy>>::ShortNameToPtr(name, true));
      //ret.push_back(Registry<ISyst<caf::SRTrueInteractionProxy>>::ShortNameToPtr(name, true));
      if(!ret.back()){
        std::cout << "FitMultiverse::LoadFrom(): couldn't find any syst with short name '" << name << "'" << std::endl;
        abort();
      }
    }

    return ret;
  }

  //----------------------------------------------------------------------
  FitMultiverse::FitMultiverse(const std::string& shortName,
                               const std::string& latexName,
                               const std::vector<FitUniverse>& univs,
                               const EMultiverseType multiverseType)
    : INamed(shortName, latexName), fUnivs(univs), fMultiverseType(multiverseType)
  {
    Registry<FitMultiverse>::Register(this);
  }

  //----------------------------------------------------------------------
  FitMultiverse::FitMultiverse(const FitMultiverse&& m)
    : INamed(m), fUnivs(m.fUnivs), fMultiverseType(m.fMultiverseType)
  {
    // "leak" the entry in the global list so we don't double-delete it
    for(auto& it: gFitMultiverses) if(it.get() == &m) it.release();

    Registry<FitMultiverse>::UnRegister(&m);
    Registry<FitMultiverse>::Register(this);
  }

  //----------------------------------------------------------------------
  const FitMultiverse& FitMultiverse::
  Hypercross(const std::vector<const IFitSyst*>& systs_unsorted, int nsigma)
  {
    // Ensure reproducibility
    const std::vector<const IFitSyst*> systs = SortedSystList(systs_unsorted);

    std::string name = "cross"+kSep+"nsigma="+std::to_string(nsigma);
    for(const IFitSyst* s: systs) name += kSep + s->ShortName();

    // Make sure that requesting the same multiverse multiple times always
    // gives back the same object
    const FitMultiverse* reg = Registry<FitMultiverse>::ShortNameToPtr(name, true);
    if(reg) return *reg;

    const std::string latexName = "Hypercross for #pm"+std::to_string(nsigma)+"#sigma over "+std::to_string(systs.size())+" systematics";


    std::vector<FitUniverse> univs;
    univs.reserve(2*nsigma*systs.size()+1);
    univs.emplace_back(); // nominal
    for(const IFitSyst* s: systs){
      for(int i = -nsigma; i <= +nsigma; ++i){
        if(i == 0) continue;
        FitUniverse univ;
        univ.emplace(s, i);
        univs.push_back(univ);
      }
    } // end for systs

    gFitMultiverses.emplace_back(new FitMultiverse(name, latexName, univs, kHypercross));
    return *gFitMultiverses.back();
  }

  //----------------------------------------------------------------------
  const FitMultiverse& FitMultiverse::
  RandomGas(const std::vector<const IFitSyst*>& systs_unsorted, int Nuniv,
            unsigned int seed)
  {
    // Ensure reproducibility
    const std::vector<const IFitSyst*> systs = SortedSystList(systs_unsorted);

    if(seed == kTrulyRandom) seed = std::random_device{}();

    std::string name = "gas"+kSep+"nuniv="+std::to_string(Nuniv)+kSep+"seed="+std::to_string(seed);
    for(const IFitSyst* s: systs) name += kSep+s->ShortName();

    // Make sure that requesting the same multiverse multiple times always
    // gives back the same object
    const FitMultiverse* reg = Registry<FitMultiverse>::ShortNameToPtr(name, true);
    if(reg) return *reg;

    const std::string latexName = "Random multiverse with "+std::to_string(Nuniv)+" universes over "+std::to_string(systs.size())+" systematics";

    // Use stdlib rather than rely on ROOT's dodgy random numbers
    std::mt19937_64 generator(seed); // 64-bit Mersenne twister
    std::normal_distribution<double> gaus(0, 1);

    std::vector<FitUniverse> univs;
    univs.reserve(Nuniv+1);
    univs.emplace_back(); // nominal
    for(int i = 0; i < Nuniv; ++i){
      FitUniverse univ;
      for(const IFitSyst* s: systs){
        univ.emplace(s, gaus(generator));
      }
      univs.push_back(univ);
    }

    gFitMultiverses.emplace_back(new FitMultiverse(name, latexName, univs, kRandomGas));
    return *gFitMultiverses.back();
  }

  // TO DO: a member that creates a new multiverse from a vector of multiverses 
  // care has to be put in making sure they are compatible
  //----------------------------------------------------------------------  
  //  const FitMultiverse& FitMultiverse::MergeMultiverse( std::vector<Multiverse> multiverses ){ 
  //      
  //    GetUniverse 
  //  
  //  }

  //----------------------------------------------------------------------
  void FitMultiverse::SaveTo(TDirectory* dir, const std::string& name) const
  {
    // We could make a whole directory and fill it with a detailed record of
    // this object. But this will be saved for every single EnsembleSpectrum,
    // so try and make it as minimal as possible. We should be able to encode
    // enough information to reconstruct ourselves in a simple string.

    TDirectory* tmp = gDirectory;

    dir->cd();
    TObjString((ShortName()+kSep+"md5="+Checksum()).c_str()).Write(name.c_str());

    tmp->cd();
  }

  //----------------------------------------------------------------------
  const FitMultiverse* FitMultiverse::LoadFrom(TDirectory* dir,
                                               const std::string& name)
  {
    TObjString* obj = (TObjString*)dir->Get(name.c_str());
    assert(obj);
    const std::string str = obj->GetString().Data();

    const std::string shortname = str.substr(0, str.rfind(kSep));

    std::vector<std::string> tokens = Tokenize(str);
    assert(tokens.size() >= 2); // type and checksum at least

    const std::string md5 = GetLabelValue(tokens.back(), "md5");
    tokens.pop_back();

    // If this multiverse is already known, just give it back directly
    const FitMultiverse* ret = Registry<FitMultiverse>::ShortNameToPtr(shortname, true);

    if(!ret){
      // Otherwise we need to construct one
      const std::string type = tokens.front();
      tokens.erase(tokens.begin()); // effectively pop_front()

      if(type == "gas"){
        const int nuniv = ConsumeLabelIntValue(tokens, "nuniv");
        const unsigned int seed = ConsumeLabelIntValue(tokens, "seed");

        ret = &FitMultiverse::RandomGas(SystNamesToSysts(tokens), nuniv, seed);
      }
      else if(type == "cross"){
        const int nsigma = ConsumeLabelIntValue(tokens, "nsigma");

        ret = &FitMultiverse::Hypercross(SystNamesToSysts(tokens), nsigma);
      }
      else{
        std::cout << "Multiverse::LoadFrom(): unknown multiverse type '"
                  << type << "'" << std::endl;
        abort();
      }
    }

    if(ret->Checksum() != md5){
      std::cout << "Multiverse::LoadFrom(): checksum mismatch while loading '" << name << "' with content '" << str << "'" << std::endl;
      abort();
    }

    return ret;
  }

  //----------------------------------------------------------------------
  std::string FitMultiverse::Checksum() const
  {
    TMD5 md5;

    for(const FitUniverse& univ: fUnivs){
      std::vector<const IFitSyst*> keys;
      for(auto it: univ) keys.push_back(it.first);
      // Ensure reproducibility because the map doesn't guarantee any
      // particular ordering
      for(const IFitSyst* syst: SortedSystList(keys)){
        md5.Update((unsigned char*)syst->ShortName().c_str(),
                   syst->ShortName().size());
        const double val = univ.at(syst);
        md5.Update((unsigned char*)&val, sizeof(double));
      } // end fo syst
    } // end for univ

    md5.Final();
    return md5.AsString();
  }
}