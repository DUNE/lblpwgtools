set(Fit_implementation_files
  Bayesian1DMarginal.cxx
  BayesianMarginal.cxx
  BayesianSurface.cxx
  Fit.cxx
  FrequentistSurface.cxx
  GradientDescent.cxx
  IFitter.cxx
  ISurface.cxx
  MCMCSample.cxx
  MCMCSamples.cxx
  MinuitFitter.cxx
  Priors.cxx
  RefineSeeds.cxx
  SeedList.cxx
  StanFitter.cxx)

set(Fit_header_files
  Bayesian1DMarginal.h
  BayesianMarginal.h
  BayesianSurface.h
  Fit.h
  FrequentistSurface.h
  GradientDescent.h
  IFitter.h
  ISurface.h
  MCMCSample.h
  MCMCSamples.h
  MinuitFitter.h
  Priors.h
  RefineSeeds.h
  SeedList.h
  StanConfig.h
  StanFitter.h)

add_library(CAFAnaFit SHARED ${Fit_implementation_files})
target_link_libraries(CAFAnaFit ${CAFANACOREEXT})

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaFit gperftools)
  target_link_libraries(CAFAnaFit ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaFit DESTINATION lib)

install(FILES ${Fit_header_files} DESTINATION include/CAFAna/Fit)
