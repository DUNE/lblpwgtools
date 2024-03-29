LIST(APPEND PRISM_implementation_files
  PRISMReweightableSpectrum.cxx
  PredictionsForPRISM.cxx
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
  PRISMReweightableSpectrum.h
  PredictionsForPRISM.h
  PRISMExtrapolator.h
  PredictionPRISM.h
  PRISMUtils.h
  PRISMAnalysisDefinitions.h
  EigenUtils.h
  PRISMDetectorExtrapolation.h
  PRISMMCEffCorrection.h
  RunPlan.h
  SimpleChi2Experiment.h
  Axes.h
  Cuts.h
  Weights.h)

add_library(CAFAnaPRISM SHARED ${PRISM_implementation_files})

include_directories($ENV{SRPROXY_INC})

target_link_libraries(CAFAnaPRISM CAFAnaCore CAFAnaCore::Ext Stan::All)

if(DEFINED ENV{FHICLCPP_FQ_DIR})
  # fhiclcpp from ups
  target_link_libraries(CAFAnaPRISM fhiclcpp::fhiclcpp)
else()
  # need different target name for fhiclcpp from support/
  target_link_libraries(CAFAnaPRISM fhiclcpp::Headers)
endif()

set_target_properties(CAFAnaPRISM PROPERTIES
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
