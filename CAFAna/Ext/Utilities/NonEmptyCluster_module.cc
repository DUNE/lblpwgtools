////////////////////////////////////////////////////////////////////////
/// \brief  A module to filter events which have a collection of empty clusters
/// \author ahimmel@fnal.gov
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
  class NonEmptyCluster : public art::EDFilter {

  public:

    explicit NonEmptyCluster(fhicl::ParameterSet const &pset);
    virtual ~NonEmptyCluster();
    void beginJob();
    bool filter(art::Event& evt);
  protected:
    const art::ProductToken<std::vector<rb::Cluster>> fClusterToken;
  };

  //___________________________________________________________________________
  NonEmptyCluster::NonEmptyCluster(fhicl::ParameterSet const& pset):
    fClusterToken(consumes<std::vector<rb::Cluster>>(pset.get<std::string>("ClusterLabel")))
  {
  }

  //___________________________________________________________________________
  NonEmptyCluster::~NonEmptyCluster()
  {
  }

  //___________________________________________________________________________
  void NonEmptyCluster::beginJob()
  {
  }

  //___________________________________________________________________________
  bool NonEmptyCluster::filter(art::Event& evt)
  {
    art::Handle< std::vector<rb::Cluster> > clustcol;
    evt.getByToken(fClusterToken, clustcol);

    int nhits = 0;
    for(const rb::Cluster& clust: *clustcol)
      nhits += clust.NCell();

    return (nhits > 0);
  }

  DEFINE_ART_MODULE(NonEmptyCluster)

} // namespace
