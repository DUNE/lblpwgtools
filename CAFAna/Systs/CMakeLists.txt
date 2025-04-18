set(Systs_implementation_files
  AnaSysts.cxx
  DUNEFluxSysts.cxx
  DUNEXSecSysts.cxx
  DUNEXSecSystsPCA.cxx
  XSecSysts.cxx
  NuWroReweightFakeData.cxx
  CrazyFluxFakeData.cxx
  SystComponentScale.cxx
  UniverseOracle.cxx
  XSecSystList.cxx
  OffAxisFluxUncertaintyHelper.cxx
  NewOffAxisFluxUncertainty2022Helper.cxx)

set(Systs_header_files
  AnaSysts.h
  DUNEFluxSysts.h
  DUNEXSecSysts.h
  DUNEXSecSystsPCA.h
  XSecSysts.h
  SystComponentScale.h
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
target_link_libraries(CAFAnaSysts ${STANDARDRECORD} CAFAnaExperiment TH2Jagged::All)

if(NOT DEFINED BUILD_FLUX_SYSTS)
  message(WARNING "Not building flux systematics code.  Include `-DBUILD_FLUX_SYSTS` in the 'extra CMake arguments' passed to `standalone_configure_and_build.sh` to include it.")
endif()

if(NOT DEFINED BUILD_XSEC_SYSTS)
  message(WARNING "Not building xsec systematics code.  Include `-DBUILD_XSEC_SYSTS` in the 'extra CMake arguments' passed to `standalone_configure_and_build.sh` to include it.")
endif()

if(NOT DEFINED BUILD_FDS_SYSTS)
  message(WARNING "Not building fake data systematics code.  Include `-DBUILD_XSEC_SYSTS` in the 'extra CMake arguments' passed to `standalone_configure_and_build.sh` to include it.")
endif()

set_target_properties(CAFAnaSysts PROPERTIES
  PUBLIC_HEADER "${Systs_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaSysts BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaSysts
  EXPORT CAFAnaTargets
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

add_cafana_lib(CAFAnaSysts)