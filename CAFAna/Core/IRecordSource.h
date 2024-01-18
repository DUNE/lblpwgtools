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

  using INDLarInteractionSource = _IRecordSource<caf::SRNDLArIntProxy>;

  using INuTruthSource = _IRecordSource<caf::SRTrueInteractionProxy>;

  using IParticleSource = _IRecordSource<caf::SRRecoParticleProxy>;

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

  template <RecoType PartType>
  const caf::Proxy<std::vector<caf::SRRecoParticle>>& GetRecoParticles(const caf::SRInteractionProxy* ixn);

  template <RecoType IntType>
  const caf::Proxy<std::vector<caf::SRInteraction>>& GetInteractions(const caf::SRProxy* sr);

  const caf::Proxy<std::vector<caf::SRTrueInteraction>>& GetNuTruths(const caf::SRProxy* sr);

  template <RecoType IntType>
  const caf::Proxy<std::vector<caf::SRNDLArInt>>& GetNDLarInteractions(const caf::SRProxy* sr);


  //----------------------------------------------------------------------

  template<> class _IRecordSource<caf::SRInteractionProxy>
    : public _IRecordSourceDefaultImpl<caf::SRInteractionProxy>
  {
  public:
    _IRecordSource();

    IParticleSource& RecoParticles( const RecoType kRType) {return fParticleCollections.at(kRType);}

  protected:    
    std::unordered_map<RecoType, VectorAdaptor<caf::SRInteraction, caf::SRRecoParticle>> fParticleCollections;

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

  protected:
    std::unordered_map<RecoType, VectorAdaptor<caf::StandardRecord, caf::SRInteraction>> fInteractionCollections;
    std::unordered_map<RecoType, VectorAdaptor<caf::StandardRecord, caf::SRNDLArInt>> fNDLarInteractionCollections;
    VectorAdaptor<caf::StandardRecord, caf::SRTrueInteraction> fNuTruths{*this, GetNuTruths};

  };

  //----------------------------------------------------------------------


}
