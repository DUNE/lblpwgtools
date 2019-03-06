set(Analysis_implementation_files
  Calcs.cxx
  CalcsNuFit.cxx
  Fit.cxx
  GradientDescent.cxx
  Plots.cxx
  Resolution.cxx
  Surface.cxx
  TDRLoaders.cxx
  PRISMFluxMatcher.cxx)

set(Analysis_header_files
  Calcs.h
  CalcsNuFit.h
  Fit.h
  GradientDescent.h
  Plots.h
  Resolution.h
  Surface.h
  TDRLoaders.h
  Exposures.h
  Style.h
  PRISMFluxMatcher.h)

add_library(CAFAnaAnalysis SHARED ${Analysis_implementation_files})
target_link_libraries(CAFAnaAnalysis)

install(TARGETS CAFAnaAnalysis DESTINATION lib)

install(FILES ${Analysis_header_files} DESTINATION include/CAFAna/Analysis)
