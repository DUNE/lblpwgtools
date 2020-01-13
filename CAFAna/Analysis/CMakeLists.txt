set(Analysis_implementation_files
  Calcs.cxx
  CalcsNuFit.cxx
  Plots.cxx
  Resolution.cxx
  TDRLoaders.cxx
  AnalysisBinnings.cxx
  AnalysisVars.cxx
  common_fit_definitions.cxx)

set(Analysis_header_files
  Calcs.h
  CalcsNuFit.h
  Plots.h
  Resolution.h
  TDRLoaders.h
  Exposures.h
  Style.h
  AnalysisBinnings.h
  AnalysisDialGroups.h
  AnalysisVars.h
  common_fit_definitions.h
  AnalysisVersion.h
  ExtraHeaders.h
  CheckPointHelper.h)

add_library(CAFAnaAnalysis SHARED ${Analysis_implementation_files})
target_link_libraries(CAFAnaAnalysis)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaAnalysis gperftools)
  target_link_libraries(CAFAnaAnalysis ${GPERF_LINKER_FLAGS})
endif()

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaAnalysis BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaAnalysis DESTINATION lib)

install(FILES ${Analysis_header_files} DESTINATION include/CAFAna/Analysis)
