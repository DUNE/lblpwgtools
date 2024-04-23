#pragma once

#include "cafanacore/IRecordSource.h"

#include "CAFAna/Core/IRecordSink.h"

#include "duneanaobj/StandardRecord/Proxy/FwdDeclare.h"

namespace ana
{
 // template<class RecT>  class ISyst;
  class ISyst;
  
  template<class SystT> class _Multiverse;
 // using Multiverse = _Multiverse<ISyst<caf::SRInteractionProxy>>;
  //using MultiverseTrue = _Multiverse<ISyst<caf::SRTrueInteractionProxy>>;
  using Multiverse = _Multiverse<ISyst>;

  class SystShifts;

  //----------------------------------------------------------------------
  // Introduce some aliases so we can express ourselves more succinctly
  using ISRSource = _IRecordSource<caf::SRProxy>;
  using IInteractionSource = _IRecordSource<caf::SRInteractionProxy>;
  using INDLarInteractionSource = _IRecordSource<caf::SRNDLArIntProxy>;
  using IParticleSource = _IRecordSource<caf::SRRecoParticleProxy>;
  using INuTruthSource = _IRecordSource<caf::SRTrueInteractionProxy>;
  using INuTruthParticleSource = _IRecordSource<caf::SRTrueParticleProxy>;
  //----------------------------------------------------------------------
  using IInteractionEnsembleSource = _IRecordEnsembleSource<caf::SRInteractionProxy>;
  using ITrueInteractionEnsembleSource = _IRecordEnsembleSource<caf::SRTrueInteractionProxy>;
  using IParticleEnsembleSource = _IRecordEnsembleSource<caf::SRRecoParticleProxy>;
  using INuTruthParticleEnsembleSource = _IRecordEnsembleSource<caf::SRTrueParticleProxy>;
  //----------------------------------------------------------------------
	
  enum class RecoType {
                      	kDLP,
                      	kPandora,
                      	kPIDA
	};
  enum class TruePType{
                        kPrim,
                        kSec,
                        kPreFSI
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

  template <RecoType PartType>
  const caf::Proxy<std::vector<caf::SRRecoParticle>>& GetRecoParticles(const caf::SRInteractionProxy* ixn);

//  template <RecoType PartType>
//  const caf::Proxy<std::vector<caf::SRTrack>>& GetTracks(const caf::SRInteractionProxy* ixn);
//
//  template <RecoType PartType>
//  const caf::Proxy<std::vector<caf::SRTrack>>& GetShowers(const caf::SRInteractionProxy* ixn);// or caf::SRProxy* sr? 

  template <RecoType IntType>
  const caf::Proxy<std::vector<caf::SRInteraction>>& GetInteractions(const caf::SRProxy* sr);

  template <RecoType IntType>
  const caf::Proxy<std::vector<caf::SRNDLArInt>>& GetNDLarInteractions(const caf::SRProxy* sr);

  const caf::Proxy<std::vector<caf::SRTrueInteraction>>& GetNuTruths(const caf::SRProxy* sr);

  template <TruePType PartType>
  const caf::Proxy<std::vector<caf::SRTrueParticle>>& GetNuTruthParticles(const caf::SRTrueInteractionProxy* nu);  
    //  template <RecoType IntType>
    //  const caf::Proxy<std::vector<caf::SRInteraction>>& GetRecoInteractionsFromTruths(const caf::SRProxy* sr);
  //  // usage: apply cuts on truth branch but get records from common branch
  //const caf::Proxy<std::vector<caf::SRInteraction>>& GetRecoInteractionsFromTruths(const caf::SRProxy* sr);
  //----------------------------------------------------------------------

  template<> class _IRecordSource<caf::SRInteractionProxy>
    : public _IRecordSourceDefaultImpl<caf::SRInteractionProxy>
  {
  public:
    _IRecordSource();
     
    // Weight-based ensembles are still supported
    using _IRecordSourceDefaultImpl::Ensemble;
    // But also support an ensemble based on SystShifts
    IInteractionEnsembleSource& Ensemble(const Multiverse& multiverse);

    IParticleSource& RecoParticles( const RecoType kRType) {return fParticleCollections.at(kRType);}

  protected:    
    IDDict<const FitMultiverse*, IInteractionEnsembleSource> fEnsembleSources;
 
    std::unordered_map<RecoType, VectorAdaptor<caf::SRInteraction, caf::SRRecoParticle>> fParticleCollections;

  };

  template<> class _IRecordSource<caf::SRTrueInteractionProxy>
    : public _IRecordSourceDefaultImpl<caf::SRTrueInteractionProxy>
  {
  public:
    _IRecordSource();

  //using _IRecordSourceDefaultImpl::Ensemble;
   //ITrueInteractionEnsembleSource& Ensemble(const Multiverse& multiverse);
   INuTruthParticleSource& TruthParticles(const TruePType kPType) {return fNuTruthParticleCollections.at(kPType);}

  protected:
    //IDDict<const FitMultiverse*, ITrueInteractionEnsembleSource> fEnsembleSources;
    std::unordered_map<TruePType, VectorAdaptor<caf::SRTrueInteraction, caf::SRTrueParticle>> fNuTruthParticleCollections;

  };


  //----------------------------------------------------------------------
  // Standard Record sources also provide a  source (which loops over the interactions)
  template<> class _IRecordSource<caf::SRProxy>
    : public _IRecordSourceDefaultImpl<caf::SRProxy>
  {
  public:
    _IRecordSource();

    IInteractionSource&       Interactions( const RecoType kRType ) {return fInteractionCollections.at(kRType);}    
    INDLarInteractionSource&  NDLarInteractions(  const RecoType kRType ) {return fNDLarInteractionCollections.at(kRType);}
    INuTruthSource& NuTruths() {return fNuTruths;}
    //IInteractionSource&       InteractionsTruthFiltered(){return fFilteredInteractions;}
  protected:
    std::unordered_map<RecoType, VectorAdaptor<caf::StandardRecord, caf::SRInteraction>> fInteractionCollections;
    std::unordered_map<RecoType, VectorAdaptor<caf::StandardRecord, caf::SRNDLArInt>> fNDLarInteractionCollections;
    VectorAdaptor<caf::StandardRecord, caf::SRTrueInteraction> fNuTruths{*this, GetNuTruths};
    //VectorAdaptor<caf::StandardRecord,caf::SRInteraction> fFilteredInteractions{*this,GetRecoInteractionsFromTruths};

  };


//  // --------------------------------- ********
//  // attempt to connect the nu tree with the reco / common tree
// 
//  template<> class _IRecordSource<caf::SRProxy>
//    : public _IRecordSourceDefaultImpl<caf::SRProxy>
//  {
//  public:
//    _IRecordSource();
//
//    IInteractionSource&   Interactions( const RecoType kRType ) {return fRecoTrueInteractionCollections.at(kRType);}    
//
//  protected:
//    std::unordered_map<RecoType, VectorAdaptor<caf::StandardRecord, caf::SRInteraction>> fRecoTrueInteractionCollections;
//
//  };
//
//
//  

  //----------------------------------------------------------------------
  /// Helper class for implementing looping over interactions, particles, etc
  template<class FromT, class ToT> class EnsembleVectorAdaptor:
    public PassthroughExposure<_IRecordEnsembleSink<caf::Proxy<FromT>>,
                               _IRecordEnsembleSource<caf::Proxy<ToT>>>
  {
  public:
    using Source_t = _IRecordEnsembleSource<caf::Proxy<FromT>>;
    using Func_t = std::function<const caf::Proxy<std::vector<ToT>>&(const caf::Proxy<FromT>*)>;

    EnsembleVectorAdaptor(Source_t& src, Func_t vecGetter);

    virtual void HandleSingleRecord(const caf::Proxy<FromT>* rec,
                                    double weight,
                                    int universeIdx) override;

    virtual void HandleEnsemble(const caf::Proxy<FromT>* rec,
                                const std::vector<double>& weight) override;

    virtual const ana::FitMultiverse* GetMultiverse() const {return fSource->GetMultiverse();}

  protected:
    const Source_t* fSource;
    Func_t fVecGetter;
  };

  //----------------------------------------------------------------------
  // Provide ability to get particle sources from the reco branch
  // ensemble source.

  template<> class _IRecordEnsembleSource<caf::SRInteractionProxy>
    : public _IRecordEnsembleSourceDefaultImpl<caf::SRInteractionProxy>
  {
  public:
    _IRecordEnsembleSource();

    IParticleEnsembleSource& RecoParticles( const RecoType kRType) {return fParticleCollections.at(kRType);}

  protected:
     std::unordered_map<RecoType, EnsembleVectorAdaptor<caf::SRInteraction, caf::SRRecoParticle>> fParticleCollections;

  };

  // the analogous for truth branch
  template<> class _IRecordEnsembleSource<caf::SRTrueInteractionProxy>
    : public _IRecordEnsembleSourceDefaultImpl<caf::SRTrueInteractionProxy>
  {
  public:
    _IRecordEnsembleSource();

    INuTruthParticleEnsembleSource& TruthParticles(const TruePType kPType) {return fNuTruthParticleCollections.at(kPType);}
  protected:    
     std::unordered_map<TruePType, EnsembleVectorAdaptor<caf::SRTrueInteraction, caf::SRTrueParticle>> fNuTruthParticleCollections;
  };

 /// Still missing the analogous StandardRecord sources (which loop over interactions)
  // SR->Interactions, SR->NDLARInte, SR->TrueInteraction
}
