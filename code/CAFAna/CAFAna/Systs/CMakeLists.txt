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
  MissingProtonFakeData.h)

add_library(CAFAnaSysts SHARED ${Systs_implementation_files})
target_link_libraries(CAFAnaSysts)

install(TARGETS CAFAnaSysts DESTINATION lib)

install(FILES ${Systs_header_files} DESTINATION include/CAFAna/Systs)

set(SYST_INPUT_ROOT_FILES
  ProtonEdepm20pc_binnedWeights_nu.root
  ProtonEdepm20pc_binnedWeights_nubar.root
  flux_shifts.root
  flux_shifts_wOffAxis.root
  xsec_shifts.root
  modelComp.root
  ND_eff_syst.root)

install(FILES ${SYST_INPUT_ROOT_FILES} DESTINATION Systs)
