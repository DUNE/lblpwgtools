#include "CAFAna/Core/HistAxis.h"

#include <iostream>

namespace ana
{
  // explicitly instantiate the template for the types we know we have
  template class GenericHistAxis<Var>;
  template class GenericHistAxis<SpillVar>;
  template class GenericHistAxis<SpillTruthVar>;

  //----------------------------------------------------------------------
  template<class T> GenericHistAxis<T>::
  GenericHistAxis(const std::vector<std::string>& labels,
                  const std::vector<Binning>& bins,
                  const std::vector<T>& vars)
    : fLabels(labels), fBins(bins), fVars(vars)
  {
    for (auto &v : fVars) {
      assert(v.IsValid());
    }
    assert(fLabels.size() == fBins.size());
    assert(fBins.size() == fVars.size() || fVars.empty());
  }

  //----------------------------------------------------------------------
  template<class T>GenericHistAxis<T>::
  GenericHistAxis(const std::vector<GenericHistAxis<T>>& axes)
  {
    for(const auto& a: axes){
      fLabels.insert(fLabels.end(), a.fLabels.begin(), a.fLabels.end());
      fBins.insert(fBins.end(), a.fBins.begin(), a.fBins.end());
      fVars.insert(fVars.end(), a.fVars.begin(), a.fVars.end());
    }

    assert(fLabels.size() == fBins.size());
    assert(fBins.size() == fVars.size() || fVars.empty());
  }

  //----------------------------------------------------------------------
  template<class T> T GenericHistAxis<T>::GetVar1D() const
  {
    switch(fVars.size()){
    case 1:
      return fVars[0];
    case 2:
      return Var2D(fVars[0], fBins[0],
                   fVars[1], fBins[1]);
    case 3:
      return Var3D(fVars[0], fBins[0],
                   fVars[1], fBins[1],
                   fVars[2], fBins[2]);
    default:
      std::cout << "Error: HistAxis::GetMultiDVar() doesn't support "
                << fVars.size() << "-dimensional axes" << std::endl;
      abort();
    }
  }

  //----------------------------------------------------------------------
  template<class T> const Binning& GenericHistAxis<T>::GetBins1D() const
  {
    if(fBins.size() == 1) return fBins[0];

    if(fBins1D) return *fBins1D;

    assert(!fBins.empty());

    int n = 1;
    for(const Binning& b: fBins) n *= b.NBins();
    fBins1D = Binning::Simple(n, 0, n);

    return *fBins1D;
  }

  //----------------------------------------------------------------------
  template<class T> const std::string& GenericHistAxis<T>::GetLabel1D() const
  {
    if(fLabels.size() == 1) return fLabels[0];

    if(fLabel1D) return *fLabel1D;

    fLabel1D = "";
    for(const std::string& l: fLabels) *fLabel1D += l + " and ";
    fLabel1D->resize(fLabel1D->size()-5); // drop extra "and"

    return *fLabel1D;
  }
}
