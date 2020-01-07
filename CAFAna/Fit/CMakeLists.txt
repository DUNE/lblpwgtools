set(Fit_implementation_files
  Fit.cxx
  FrequentistSurface.cxx
  GradientDescent.cxx
  ISurface.cxx
  MCMCSamples.cxx
  Priors.cxx
  RefineSeeds.cxx
  SeedList.cxx
  StanFitter.cxx)

set(Fit_header_files
  Fit.h
  FrequentistSurface.h
  GradientDescent.h
  ISurface.h
  MCMCSamples.h
  Priors.h
  RefineSeeds.h
  SeedList.h
  StanConfig.h
  StanFitter.h)

add_library(CAFAnaFit SHARED ${Fit_implementation_files})
target_link_libraries(CAFAnaFit)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaFit gperftools)
  target_link_libraries(CAFAnaFit ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaFit DESTINATION lib)

install(FILES ${Fit_header_files} DESTINATION include/CAFAna/Fit)
