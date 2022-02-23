include_directories(${CMAKE_CURRENT_SOURCE_DIR})

LIST(APPEND PRISM_implementation_files
  PRISMExtrapolator.cxx
  PredictionPRISM.cxx
  PRISMUtils.cxx
  PRISMAnalysisDefinitions.cxx
  EigenUtils.cxx
  PRISMDetectorExtrapolation.cxx
  PRISMMCEffCorrection.cxx
  RunPlan.cxx
  SimpleChi2Experiment.cxx
  Axes.cxx
  Cuts.cxx
  Weights.cxx)

LIST(APPEND PRISM_header_files
  PRISMExtrapolator.h
  PredictionPRISM.h
  PRISMUtils.h
  PRISMAnalysisDefinitions.h
  EigenUtils.h
  PRISMDetectorExtrapolation.h
  PRISMMCEffCorrection.h
  RunPlan.h
  Axes.h
  Cuts.h
  Weights.h
  SimpleChi2Experiment.h)

add_library(CAFAnaPRISM SHARED ${PRISM_implementation_files})
target_link_libraries(CAFAnaPRISM CAFAnaCore fhiclcpp::Headers)

set_target_properties(StandardRecord PROPERTIES
  PUBLIC_HEADER "${PRISM_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaPRISM BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaPRISM 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/PRISM/)


add_subdirectory(app)
add_subdirectory(flux)
add_subdirectory(FileExposures)
add_subdirectory(scripts)
