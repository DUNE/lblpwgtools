set(Experiment_implementation_files
        CountingExperiment.cxx
        CovarianceExperiment.cxx
        ICovarianceMatrix.cxx
        CovMxChiSq.cxx
        CovMxChiSqPreInvert.cxx
        CovMxLL.cxx
        IExperiment.cxx
        MultiExperiment.cxx
        ReactorExperiment.cxx
        SingleSampleExperiment.cxx
        SolarConstraints.cxx)

set(Experiment_header_files
        CountingExperiment.h
        CovarianceExperiment.h
        ICovarianceMatrix.h
        CovMxChiSq.h
        CovMxChiSqPreInvert.h
        CovMxLL.h
        IExperiment.h
        MultiExperiment.h
        ReactorExperiment.h
        SingleSampleExperiment.h
        SolarConstraints.h)

add_library(CAFAnaExperiment SHARED ${Experiment_implementation_files})
target_link_libraries(CAFAnaExperiment CAFAnaPrediction CAFAnaVars)

set_target_properties(CAFAnaExperiment PROPERTIES
  PUBLIC_HEADER "${Experiment_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaExperiment BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaExperiment 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Experiment/)
