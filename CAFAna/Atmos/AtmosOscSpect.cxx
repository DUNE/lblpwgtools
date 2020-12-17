#include "CAFAna/Atmos/AtmosOscSpect.h"

#include "CAFAna/Atmos/AtmosOscillogram.h"

namespace ana
{
  const Binning kTrueEBins = Binning::Simple(400, 0.1, 100);
  // TODO it's not really clear that 50 cos_zenith bins are enough. Or,
  // shouldn't we bin evenly around the perimeter of the earth instead?
  const Binning kTrueCosZenithBins = Binning::Simple(50, -1, +1);

  const LabelsAndBins kTrueAxis({"True Energy (GeV)", "True cos(#theta_{zenith})"},
                                {kTrueEBins, kTrueCosZenithBins});

  //----------------------------------------------------------------------
  AtmosOscSpect::AtmosOscSpect(const LabelsAndBins& recoAxis)
    : ReweightableSpectrum(recoAxis, kTrueAxis)
  {
    fMat.resize(kTrueAxis.GetBins1D().NBins()+2, recoAxis.GetBins1D().NBins()+2);
    fMat.setZero();
  }

  // TODO copy constructors et al handling fCache

  //----------------------------------------------------------------------
  Spectrum AtmosOscSpect::Oscillated(osc::IOscCalc* calc, int from, int to) const
  {
    TMD5* hash = calc->GetParamsHash();
    if(hash && fCache->hash && *hash == *fCache->hash){
      delete hash;
      return fCache->spect;
    }

    const AtmosOscillogram curve(calc, from, to);
    const Spectrum ret = WeightedBy(curve);
    if(hash){
      fCache->spect = ret;
      fCache->hash.reset(hash);
    }

    return ret;
  }
}
