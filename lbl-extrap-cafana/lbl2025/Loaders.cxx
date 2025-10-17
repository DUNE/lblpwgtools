#include "lbl2025/Loaders.h"

namespace lbl2025
{

	const std::map<std::tuple<ana::DataMC, caf::Det_t, ana::SwappingConfig>, std::string> kLoaderPaths_FHC
	{
        	{{ana::DataMC::kMC, caf::Det_t::kFARDET, ana::SwappingConfig::kNonSwap},   {"/pnfs/dune/persistent/staging/fardet-vd/6d/35/fardet-vd_mc_list-merge_physics_cafmaker_dunevd10kt_1x8x6_3view_30deg_runreco-nuenergy_geov3_root-tuple_merged_skip000000_lim000060_final_20241024T024241.root"}},
			{{ana::DataMC::kMC, caf::Det_t::kFARDET, ana::SwappingConfig::kNueSwap},   {"/pnfs/dune/persistent/staging/fardet-vd/72/30/fardet-vd_mc_list-nu_numu2nue_nue2nutau_physics_cafmaker_dunevd10kt_1x8x6_3view_30deg_runreco-nuenergy_geov3_root-tuple_merged_skip000000_lim000073_final_20241031T172350.root"}},
			{{ana::DataMC::kMC, caf::Det_t::kFARDET, ana::SwappingConfig::kNuTauSwap}, {"/pnfs/dune/persistent/staging/fardet-vd/ac/50/fardet-vd_mc_list-nu_numu2nutau_nue2numu_physics_cafmaker_dunevd10kt_1x8x6_3view_30deg_runreco-nuenergy_geov3_root-tuple_merged_skip000000_lim000066_final_20241103T203912.root"}},
   // TODO: Completely arbitrary files.
			{{ana::DataMC::kMC, caf::Det_t::kNEARDET, ana::SwappingConfig::kNonSwap}, {"/pnfs/dune/persistent/physicsgroups/dunendsim/abooth/nd-production/MicroProdN4p1/run-cafmaker/MicroProdN4p1_NDComplex_FHC.caf.full.light.spineonly/CAF/0002000/MicroProdN4p1_NDComplex_FHC.caf.full.light.spineonly.0002510.CAF.root"}},
			{{ana::DataMC::kData, caf::Det_t::kNEARDET, ana::SwappingConfig::kNonSwap}, {"/pnfs/dune/persistent/physicsgroups/dunendsim/abooth/nd-production/MicroProdN4p1/run-cafmaker/MicroProdN4p1_NDComplex_FHC.caf.full.light.spineonly/CAF/0002000/MicroProdN4p1_NDComplex_FHC.caf.full.light.spineonly.0002551.CAF.root"}},
    };
	const std::map<std::tuple<ana::DataMC, caf::Det_t, ana::SwappingConfig>, std::string> kLoaderPaths_RHC
	{
		{{ana::DataMC::kMC, caf::Det_t::kFARDET, ana::SwappingConfig::kNonSwap},   {"/pnfs/dune/persistent/staging/fardet-vd/8c/4f/fardet-vd_mc_list-anus_vd_physics_cafmaker_dunevd10kt_1x8x6_3view_30deg_runreco-nuenergy_geov3_root-tuple_merged_skip000000_lim000067_final_20241030T204817.root"}},
		{{ana::DataMC::kMC, caf::Det_t::kFARDET, ana::SwappingConfig::kNueSwap},   {"/pnfs/dune/persistent/staging/fardet-vd/aa/3f/fardet-vd_mc_list-anu_numu2nue_nue2nutau_physics_cafmaker_dunevd10kt_1x8x6_3view_30deg_runreco-nuenergy_geov3_root-tuple_merged_skip000000_lim000069_final_20241103T200206.root"}},
		{{ana::DataMC::kMC, caf::Det_t::kFARDET, ana::SwappingConfig::kNuTauSwap}, {"/pnfs/dune/persistent/staging/fardet-vd/59/39/fardet-vd_mc_list-anu_numu2nutau_nue2numu_physics_cafmaker_dunevd10kt_1x8x6_3view_30deg_runreco-nuenergy_geov3_root-tuple_merged_skip000000_lim000070_final_20241103T202115.root"}}
	};

	// -----------------------------------------------------------------------
	Loaders::Loaders(const ana::RecoType & recoType)
		: fFHC(std::make_unique<ana::Loaders>(recoType)), fRHC(std::make_unique<ana::Loaders>(recoType))
	{
		// I miss Python
		for (const auto & hcPair : std::map<ana::Loaders*, decltype(kLoaderPaths_FHC)*>{{fFHC.get(), &kLoaderPaths_FHC}, {fRHC.get(), &kLoaderPaths_RHC}})
		{
			for (const auto & configPathPair : *(hcPair.second))
			{
				hcPair.first->AddSource(std::make_unique<ana::SpectrumLoader>(configPathPair.second, "cafmaker"),
						  std::get<0>(configPathPair.first),
						  std::get<1>(configPathPair.first),
						  std::get<2>(configPathPair.first));
				// std::cout << "    added source: " << &hcPair.first->GetSource(std::get<0>(configPathPair.first),
				//                                                               std::get<1>(configPathPair.first),
				//                                                               std::get<2>(configPathPair.first))
				//           << "\n";
			}
		}
	}

	// -----------------------------------------------------------------------
	ana::Loaders* Loaders::GetLoaders(const ana::FluxType& flux)
	{
		return flux == ana::FluxType::kFHC ? fFHC.get() : fRHC.get();
	}

	// -----------------------------------------------------------------------
	void Loaders::Go()
	{
		fFHC->Go();
		fRHC->Go();
	}

}
