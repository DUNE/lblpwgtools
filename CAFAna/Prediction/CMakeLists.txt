set(Prediction_implementation_files
  IPrediction.cxx
  PredictionExtrap.cxx
  PredictionGenerator.cxx
  PredictionInterp.cxx
  PredictionNoExtrap.cxx
  PredictionNoOsc.cxx
  PredictionScaleComp.cxx
  PredictionXSecDiag.cxx
  PredictionInterpKernel.cxx)
  #PredictionsForPRISM.cxx)

set(Prediction_header_files
  IPrediction.h
  PredictionExtrap.h
  PredictionGenerator.h
  PredictionInterp.h
  PredictionNoExtrap.h
  PredictionNoOsc.h
  PredictionScaleComp.h
  PredictionXSecDiag.h
  PredictionInterpKernel.h)
  #PredictionsForPRISM.h)

add_library(CAFAnaPrediction SHARED ${Prediction_implementation_files})
target_link_libraries(CAFAnaPrediction CAFAnaExtrap) # CAFAnaPRISM)

set_target_properties(CAFAnaPrediction PROPERTIES
  PUBLIC_HEADER "${Prediction_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaPrediction BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaPrediction 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Prediction/)
