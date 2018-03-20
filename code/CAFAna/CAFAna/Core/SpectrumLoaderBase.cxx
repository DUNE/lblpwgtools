#include "CAFAna/Core/SpectrumLoaderBase.h"

#include "CAFAna/Core/Progress.h"
#include "CAFAna/Core/ReweightableSpectrum.h"
#include "CAFAna/Core/SAMQuerySource.h"
#include "CAFAna/Core/SAMProjectSource.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/Utilities.h"
#include "CAFAna/Core/WildcardSource.h"

#include "StandardRecord/StandardRecord.h"

#include "ifdh.h"

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include "boost/algorithm/string.hpp"

namespace ana
{
  // Apparently the existence of fSpillDefs isn't enough and I need to spell
  // this out to make sure the function bodies are generated.
  template class SpectrumLoaderBase::IDMap<SystShifts, SpectrumLoaderBase::IDMap<Cut, SpectrumLoaderBase::IDMap<Var, SpectrumLoaderBase::IDMap<SpectrumLoaderBase::VarOrMultiVar, SpectrumLoaderBase::SpectList>>>>;

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::SpectList::Erase(Spectrum* s)
  {
    auto it = std::find(spects.begin(), spects.end(), s);
    if(it != spects.end()) spects.erase(it);
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::SpectList::Erase(ReweightableSpectrum* rs)
  {
    auto it = std::find(rwSpects.begin(), rwSpects.end(), rs);
    if(it != rwSpects.end()) rwSpects.erase(it);
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::SpectList::RemoveLoader(SpectrumLoaderBase* l)
  {
    for(Spectrum* s: spects) s->RemoveLoader(l);
    for(ReweightableSpectrum* rs: rwSpects) rs->RemoveLoader(l);
  }

  //----------------------------------------------------------------------
  size_t SpectrumLoaderBase::SpectList::TotalSize() const
  {
    return spects.size() + rwSpects.size();
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::SpectList::GetSpectra(std::vector<Spectrum*>& ss)
  {
    ss.insert(ss.end(), spects.begin(), spects.end());
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::SpectList::
  GetReweightableSpectra(std::vector<ReweightableSpectrum*>& ss)
  {
    ss.insert(ss.end(), rwSpects.begin(), rwSpects.end());
  }

  //----------------------------------------------------------------------
  template<class T, class U> U& SpectrumLoaderBase::IDMap<T, U>::
  operator[](const T& key)
  {
    for(auto& it: fElems){
      if(it.first.ID() == key.ID()) return it.second;
    }
    fElems.push_back(std::make_pair(key, U()));
    return fElems.back().second;
  }

  //----------------------------------------------------------------------
  template<class T, class U> template<class V> void SpectrumLoaderBase::IDMap<T, U>::Erase(const V& v)
  {
    for(auto& it: fElems) it.second.Erase(v);
  }

  //----------------------------------------------------------------------
  template<class T, class U> void SpectrumLoaderBase::IDMap<T, U>::
  RemoveLoader(SpectrumLoaderBase* l)
  {
    for(auto& it: fElems) it.second.RemoveLoader(l);
  }

  //----------------------------------------------------------------------
  template<class T, class U> void SpectrumLoaderBase::IDMap<T, U>::Clear()
  {
    fElems.clear();
  }

  //----------------------------------------------------------------------
  template<class T, class U> size_t SpectrumLoaderBase::IDMap<T, U>::
  TotalSize()
  {
    size_t ret = 0;
    for(auto& it: fElems) ret += it.second.TotalSize();
    return ret;
  }

  //----------------------------------------------------------------------
  template<class T, class U> void SpectrumLoaderBase::IDMap<T, U>::
  GetSpectra(std::vector<Spectrum*>& ss)
  {
    for(auto& it: fElems) it.second.GetSpectra(ss);
  }

  //----------------------------------------------------------------------
   template<class T, class U> void SpectrumLoaderBase::IDMap<T, U>::
  GetReweightableSpectra(std::vector<ReweightableSpectrum*>& ss)
  {
    for(auto& it: fElems) it.second.GetReweightableSpectra(ss);
  }

  // Start of SpectrumLoaderBase proper

  //----------------------------------------------------------------------
  SpectrumLoaderBase::SpectrumLoaderBase(DataSource src)
    : fSource(src), fGone(false),
      fPOT(0), fRunPOT(0)
  {
  }

  //----------------------------------------------------------------------
  SpectrumLoaderBase::SpectrumLoaderBase(const std::string& wildcard,
                                         DataSource src)
    : SpectrumLoaderBase(src)
  {
    fWildcard = wildcard;
    fFileSource = std::unique_ptr<IFileSource>(WildcardOrSAMQuery(wildcard));
  }

  //----------------------------------------------------------------------
  SpectrumLoaderBase::SpectrumLoaderBase(const std::vector<std::string>& fnames,
                                         DataSource src)
    : SpectrumLoaderBase(src)
  {
    fWildcard = "file list";
    fFileSource = std::unique_ptr<IFileSource>(new FileListSource(fnames));

    assert(!fnames.empty());
    std::cout << "Loading from " << fnames.size() << " files" << std::endl;
  }

  //----------------------------------------------------------------------
  SpectrumLoaderBase::~SpectrumLoaderBase()
  {
    fHistDefs.RemoveLoader(this);
  }

  //----------------------------------------------------------------------
  IFileSource* SpectrumLoaderBase::
  WildcardOrSAMQuery(const std::string& str) const
  {
    int stride = -1;
    int offset = -1;
    if(getenv("CAFANA_STRIDE")){
      stride = atoi(getenv("CAFANA_STRIDE"));
      if(stride > 1 && getenv("CAFANA_OFFSET")){
        offset = atoi(getenv("CAFANA_OFFSET"));
      }
    }

    // stat() blows up on strings with spaces
    if(str.find(' ') == std::string::npos){
      WildcardSource* ret = new WildcardSource(str, stride, offset);
      if(ret->NFiles() > 0) return ret;
      delete ret;
    }

    // Maybe this the name of a SAM project?
    ifdh_ns::ifdh i;
    const std::string info = i.dumpProject(i.findProject(str, "nova"));
    // findProject always gives back an address just by gluing bits
    // together. But dumpProject will give an empty result for a nonexistent
    // project.
    if(!info.empty()){
      if(stride > 1)
        std::cout << "Warning: --stride has no effect on SAM projects"
                  << std::endl;

      return new SAMProjectSource(str);
    }

    // Maybe this is a SAM dataset or query?
    return new SAMQuerySource(str, stride, offset);
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::AddSpectrum(Spectrum& spect,
                                       const Var& var,
                                       const Cut& cut,
                                       const SystShifts& shift,
                                       const Var& wei)
  {
    if(fGone){
      std::cerr << "Error: can't add Spectra after the call to Go()" << std::endl;
      abort();
    }

    fHistDefs[shift][cut][wei][var].spects.push_back(&spect);

    spect.AddLoader(this); // Remember we have a Go() pending
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::AddSpectrum(Spectrum& spect,
                                       const MultiVar& var,
                                       const Cut& cut,
                                       const SystShifts& shift,
                                       const Var& wei)
  {
    if(fGone){
      std::cerr << "Error: can't add Spectra after the call to Go()" << std::endl;
      abort();
    }

    fHistDefs[shift][cut][wei][var].spects.push_back(&spect);

    spect.AddLoader(this); // Remember we have a Go() pending
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::RemoveSpectrum(Spectrum* spect)
  {
    fHistDefs.Erase(spect);
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::AddReweightableSpectrum(ReweightableSpectrum& spect,
                                                   const Var& var,
                                                   const Cut& cut,
                                                   const SystShifts& shift,
                                                   const Var& wei)
  {
    if(fGone){
      std::cerr << "Error: can't add Spectra after the call to Go()" << std::endl;
      abort();
    }

    fHistDefs[shift][cut][wei][var].rwSpects.push_back(&spect);

    spect.AddLoader(this); // Remember we have a Go() pending
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::
  RemoveReweightableSpectrum(ReweightableSpectrum* spect)
  {
    fHistDefs.Erase(spect);
  }

  //----------------------------------------------------------------------
  void SpectrumLoaderBase::
  RegisterCompletionCallback(std::function<CallbackFunc_t> func)
  {
    fCallbacks.push_back(func);
  }

  //----------------------------------------------------------------------
  int SpectrumLoaderBase::NFiles() const
  {
    return fFileSource->NFiles();
  }

  //----------------------------------------------------------------------
  TFile* SpectrumLoaderBase::GetNextFile()
  {
    TFile* f = fFileSource->GetNextFile();
    if(!f) return 0; // out of files

    //    TTree* trPot = (TTree*)f->Get("mvaselect/pottree");
    TTree* trPot;
    if (f->GetListOfKeys()->Contains("cafmaker")) trPot = (TTree*)f->Get("mvaselectnumu/pottree");
    //    else  trPot = (TTree*)f->Get("mvaselectnumu/pottree");
    else  trPot = (TTree*)f->Get("mvaselect/pottree");
    assert(trPot);

    double pot;
    int run;
    trPot->SetBranchAddress("pot", &pot);
    trPot->SetBranchAddress("run", &run);

    for(int n = 0; n < trPot->GetEntries(); ++n){
      trPot->GetEntry(n);

      fPOTMap[run] += pot;
      fPOTMap[0] += pot; // Run-agnostic total
    }

    return f;
  }

  //----------------------------------------------------------------------
  template<class T> SpectrumLoaderBase::BranchList<T>::
  BranchList(TTree* tr, const std::string& recName,
             const std::set<std::string>& branchNames)
    : fTree(tr), fRecName(recName)
  {
    // Constructing the StandardRecord can involve NaNs. That's not a
    // problem. The idea is that they trip when someone tries to /use/ them.
    FloatingExceptionOnNaN fpnan(false);

    fRec = 0;
    tr->SetBranchAddress(recName.c_str(), &fRec);

    // If we haven't been requested to do anything that actually signals to
    // load the entire StandardRecord object.
    if(branchNames.empty()) return;

    tr->SetBranchStatus("*", 0);

    fBranches.reserve(branchNames.size());
    for(std::string brName: branchNames){
      // Components of branches can be marked with '~' to indicate
      // that they may not exist (for backwards-compatibility).
      // If one of these doesn't, just make sure the parent branch is enabled
      // (so that the default constructor takes care of filling
      //  the desired branch with default values).
      std::string workStr(brName);
      boost::algorithm::erase_all(workStr, "~");
      std::vector<std::string> brToTry { workStr };
      std::string::size_type tildePos = brName.size()-1;
      // work backwards through the occurrences of '~'...
      while ((tildePos = brName.rfind("~", tildePos-1)) != std::string::npos)
      {
        workStr = brName.substr(0, tildePos-1);
        boost::algorithm::erase_all(workStr, "~");
        brToTry.push_back(workStr);
        if (workStr[workStr.size()-1] == '.')
          brToTry.back().resize(workStr.size()-1);
      }

      // we're going to be trying various possibly nonexistent
      // branch names.  we don't need any error message for every one.
      decltype(kWarning) errLevel = gErrorIgnoreLevel;
      gErrorIgnoreLevel = kFatal;

      TBranch * branch = nullptr;
      for (auto br : brToTry)
      {
        tr->SetBranchStatus(br.c_str(), 1);

        // If branch name ends with ".*" then it's right to set the branch status
        // with the star included, but GetBranch needs it removed. Because C++'s
        // string handling sucks this is pretty clumsy.
        if(br.size() > 2 &&
            br[brName.size()-2] == '.' &&
            br[br.size()-1] == '*')
          br.resize(br.size()-2);

        // don't keep looking if we've found a match
        if ( (branch = tr->GetBranch(br.c_str())) )
          break;
      }

      if(!branch){
        std::cerr << "Tree has no branch \"" << brName
                  << "\". Aborting" << std::endl;
        abort();
      }

      gErrorIgnoreLevel = errLevel;

      fBranches.push_back(branch);
      tr->AddBranchToCache(branch);
    }
  }

  //----------------------------------------------------------------------
  template<class T> SpectrumLoaderBase::BranchList<T>::~BranchList()
  {
    // Disclaim any interest in this tree, so that subsequent users don't end
    // up also trying to fill our now-freed fRec
    fTree->SetBranchAddress(fRecName.c_str(), 0);
  }

  //----------------------------------------------------------------------
  template<class T> T* SpectrumLoaderBase::BranchList<T>::GetEntry(int n)
  {
    // Reading the StandardRecord can involve NaNs. That's not a problem. The
    // idea is that they trip when someone tries to /use/ them.
    FloatingExceptionOnNaN fpnan(false);

    assert(n >= 0);
    const unsigned int NBr = fBranches.size();

    if(NBr == 0){
      // If we haven't been requested to do anything that actually signals to
      // load the entire StandardRecord object.
      fTree->GetEntry(n);
    }
    else{
      for(unsigned int i = 0; i < NBr; ++i){
        fBranches[i]->GetEntry(n);
      }
    }
    return fRec;
  }

  // Instantiate the two types people need
  template class SpectrumLoaderBase::BranchList<caf::StandardRecord>;

  //----------------------------------------------------------------------
  void NullLoader::Go()
  {
    // Polite to signal everyone in case they're relying on it
    for(auto c: fCallbacks) c();
  }

  //----------------------------------------------------------------------
  NullLoader::~NullLoader()
  {
  }

} // namespace

