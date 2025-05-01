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
  RefineSeeds.cxx
  SeedList.cxx
)
if(CAFANA_USE_STAN)
  set(Fit_implementation_files ${Fit_implementation_files}
      Priors.cxx
      StanFitter.cxx
  )
endif()

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
  RefineSeeds.h
  SeedList.h
  StanConfig.h
)
if(CAFANA_USE_STAN)
  set(Fit_header_files ${Fit_header_files}
    StanFitter.h
    Priors.h
  )
endif()

add_library(CAFAnaFit SHARED ${Fit_implementation_files})
target_link_libraries(CAFAnaFit CAFAnaCore OscLib::All)

set_target_properties(CAFAnaFit PROPERTIES
  PUBLIC_HEADER "${Fit_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaFit BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaFit
  EXPORT CAFAnaTargets
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Fit/)

add_cafana_lib(CAFAnaFit)