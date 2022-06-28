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
target_link_libraries(CAFAnaFit CAFAnaCore)

set_target_properties(StandardRecord PROPERTIES
  PUBLIC_HEADER "${Fit_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaFit BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaFit 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Fit/)