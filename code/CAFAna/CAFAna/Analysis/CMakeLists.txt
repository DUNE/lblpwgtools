set(Analysis_implementation_files
  Calcs.cxx
  CalcsNuFit.cxx
  Fit.cxx
  GradientDescent.cxx
  Plots.cxx
  Resolution.cxx
  Surface.cxx
  # TDRLoaders.cxx
  SeedList.cxx
  RefineSeeds.cxx
  AnalysisBinnings.cxx
  AnalysisVars.cxx
  common_fit_definitions.cxx
  AnalysisVersion.cxx
  XSecSystList.cxx)

set(Analysis_header_files
  Calcs.h
  CalcsNuFit.h
  Fit.h
  GradientDescent.h
  Plots.h
  Resolution.h
  Surface.h
  # TDRLoaders.h
  Exposures.h
  Style.h
  SeedList.h
  RefineSeeds.h
  AnalysisBinnings.h
  AnalysisDialGroups.h
  AnalysisVars.h
  common_fit_definitions.h
  AnalysisVersion.h
  ExtraHeaders.h
  CheckPointHelper.h
  XSecSystList.h)

add_library(CAFAnaAnalysis SHARED ${Analysis_implementation_files})
target_link_libraries(CAFAnaAnalysis)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaAnalysis gperftools)
  target_link_libraries(CAFAnaAnalysis ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaAnalysis DESTINATION lib)

install(FILES ${Analysis_header_files} DESTINATION include/CAFAna/Analysis)
