#include "CAFAna/Core/IRecordSource.h"

//#include "CAFAna/Core/Multiverse.h"

#include "CAFAna/Core/SystShifts.h"

#include "duneanaobj/StandardRecord/Proxy/SRProxy.h"

namespace ana
{


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
  const caf::Proxy<std::vector<caf::SRInteraction>>&
  GetInteractions(const RecoType kRType,const caf::SRProxy* sr)
  {
	  if(kRType==RecoType::kDLP)  return sr->common.ixn.dlp;
    if(kRType==RecoType::kPandora) return sr->common.ixn.pandora;
    // error you have to specify which type of reconstruction "kPandora or kDLP" 
  }

  // specifically for deep learning, a generic dlp or pandora implementation is a TO DO
  const caf::Proxy<std::vector<caf::SRRecoParticle>>&
  GetRecoParticles(const RecoType kRType, const caf::SRInteractionProxy* ixn)
  {
     if (kRType==RecoType::kDLP) return ixn->part.dlp;
     if (kRType==RecoType::kPandora) return ixn->part.pandora;
     if (kRType==RecoType::kPIDA) return ixn->part.pida;
    // error you have to specify which type of reconstruction "kPandora or kDLP" 
  }

//Things I was trying so we could write loader.Interactions()[IntCut].{dlp,pandora}.RecoParticles().{dlp,pandora}[PartiCut]
//  //----------------------------------------------------------------------
////  const caf::Proxy<std::vector<caf::SRInteractionBranch>>&
//  const caf::Proxy<caf::SRInteractionBranch>&
//  GetInteractions(const caf::SRProxy* sr)
//  {
//    return sr->common.ixn; // then .dlp or .pandora
//  }
//
//  // specifically for deep learning, a generic dlp or pandora implementation is a TO DO
////  const caf::Proxy<std::vector<caf::SRRecoParticlesBranch>>&
//  const caf::Proxy<caf::SRRecoParticlesBranch>&
//  GetRecoParticles(const caf::SRInteractionProxy* ixn)
//  {
//    return ixn->part; // then .dlp or .pandora or .ida
//  }

  template <RecoType IntType>
  const caf::Proxy<std::vector<caf::SRInteraction>> & GetInteractions(const caf::SRProxy* sr)
  {
    if constexpr (IntType == RecoType::kDLP)
      return sr->common.ixn.dlp;
    else if (IntType == RecoType::kPandora)
      return sr->common.ixn.pandora;
    else
      static_assert(false, "GetInteractions() is currently uninstrumented for RecoType '" + std::string(IntType));
  }

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
      static_assert(false, "GetRecoParticles() is currently uninstrumented for RecoType '" + std::to_string(PartType));
  }

//  //----------------------------------------------------------------------

  // Instantiations
  template class VectorAdaptor<caf::StandardRecord, caf::SRInteraction>;

  template class VectorAdaptor<caf::SRInteraction, caf::SRRecoParticle>;

  // Instantiations
  //template class VectorAdaptor<caf::StandardRecord, caf::SRInteractionBranch>;
  //template class VectorAdaptor<caf::SRInteraction, caf::SRRecoParticlesBranch>;

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

}
