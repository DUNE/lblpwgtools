set(Analysis_implementation_files
  Calcs.cxx
  CalcsNuFit.cxx
  Plots.cxx
  Resolution.cxx)

set(Analysis_header_files
  Calcs.h
  CalcsNuFit.h
  Plots.h
  Resolution.h
  Exposures.h
  Style.h
  ExtraHeaders.h
  CheckPointHelper.h)

add_library(CAFAnaAnalysis SHARED ${Analysis_implementation_files})
target_link_libraries(CAFAnaAnalysis CAFAnaCore CAFAnaFit)

set_target_properties(CAFAnaAnalysis PROPERTIES
  PUBLIC_HEADER "${Analysis_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaAnalysis BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaAnalysis
  EXPORT CAFAnaTargets
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Analysis/)

add_cafana_lib(CAFAnaAnalysis)