set(Experiment_implementation_files
  BinnedLkhdExperiment.cxx
  CountingExperiment.cxx
  IChiSqExperiment.cxx
  IExperiment.cxx
  ILkhdExperiment.cxx
  MultiExperiment.cxx
  ReactorExperiment.cxx
  SingleSampleExperiment.cxx
  SolarConstraints.cxx)

set(Experiment_header_files
  BinnedLkhdExperiment.h
  CountingExperiment.h
  IChiSqExperiment.h
  IExperiment.h
  ILkhdExperiment.h
  MultiExperiment.h
  ReactorExperiment.h
  SingleSampleExperiment.h
  SolarConstraints.h)

add_library(CAFAnaExperiment SHARED ${Experiment_implementation_files})
target_link_libraries(CAFAnaExperiment)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaExperiment gperftools)
  target_link_libraries(CAFAnaExperiment ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaExperiment DESTINATION lib)

install(FILES ${Experiment_header_files} DESTINATION include/CAFAna/Experiment)
