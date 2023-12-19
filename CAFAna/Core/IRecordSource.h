#pragma once

#include "cafanacore/IRecordSource.h"

#include "CAFAna/Core/IRecordSink.h"

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{

  class ISyst;
  template<class SystT> class _Multiverse;
  using Multiverse = _Multiverse<ISyst>;

  class SystShifts;

  //----------------------------------------------------------------------
  // Introduce some aliases so we can express ourselves more succinctly

  using ISRSource = _IRecordSource<caf::SRProxy>;
  using IInteractionSource = _IRecordSource<caf::SRInteractionProxy>;

  using IParticleSource = _IRecordSource<caf::SRRecoParticleProxy>;


/*// no ensemble stuff yet
*  using ISpillEnsembleSource = _IRecordEnsembleSource<caf::SRSpillProxy>;
*  using ISliceEnsembleSource = _IRecordEnsembleSource<caf::SRSliceProxy>;
*  using INuTruthEnsembleSource = _IRecordEnsembleSource<caf::SRTrueInteractionProxy>;
*
*  //----------------------------------------------------------------------
*
*  using ITrackEnsembleSource = _IRecordEnsembleSource<caf::SRTrackProxy>;
*  using IShowerEnsembleSource = _IRecordEnsembleSource<caf::SRShowerProxy>;
*  using IStubEnsembleSource = _IRecordEnsembleSource<caf::SRStubProxy>;
*/
  //----------------------------------------------------------------------

  /// Helper class for implementing looping over slices, tracks, etc
  template<class FromT, class ToT> class VectorAdaptor:
    public PassthroughExposure<_IRecordSink<caf::Proxy<FromT>>,
                               _IRecordSource<caf::Proxy<ToT>>>
  {
  public:
    typedef std::function<const caf::Proxy<std::vector<ToT>>&(const caf::Proxy<FromT>*)> Func_t;

    VectorAdaptor(_IRecordSource<caf::Proxy<FromT>>& src, Func_t vecGetter);
    virtual void HandleRecord(const caf::Proxy<FromT>* rec, double weight) override;
  protected:
    Func_t fVecGetter;
  };

  //----------------------------------------------------------------------

  // Accessors needed by VectorAdaptor

  const caf::Proxy<std::vector<caf::SRInteraction>>& GetInteractions(const caf::SRProxy* sr);

  const caf::Proxy<std::vector<caf::SRRecoParticle>>& GetRecoParticles(const caf::SRInteractionProxy* ixn);
  
//  const caf::Proxy<std::vector<caf::SRTrueInteraction>>& GetNuTruths(const caf::SRSpillProxy* spill);
//  const caf::Proxy<std::vector<caf::SRTrack>>& GetTracks(const caf::SRSliceProxy* reco);
//  const caf::Proxy<std::vector<caf::SRShower>>& GetShowers(const caf::SRSliceProxy* reco);
//  const caf::Proxy<std::vector<caf::SRStub>>& GetStubs(const caf::SRSliceProxy* reco);

  //----------------------------------------------------------------------

  template<> class _IRecordSource<caf::SRInteractionProxy>
    : public _IRecordSourceDefaultImpl<caf::SRInteractionProxy>
  {
  public:
    // Weight-based ensembles are still supported
    //using _IRecordSourceDefaultImpl::Ensemble;

    // But also support an ensemble based on SystShifts
    //ISliceEnsembleSource& Ensemble(const Multiverse& multiverse);

    IParticleSource& RecoParticles() {return fParticle;}

  protected:    
    //IDDict<const FitMultiverse*, ISliceEnsembleSource> fEnsembleSources;
    VectorAdaptor<caf::SRInteraction, caf::SRRecoParticle> fParticle{*this, GetRecoParticles};

  };

  //----------------------------------------------------------------------

  // Spill sources also provide a slice source (which loops over the slices)
  template<> class _IRecordSource<caf::SRProxy>
    : public _IRecordSourceDefaultImpl<caf::SRProxy>
  {
  public:
    IInteractionSource& Interactions() {return fInteractions;}
    //INuTruthSource& NuTruths() {return fNuTruths;}

  protected:
    VectorAdaptor<caf::StandardRecord, caf::SRInteraction> fInteractions{*this, GetInteractions};
  };
  //----------------------------------------------------------------------

/* /// Skipping over ensemble implementations
*  /// Helper class for implementing looping over slices, tracks, etc
*  template<class FromT, class ToT> class EnsembleVectorAdaptor:
*    public PassthroughExposure<_IRecordEnsembleSink<caf::Proxy<FromT>>,
*                               _IRecordEnsembleSource<caf::Proxy<ToT>>>
*  {
*  public:
*    using Source_t = _IRecordEnsembleSource<caf::Proxy<FromT>>;
*    using Func_t = std::function<const caf::Proxy<std::vector<ToT>>&(const caf::Proxy<FromT>*)>;
*
*    EnsembleVectorAdaptor(Source_t& src, Func_t vecGetter);
*
*    virtual void HandleSingleRecord(const caf::Proxy<FromT>* rec,
*                                    double weight,
*                                    int universeIdx) override;
*
*    virtual void HandleEnsemble(const caf::Proxy<FromT>* rec,
*                                const std::vector<double>& weight) override;
*
*    virtual const ana::FitMultiverse* GetMultiverse() const {return fSource->GetMultiverse();}
*
*  protected:
*    const Source_t* fSource;
*    Func_t fVecGetter;
*  };
*
*  //----------------------------------------------------------------------
*
*  // Provide ability to get track / shower / stub sources from the reco branch
*  // ensemble source.
*
*  template<> class _IRecordEnsembleSource<caf::SRSliceProxy>
*    : public _IRecordEnsembleSourceDefaultImpl<caf::SRSliceProxy>
*  {
*  public:
*    ITrackEnsembleSource& Tracks() {return fTracks;}
*    IShowerEnsembleSource& Showers() {return fShowers;}
*    IStubEnsembleSource& Stubs() {return fStubs;}
*
*  protected:
*    EnsembleVectorAdaptor<caf::SRSlice, caf::SRTrack> fTracks{*this, GetTracks};
*    EnsembleVectorAdaptor<caf::SRSlice, caf::SRShower> fShowers{*this, GetShowers};
*    EnsembleVectorAdaptor<caf::SRSlice, caf::SRStub> fStubs{*this, GetStubs};
*  };
*
*  //----------------------------------------------------------------------
*  // Spill sources also provide a slice source (which loops over the slices)
*  template<> class _IRecordEnsembleSource<caf::SRSpillProxy>
*    : public _IRecordEnsembleSourceDefaultImpl<caf::SRSpillProxy>
*  {
*  public:
*    ISliceEnsembleSource& Slices() {return fSlices;}
*    INuTruthEnsembleSource& NuTruths() {return fNuTruths;}
*
*  protected:
*    EnsembleVectorAdaptor<caf::StandardRecord, caf::SRSlice> fSlices{*this, GetSlices};
*    EnsembleVectorAdaptor<caf::StandardRecord, caf::SRTrueInteraction> fNuTruths{*this, GetNuTruths};
*  };
*/
}
