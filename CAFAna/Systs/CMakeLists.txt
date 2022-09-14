set(Systs_implementation_files
  AnaSysts.cxx
  DUNEFluxSysts.cxx
  RecoEnergyNDSysts.cxx
  DUNENDSysts.cxx
  DUNEXSecSysts.cxx
  DUNEXSecSystsPCA.cxx
  EnergySysts.cxx
  FDRecoSysts.cxx
  XSecSysts.cxx
  NDRecoSysts.cxx
  NuOnESysts.cxx
  CrazyFluxFakeData.cxx
  SystComponentScale.cxx
  Systs.cxx
  UniverseOracle.cxx
  XSecSystList.cxx
  OffAxisFluxUncertaintyHelper.cxx
  NewOffAxisFluxUncertainty2022Helper.cxx)

set(Systs_header_files
  AnaSysts.h
  DUNEFluxSysts.h
  RecoEnergyNDSysts.h
  DUNENDSysts.h
  DUNEXSecSysts.h
  DUNEXSecSystsPCA.h
  EnergySysts.h
  FDRecoSysts.h
  XSecSysts.h
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
  NuWroReweightFakeData.h
  CrazyFluxFakeData.h
  UniverseOracle.h
  XSecSystList.h
  OffAxisFluxUncertaintyHelper.h
  NewOffAxisFluxUncertainty2022Helper.h)

add_library(CAFAnaSysts SHARED ${Systs_implementation_files})
target_link_libraries(CAFAnaSysts CAFAnaExperiment TH2Jagged::All)

set_target_properties(CAFAnaSysts PROPERTIES
  PUBLIC_HEADER "${Systs_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaSysts BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaSysts 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Systs/)

set(SYST_INPUT_ROOT_FILES
  flux_shifts_CDR.root
  flux_shifts_Nov17.root
  flux_shifts_OffAxis.root
  flux_shifts_OffAxis2022.root
  xsec_shifts.root
  modelComp.root
  ND_eff_syst.root
  det_sys_cov.root
  det_sys_cov_v3binning.root
  OnAxis4m280kAWeights.root
)

install(FILES ${SYST_INPUT_ROOT_FILES} DESTINATION Systs)
