#include "CAFAna/Core/IRecordSource.h"

//#include "CAFAna/Core/Multiverse.h"

#include "CAFAna/Core/SystShifts.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

namespace ana
{

  // These deal with ensemblees, adapted for DUNE SR and Interaction but will keep commented out because we are not doing that yet!
/*
*  //----------------------------------------------------------------------
*  class ShiftedInteractionEnsembleSource:
*    public PassthroughExposure<IInteractionSink, IInteractionEnsembleSource>
*  {
*  public:
*    ShiftedInteractionEnsembleSource(IInteractionSource& src,
*                               const Multiverse& multiverse);
*
*    virtual void HandleRecord(const caf::SRInteractionProxy* slc, double weight) override;
*
*    virtual const ana::FitMultiverse* GetMultiverse() const override {return fMultiverse;}
*
*  protected:
*    const Multiverse* fMultiverse;
*    std::vector<SystShifts> fShifts;
*  };
*
*  //----------------------------------------------------------------------
*  ShiftedInteractionEnsembleSource::
*  ShiftedInteractionEnsembleSource(IInteractionSource& src, const Multiverse& multiverse)
*    : fMultiverse(&multiverse)
*  {
*    src.Register(this);
*
*    // Turn the universes into concrete SystShifts objects up-front
*    fShifts.reserve(multiverse.NUniv());
*    for(unsigned int i = 0; i < multiverse.NUniv(); ++i) fShifts.emplace_back(multiverse.GetUniverse(i));
*  }
*
*  //----------------------------------------------------------------------
*  void ShiftedInteractionEnsembleSource::HandleRecord(const caf::SRInteractionProxy* slc,
*                                                double weight)
*  {
*    if(weight == 0) return;
*
*    std::vector<double> weights(fShifts.size(), weight);
*
*    bool anyShifted = false;
*
*    for(unsigned int univIdx = 0; univIdx < fShifts.size(); ++univIdx){
*      // Need to provide a clean slate for each new set of systematic
*      // shifts to work from. Copying the whole StandardRecord is pretty
*      // expensive, so modify it in place and revert it afterwards.
*
*      caf::SRProxySystController::BeginTransaction();
*
*      bool shifted = false;
*
*      // Can special-case nominal to not pay cost of Shift()
*      if(!fShifts[univIdx].IsNominal()){
*        // const_cast is naughty. I hope the fact that we put the record back
*        // afterwards absolves most sins.
*        fShifts[univIdx].Shift(const_cast<caf::SRInteractionProxy*>(slc), weights[univIdx]);
*        // If there were only weighting systs applied then the cached
*        // nominal values are still valid.
*        shifted = caf::SRProxySystController::AnyShifted();
*      }
*
*      // Interaction was shifted or we are already in the slow path, so we have to
*      // handle this individually
*      if((shifted || anyShifted) && weights[univIdx] != 0)
*        for(IInteractionEnsembleSink* sink: fSinks)
*          sink->HandleSingleRecord(slc, weights[univIdx], univIdx);
*
*      // Return StandardRecord to its unshifted form ready for the next
*      // histogram.
*      caf::SRProxySystController::Rollback();
*
*      // We entered the slow path for the first time for this universe, have to
*      // catch up with all the other universes we were hoping to be able to
*      // handle in the fast path.
*      if(shifted && !anyShifted){
*        anyShifted = true;
*
*        for(unsigned int prevIdx = 0; prevIdx < univIdx; ++prevIdx){
*          if(weights[prevIdx] == 0) continue;
*
*          for(IInteractionEnsembleSink* sink: fSinks){
*            sink->HandleSingleRecord(slc, weights[prevIdx], prevIdx);
*          }
*        } // end for prevIdx
*      } // end if shifted for the first time
*    } // end for univIdx
*
*    // Fast path in case none of the records got rewritten, can treat as an
*    // ensemble with weights.
*    if(!anyShifted){
*      for(IInteractionEnsembleSink* sink: fSinks){
*        sink->HandleEnsemble(slc, weights);
*      }
*    }
*  }
*
*  //----------------------------------------------------------------------
*  IInteractionEnsembleSource& IInteractionSource::
*  Ensemble(const Multiverse& multiverse)
*  {
*    return fEnsembleSources.template Get<ShiftedInteractionEnsembleSource>(&multiverse, *this, multiverse);
*  }
*/
  //----------------------------------------------------------------------
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
/*
*  //----------------------------------------------------------------------
*  template<class FromT, class ToT> EnsembleVectorAdaptor<FromT, ToT>::
*  EnsembleVectorAdaptor(_IRecordEnsembleSource<caf::Proxy<FromT>>& src,
*                        Func_t vecGetter)
*    : fSource(&src), fVecGetter(vecGetter)
*  {
*    src.Register(this);
*  }
*
*  //----------------------------------------------------------------------
*  template<class FromT, class ToT> void EnsembleVectorAdaptor<FromT, ToT>::
*  HandleSingleRecord(const caf::Proxy<FromT>* rec,
*                     double weight,
*                     int universeId)
*  {
*    for(const caf::Proxy<ToT>& to: fVecGetter(rec))
*      for(auto& sink: _IRecordEnsembleSource<caf::Proxy<ToT>>::fSinks)
*        sink->HandleSingleRecord(&to, weight, universeId);
*  }
*
*  //----------------------------------------------------------------------
*  template<class FromT, class ToT> void EnsembleVectorAdaptor<FromT, ToT>::
*  HandleEnsemble(const caf::Proxy<FromT>* rec,
*                 const std::vector<double>& weights)
*  {
*    for(const caf::Proxy<ToT>& to: fVecGetter(rec))
*      for(auto& sink: _IRecordEnsembleSource<caf::Proxy<ToT>>::fSinks)
*        sink->HandleEnsemble(&to, weights);
*  }
*/
  //----------------------------------------------------------------------
  const caf::Proxy<std::vector<caf::SRInteraction>>&
  GetInteractions(const caf::SRProxy* sr)
  {
    return sr->common.ixn.dlp;//by default chose dlp but should also write the pandora one
  }

  // specifically for deep learning, a generic dlp or pandora implementation is a TO DO
  const caf::Proxy<std::vector<caf::SRRecoParticle>>&
  GetRecoParticles(const caf::SRInteractionProxy* ixn)
  {
    return ixn->part.dlp;
  }


  //----------------------------------------------------------------------
  //----------------------------------------------------------------------

  // Instantiations
  template class VectorAdaptor<caf::StandardRecord, caf::SRInteraction>;

  template class VectorAdaptor<caf::SRInteraction, caf::SRRecoParticle>;


//  template class EnsembleVectorAdaptor<caf::StandardRecord, caf::SRInteraction>;
//  template class EnsembleVectorAdaptor<caf::SRInteraction, caf::SRTrack>;

}