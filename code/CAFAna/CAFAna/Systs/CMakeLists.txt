set(Systs_implementation_files
  DUNEFluxSysts.cxx
  DUNENDSysts.cxx
  DUNEXSecSysts.cxx
  DUNEXSecSystsPCA.cxx
  EnergySysts.cxx
  FDRecoSysts.cxx
  GenieSysts.cxx
  NDRecoSysts.cxx
  NuOnESysts.cxx
  SystComponentScale.cxx
  Systs.cxx)

set(Systs_header_files
  DUNEFluxSysts.h
  DUNENDSysts.h
  DUNEXSecSysts.h
  DUNEXSecSystsPCA.h
  EnergySysts.h
  FDRecoSysts.h
  GenieSysts.h
  NDRecoSysts.h
  NuOnESysts.h
  SystComponentScale.h
  Systs.h
  MissingProtonFakeData.h
  BDTReweighter.h
  GeneratorReweight_nuebar_FD_RHC.h
  GeneratorReweight_nue_FD_FHC.h
  GeneratorReweight_nue_FD_RHC.h
  GeneratorReweight_numubar_FD_RHC.h
  GeneratorReweight_numubar_ND_RHC.h
  GeneratorReweight_numu_FD_FHC.h
  GeneratorReweight_numu_FD_RHC.h
  GeneratorReweight_numu_ND_FHC.h
  GeneratorReweight_numu_ND_RHC.h
  MissingProtonFakeData_BDTRW_FHC.h
  MissingProtonFakeData_BDTRW_RHC.h
  NuWroReweightFakeData.h)

add_library(CAFAnaSysts SHARED ${Systs_implementation_files})
target_link_libraries(CAFAnaSysts)

install(TARGETS CAFAnaSysts DESTINATION lib)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaSysts gperftools)
  target_link_libraries(CAFAnaSysts ${GPERF_LINKER_FLAGS})
endif()

install(FILES ${Systs_header_files} DESTINATION include/CAFAna/Systs)

set(SYST_INPUT_ROOT_FILES
  flux_shifts.root
  flux_shifts_wOffAxis.root
  xsec_shifts.root
  modelComp.root
  ND_eff_syst.root
#  ND_syst_cov_withEscale.root
  ND_syst_cov_withRes.root)

install(FILES ${SYST_INPUT_ROOT_FILES} DESTINATION Systs)
