#include "CAFAna/Core/IRecordSource.h"

#include "CAFAna/Core/Multiverse.h"

#include "CAFAna/Core/SystShifts.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

# include <assert.h>

namespace ana
{

  //----------------------------------------------------------------------
  class ShiftedInteractionEnsembleSource:
    public PassthroughExposure<IInteractionSink, IInteractionEnsembleSource>
  {
  public:
    ShiftedInteractionEnsembleSource(IInteractionSource& src,
                               const Multiverse& multiverse);

    virtual void HandleRecord(const caf::SRInteractionProxy* ixn, double weight) override;

    virtual const ana::FitMultiverse* GetMultiverse() const override {return fMultiverse;}

  protected:
    const ana::FitMultiverse* fMultiverse;
    std::vector<SystShifts> fShifts;
  };

  //----------------------------------------------------------------------
  ShiftedInteractionEnsembleSource::
  ShiftedInteractionEnsembleSource(IInteractionSource& src, const Multiverse& multiverse)
    : fMultiverse(&multiverse)
  {
    src.Register(this);

    // Turn the universes into concrete SystShifts objects up-front
    fShifts.reserve(multiverse.NUniv());
    // This next line is not working...
    for(unsigned int i = 0; i < multiverse.NUniv(); ++i) fShifts.emplace_back(multiverse.GetUniverse(i));
  }

  //----------------------------------------------------------------------
  void ShiftedInteractionEnsembleSource::HandleRecord(const caf::SRInteractionProxy* slc,
                                                double weight)
  {
    if(weight == 0) return;

    std::vector<double> weights(fShifts.size(), weight);

    bool anyShifted = false;

    for(unsigned int univIdx = 0; univIdx < fShifts.size(); ++univIdx){
      // Need to provide a clean slate for each new set of systematic
      // shifts to work from. Copying the whole StandardRecord is pretty
      // expensive, so modify it in place and revert it afterwards.

      caf::SRProxySystController::BeginTransaction();

      bool shifted = false;

      // Can special-case nominal to not pay cost of Shift()
      if(!fShifts[univIdx].IsNominal()){
        // const_cast is naughty. I hope the fact that we put the record back
        // afterwards absolves most sins.
        fShifts[univIdx].Shift(const_cast<caf::SRInteractionProxy*>(slc), weights[univIdx]);
        // If there were only weighting systs applied then the cached
        // nominal values are still valid.
        shifted = caf::SRProxySystController::AnyShifted();
      }

      // Interaction was shifted or we are already in the slow path, so we have to
      // handle this individually
      if((shifted || anyShifted) && weights[univIdx] != 0)
        for(IInteractionEnsembleSink* sink: fSinks)
          sink->HandleSingleRecord(slc, weights[univIdx], univIdx);

      // Return StandardRecord to its unshifted form ready for the next
      // histogram.
      caf::SRProxySystController::Rollback();

      // We entered the slow path for the first time for this universe, have to
      // catch up with all the other universes we were hoping to be able to
      // handle in the fast path.
      if(shifted && !anyShifted){
        anyShifted = true;

        for(unsigned int prevIdx = 0; prevIdx < univIdx; ++prevIdx){
          if(weights[prevIdx] == 0) continue;

          for(IInteractionEnsembleSink* sink: fSinks){
            sink->HandleSingleRecord(slc, weights[prevIdx], prevIdx);
          }
        } // end for prevIdx
      } // end if shifted for the first time
    } // end for univIdx

    // Fast path in case none of the records got rewritten, can treat as an
    // ensemble with weights.
    if(!anyShifted){
      for(IInteractionEnsembleSink* sink: fSinks){
        sink->HandleEnsemble(slc, weights);
      }
    }
  }

  //----------------------------------------------------------------------
  IInteractionEnsembleSource& IInteractionSource::
  Ensemble(const Multiverse& multiverse)
  {
    return fEnsembleSources.template Get<ShiftedInteractionEnsembleSource>(&multiverse, *this, multiverse);
  }
  //----------------------------------------------------------------------
  // Truth branch version of ensembles??


  //----------------------------------------------------------------------

  template<class FromT, class ToT> VectorAdaptor<FromT, ToT>::
  VectorAdaptor(_IRecordSource<caf::Proxy<FromT>>& src,
                Func_t vecGetter)
    : fVecGetter(vecGetter)
  {
    src.Register(this);
  }

  //----------------------------------------------------------------------
  template<class FromT, class ToT> void VectorAdaptor<FromT, ToT>::
  HandleRecord(const caf::Proxy<FromT>* rec, double weight)
  {
    for(const caf::Proxy<ToT>& to: fVecGetter(rec))
      for(auto& sink: _IRecordSource<caf::Proxy<ToT>>::fSinks)
        sink->HandleRecord(&to, weight);
  }
  //----------------------------------------------------------------------
  template<class FromT, class ToT> EnsembleVectorAdaptor<FromT, ToT>::
  EnsembleVectorAdaptor(_IRecordEnsembleSource<caf::Proxy<FromT>>& src,
                        Func_t vecGetter)
    : fSource(&src), fVecGetter(vecGetter)
  {
    src.Register(this);
  }

  //----------------------------------------------------------------------
  template<class FromT, class ToT> void EnsembleVectorAdaptor<FromT, ToT>::
  HandleSingleRecord(const caf::Proxy<FromT>* rec,
                     double weight,
                     int universeId)
  {
    for(const caf::Proxy<ToT>& to: fVecGetter(rec))
      for(auto& sink: _IRecordEnsembleSource<caf::Proxy<ToT>>::fSinks)
        sink->HandleSingleRecord(&to, weight, universeId);
  }

  //----------------------------------------------------------------------
  template<class FromT, class ToT> void EnsembleVectorAdaptor<FromT, ToT>::
  HandleEnsemble(const caf::Proxy<FromT>* rec,
                 const std::vector<double>& weights)
  {
    for(const caf::Proxy<ToT>& to: fVecGetter(rec))
      for(auto& sink: _IRecordEnsembleSource<caf::Proxy<ToT>>::fSinks)
        sink->HandleEnsemble(&to, weights);
  }

  //----------------------------------------------------------------------
  //----------------------------------------------------------------------

  template <RecoType PartType>
  const caf::Proxy<std::vector<caf::SRRecoParticle>>& GetRecoParticles(const caf::SRInteractionProxy* ixn)
  {
    if constexpr (PartType == RecoType::kDLP)
      return ixn->part.dlp;
    else if (PartType == RecoType::kPandora)
      return ixn->part.pandora;
    else if (PartType == RecoType::kPIDA)
      return ixn->part.pida;
    else
      assert(false && "GetRecoParticles() is currently instrumented only for kDLP, kPandora or kPIDA only");
        //static_assertor RecoType " + std::to_string(PartType));
  }
//  template <RecoType PartType>
//  const caf::Proxy<std::vector<caf::SRTrack>>& GetTracks(const caf::SRInteractionProxy* ixn);
//{
//
//}
  template <RecoType IntType>
  const caf::Proxy<std::vector<caf::SRInteraction>> & GetInteractions(const caf::SRProxy* sr)
  {
    if constexpr(IntType == RecoType::kDLP)
      return sr->common.ixn.dlp;
    else if (IntType == RecoType::kPandora)
      return sr->common.ixn.pandora;
    else
      assert(false &&"GetInteractions() is currently instrumented for RecoType kDLP or kPandora only" );
  }

  template <RecoType IntType>
  const caf::Proxy<std::vector<caf::SRNDLArInt>> & GetNDLarInteractions(const caf::SRProxy* sr)
  {
    if constexpr(IntType == RecoType::kDLP)
      return sr->nd.lar.dlp;
    else if (IntType == RecoType::kPandora)
      return sr->nd.lar.pandora;
    else
      assert(false &&"GetNDLarInteractions() is currently instrumented for RecoType kDLP or kPandora only" );
  }

  const caf::Proxy<std::vector<caf::SRTrueInteraction>>&
  GetNuTruths(const caf::SRProxy* sr)
  {
    return sr->mc.nu;
  }

  template <TruePType PartType>
  const caf::Proxy<std::vector<caf::SRTrueParticle>> &  GetNuTruthParticles(const caf::SRTrueInteractionProxy* nu)
  {
    if constexpr(PartType == TruePType::kPrim)
      return nu->prim;
    else if (PartType == TruePType::kSec)
       return nu->sec;
    else if (PartType == TruePType::kPreFSI)
       return nu->prefsi;
    else
     assert(false &&"GetNuTruthParticles() is currently instrumented for TruePType kPrim, kSec or kPreFSI only" );
  }

// SR to Ixn or SR to TruIxn, only the matched ones?
//
////  // From True interactions to recointeractions
////  // usage: apply cuts on truth branch but get records from common branch
////template <RecoType IntType, int pdg>
//  const caf::Proxy<std::vector<caf::SRInteraction>>& GetRecoInteractionsFromTruths(const caf::SRProxy* sr){
//   ///< Interaction ID == 'vertexID' from edep-sim (ND) or GENIE record id (FD)
//    //auto nus = sr->mc.nu;
//   ///< Indices of SRTrueInteraction(s), if relevant (use this index in SRTruthBranch::nu to get them)    
//    //auto ixns = sr->common.ixn;
//    bool pdgmatch= false;
//    // construct a vector of SRInteraction that match the SRTrueIteraction
//    std::vector<caf::SRInteraction> temp = sr->common.ixn.dlp;
//
//    //if constexpr(IntType == RecoType::kDLP){
//        for(long unsigned nixn = 0; nixn < sr->common.ixn.dlp.size(); nixn++){ 
//                  int recoid = sr->common.ixn.dlp[nixn].id;
//                  // find the truth thing....
//                  pdgmatch = abs(sr->mc.nu[recoid].pdg ) == 14;
//                  if (pdgmatch ) temp.emplace_back(sr->common.ixn.dlp[nixn]/*the matched index interaction*/); 
//                }
//      const caf::Proxy<std::vector<caf::SRInteraction>> ret = temp;
//      return &ret;
//    //}
////    else if (IntType == RecoType::kPandora)
////          sr->common.ixn.pandora.id;
////      return sr->common.ixn.pandora;
////    else
//  //    assert(false &&"GetRecoInteractionsFromTruths() is currently instrumented for RecoType kDLP or kPandora only" );
//  }

  //----------------------------------------------------------------------

  // Instantiations
  template class VectorAdaptor<caf::SRInteraction, caf::SRRecoParticle>;

  template class VectorAdaptor<caf::StandardRecord, caf::SRInteraction>;

  template class VectorAdaptor<caf::StandardRecord, caf::SRNDLArInt>;

  template class VectorAdaptor<caf::StandardRecord, caf::SRTrueInteraction>;

  template class VectorAdaptor<caf::SRTrueInteraction, caf::SRTrueParticle>;

  template class EnsembleVectorAdaptor<caf::SRInteraction, caf::SRRecoParticle>;

  template class EnsembleVectorAdaptor<caf::SRTrueInteraction, caf::SRTrueParticle>;
  // -----------------------------------------------------------------------
  _IRecordSource<caf::SRInteractionProxy>::_IRecordSource()
  {
    // it would be better (lots less boilerplate) to do this in the initializers section,
    // but I can't work out why it won't compile when the items are written
    // in initializer list format
    fParticleCollections.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(RecoType::kDLP),
                                 std::forward_as_tuple(*this, GetRecoParticles<RecoType::kDLP>));
    fParticleCollections.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(RecoType::kPandora),
                                 std::forward_as_tuple(*this, GetRecoParticles<RecoType::kPandora>));
    fParticleCollections.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(RecoType::kPIDA),
                                 std::forward_as_tuple(*this, GetRecoParticles<RecoType::kPIDA>));
  }

  // -----------------------------------------------------------------------
   _IRecordSource<caf::SRProxy>::_IRecordSource()
  {
    fInteractionCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(RecoType::kDLP),
                                    std::forward_as_tuple(*this, GetInteractions<RecoType::kDLP>));
    fInteractionCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(RecoType::kPandora),
                                    std::forward_as_tuple(*this, GetInteractions<RecoType::kPandora>));
    fNDLarInteractionCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(RecoType::kDLP),
                                    std::forward_as_tuple(*this, GetNDLarInteractions<RecoType::kDLP>));
    fNDLarInteractionCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(RecoType::kPandora),
                                    std::forward_as_tuple(*this, GetNDLarInteractions<RecoType::kPandora>));
  }
 // -----------------------------------------------------------------------
   _IRecordSource<caf::SRTrueInteractionProxy>::_IRecordSource()
  {
    fNuTruthParticleCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(TruePType::kPrim),
                                    std::forward_as_tuple(*this, GetNuTruthParticles<TruePType::kPrim>));
    fNuTruthParticleCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(TruePType::kSec),
                                    std::forward_as_tuple(*this, GetNuTruthParticles<TruePType::kSec>));
    fNuTruthParticleCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(TruePType::kPreFSI),
                                    std::forward_as_tuple(*this, GetNuTruthParticles<TruePType::kPreFSI>));
  }

// -----------------------------------------------------------------------
   _IRecordEnsembleSource<caf::SRInteractionProxy>::_IRecordEnsembleSource()
  {
    // it would be better (lots less boilerplate) to do this in the initializers section,
    // but I can't work out why it won't compile when the items are written
    // in initializer list format
    fParticleCollections.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(RecoType::kDLP),
                                 std::forward_as_tuple(*this, GetRecoParticles<RecoType::kDLP>));
    fParticleCollections.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(RecoType::kPandora),
                                 std::forward_as_tuple(*this, GetRecoParticles<RecoType::kPandora>));
    fParticleCollections.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(RecoType::kPIDA),
                                 std::forward_as_tuple(*this, GetRecoParticles<RecoType::kPIDA>));
  }
// -----------------------------------------------------------------------
   _IRecordEnsembleSource<caf::SRTrueInteractionProxy>::_IRecordEnsembleSource()
  {
    fNuTruthParticleCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(TruePType::kPrim),
                                    std::forward_as_tuple(*this, GetNuTruthParticles<TruePType::kPrim>));
    fNuTruthParticleCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(TruePType::kSec),
                                    std::forward_as_tuple(*this, GetNuTruthParticles<TruePType::kSec>));
    fNuTruthParticleCollections.emplace(std::piecewise_construct,
                                    std::forward_as_tuple(TruePType::kPreFSI),
                                    std::forward_as_tuple(*this, GetNuTruthParticles<TruePType::kPreFSI>));
  }


}
