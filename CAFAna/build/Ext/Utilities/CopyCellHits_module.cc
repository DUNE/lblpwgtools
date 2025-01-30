////////////////////////////////////////////////////////////////////////
/// \brief  A module to copy hits from selected clusers into a vector of cell hits
/// \author edniner@fnal.gov
/// \date
////////////////////////////////////////////////////////////////////////

// Framework includes
#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"

// NOvA includes
#include "RecoBase/CellHit.h"
#include "RecoBase/Cluster.h"

namespace util {

  /// A module to copy hits from selected clusers into a vector of cell hits
  class CopyCellHits : public art::EDFilter {

  public:

    explicit CopyCellHits(fhicl::ParameterSet const &pset);
    virtual ~CopyCellHits();
    void beginJob();
    bool filter(art::Event& evt);
  protected:
    const art::ProductToken<std::vector<rb::Cluster>> fClusterToken;

    bool fOutputRawDigits;
    bool fOutputCellHits;
  };

  //___________________________________________________________________________
  CopyCellHits::CopyCellHits(fhicl::ParameterSet const& pset):
    fClusterToken(consumes<std::vector<rb::Cluster>>(pset.get<std::string>("ClusterLabel"))),
    fOutputRawDigits(pset.get<bool>("OutputRawDigits")),
    fOutputCellHits(pset.get<bool>("OutputCellHits"))
  {
    if(fOutputRawDigits) produces<std::vector<rawdata::RawDigit>>();
    if(fOutputCellHits)  produces<std::vector<rb::CellHit>>();
  }

  //___________________________________________________________________________
  CopyCellHits::~CopyCellHits()
  {
  }

  //___________________________________________________________________________
  void CopyCellHits::beginJob()
  {
  }

  //___________________________________________________________________________
  template<class T> std::unique_ptr<std::vector<T>> GetHits(const std::vector<rb::Cluster>& clusts)
  {
    auto hitcol = std::make_unique<std::vector<T>>();

    for(const rb::Cluster& clust: clusts)
      for(unsigned int iCell = 0; iCell < clust.NCell(); ++iCell)
        hitcol->push_back(*clust.Cell(iCell));

    return hitcol;
  }

  //___________________________________________________________________________
  bool CopyCellHits::filter(art::Event& evt)
  {
    art::Handle< std::vector<rb::Cluster> > clustcol;
    evt.getByToken(fClusterToken, clustcol);

    if (clustcol->size() == 0) return false;

    if(fOutputRawDigits) evt.put(GetHits<rawdata::RawDigit>(*clustcol));
    if(fOutputCellHits) evt.put(GetHits<rb::CellHit>(*clustcol));

    return true;
  }

  DEFINE_ART_MODULE(CopyCellHits)

} // namespace
