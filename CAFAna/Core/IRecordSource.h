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

//Things I was trying so we could write loader.Interactions()[IntCut].{dlp,pandora}.RecoParticles().{dlp,pandora}[PartiCut]
//  using ISRSource = _IRecordSource<caf::SRProxy>;
//  using IInteractionSource = _IRecordSource<caf::SRInteractionBranchProxy>;
//  using IParticleSource = _IRecordSource<caf::SRRecoParticlesBranchProxy>;


  //----------------------------------------------------------------------
	
  enum class RecoType {
	kDLP,
	kPandora,
	kPIDA
	};

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

  const caf::Proxy<std::vector<caf::SRInteraction>>& GetInteractions(const RecoType kRType,const caf::SRProxy* sr);

  const caf::Proxy<std::vector<caf::SRRecoParticle>>& GetRecoParticles(const RecoType kRType, const caf::SRInteractionProxy* ixn);

//Things I was trying so we could write loader.Interactions()[IntCut].{dlp,pandora}.RecoParticles().{dlp,pandora}[PartiCut]
//    const caf::Proxy<caf::SRInteractionBranch>& GetInteractions(const caf::SRProxy* sr);
//    const caf::Proxy<caf::SRRecoParticlesBranch>& GetRecoParticles(const caf::SRInteractionProxy* ixn);

  //----------------------------------------------------------------------

  template<> class _IRecordSource<caf::SRInteractionProxy>
    : public _IRecordSourceDefaultImpl<caf::SRInteractionProxy>
  {
  public:
    // Weight-based ensembles are still supported
    //using _IRecordSourceDefaultImpl::Ensemble;

    // But also support an ensemble based on SystShifts
    //ISliceEnsembleSource& Ensemble(const Multiverse& multiverse);

    IParticleSource& RecoParticles( const RecoType kRType) {return fParticle;}

  protected:    
    //IDDict<const FitMultiverse*, ISliceEnsembleSource> fEnsembleSources;
    VectorAdaptor<caf::SRInteraction, caf::SRRecoParticle> fParticle{*this, GetRecoParticles(kRType)};

  };

  //----------------------------------------------------------------------

  // Standard Record sources also provide a  source (which loops over the interactions)
  template<> class _IRecordSource<caf::SRProxy>
    : public _IRecordSourceDefaultImpl<caf::SRProxy>
  {
  public:
    IInteractionSource& Interactions( const RecoType kRType ) {return fInteractions;}
    //INuTruthSource& NuTruths() {return fNuTruths;}

  protected:
    VectorAdaptor<caf::StandardRecord, caf::SRInteraction> fInteractions{*this, GetInteractions(kRType)};
  };
  //----------------------------------------------------------------------

//Things I was trying so we could write loader.Interactions()[IntCut].{dlp,pandora}.RecoParticles().{dlp,pandora}[PartiCut]
//  template<> class _IRecordSource<caf::SRInteractionProxy>
//    : public _IRecordSourceDefaultImpl<caf::SRInteractionProxy>
//  {
//  public:
//    // Weight-based ensembles are still supported
//    //using _IRecordSourceDefaultImpl::Ensemble;
//
//    // But also support an ensemble based on SystShifts
//    //ISliceEnsembleSource& Ensemble(const Multiverse& multiverse);
//
//    IParticleSource& RecoParticles() {return fParticle;}
//
//  protected:    
//    //IDDict<const FitMultiverse*, ISliceEnsembleSource> fEnsembleSources;
//    VectorAdaptor<caf::SRInteraction, caf::SRRecoParticlesBranch> fParticle{*this, GetRecoParticles};
//
//  };
//  //----------------------------------------------------------------------
//
//  
//  template<> class _IRecordSource<caf::SRProxy>
//    : public _IRecordSourceDefaultImpl<caf::SRProxy>
//  {
//  public:
//    IInteractionSource& Interactions() {return fInteractions;}
//    //INuTruthSource& NuTruths() {return fNuTruths;}
//
//  protected:
//    VectorAdaptor<caf::StandardRecord, caf::SRInteractionBranch> fInteractions{*this, GetInteractions};
//  };
//  //---

}
