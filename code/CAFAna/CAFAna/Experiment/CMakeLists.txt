set(Experiment_implementation_files
  CountingExperiment.cxx
  IExperiment.cxx
  MultiExperiment.cxx
  ReactorExperiment.cxx
  SingleSampleExperiment.cxx
  SolarConstraints.cxx)

set(Experiment_header_files
  CountingExperiment.h
  IExperiment.h
  MultiExperiment.h
  ReactorExperiment.h
  SingleSampleExperiment.h
  SolarConstraints.h)

add_library(CAFAnaExperiment SHARED ${Experiment_implementation_files})
target_link_libraries(CAFAnaExperiment)

install(TARGETS CAFAnaExperiment DESTINATION lib)

install(FILES ${Experiment_header_files} DESTINATION include/CAFAna/Experiment)
