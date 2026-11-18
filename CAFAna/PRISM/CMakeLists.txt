################################################################################
# CAFAnaPRISM
#
# Layout:
#   include/PRISM/         public headers (included as "PRISM/<file>.h")
#   src/                   library implementation
#   app/                   executables (PRISMPrediction, MakePRISMPredInterps, ...)
#   scripts/               helper executables, macros and grid scripts
#   fcl/                   PRISM FHiCL configuration (installed to fcl/PRISM)
#   flux/, FileExposures/  input data files
################################################################################

set(PRISM_implementation_files
  src/PRISMReweightableSpectrum.cxx
  src/PredictionsForPRISM.cxx
  src/PRISMExtrapolator.cxx
  src/PredictionPRISM.cxx
  src/PRISMUtils.cxx
  src/PRISMAnalysisDefinitions.cxx
  src/EigenUtils.cxx
  src/PRISMDetectorExtrapolation.cxx
  src/PRISMMCEffCorrection.cxx
  src/RunPlan.cxx
  src/SimpleChi2Experiment.cxx
  src/Axes.cxx
  src/Cuts.cxx
  src/Weights.cxx)

set(PRISM_header_files
  include/PRISM/PRISMReweightableSpectrum.h
  include/PRISM/PredictionsForPRISM.h
  include/PRISM/PRISMExtrapolator.h
  include/PRISM/PredictionPRISM.h
  include/PRISM/PRISMUtils.h
  include/PRISM/PRISMAnalysisDefinitions.h
  include/PRISM/EigenUtils.h
  include/PRISM/PRISMDetectorExtrapolation.h
  include/PRISM/PRISMMCEffCorrection.h
  include/PRISM/RunPlan.h
  include/PRISM/SimpleChi2Experiment.h
  include/PRISM/Axes.h
  include/PRISM/Cuts.h
  include/PRISM/Weights.h)

add_library(CAFAnaPRISM SHARED ${PRISM_implementation_files} ${PRISM_header_files})

# Anything linking CAFAnaPRISM (the apps, scripts, ...) can then
# #include "PRISM/<file>.h" from either the source or install tree.
target_include_directories(CAFAnaPRISM PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if(DEFINED ENV{FHICLCPP_FQ_DIR})
  # fhiclcpp from ups
  target_link_libraries(CAFAnaPRISM CAFAnaCore fhiclcpp::fhiclcpp)
else()
  # need different target name for fhiclcpp from support/
  target_link_libraries(CAFAnaPRISM CAFAnaCore fhiclcpp::Headers)
endif()

set_target_properties(CAFAnaPRISM PROPERTIES
  PUBLIC_HEADER "${PRISM_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaPRISM BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaPRISM
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/PRISM/)

add_subdirectory(app)
add_subdirectory(scripts)
add_subdirectory(fcl)
add_subdirectory(flux)
add_subdirectory(FileExposures)

add_cafana_lib(CAFAnaPRISM)

if(DEFINED ENV{FHICLCPP_FQ_DIR})
  add_cafana_lib(fhiclcpp::fhiclcpp)
else()
  add_cafana_lib(fhiclcpp::Headers)
endif()
