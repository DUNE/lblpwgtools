set(Prediction_implementation_files
  IPrediction.cxx
  PredictionExtrap.cxx
  PredictionGenerator.cxx
  PredictionInterp.cxx
  PredictionNoExtrap.cxx
  PredictionNoOsc.cxx
  PredictionScaleComp.cxx
  PredictionXSecDiag.cxx
  PredictionNuOnE.cxx)

set(Prediction_header_files
  IPrediction.h
  PredictionExtrap.h
  PredictionGenerator.h
  PredictionInterp.h
  PredictionNoExtrap.h
  PredictionNoOsc.h
  PredictionScaleComp.h
  PredictionXSecDiag.h
  PredictionNuOnE.h)

if(DEFINED USE_EIGEN AND USE_EIGEN)
  LIST(APPEND Prediction_implementation_files PredictionPRISM.cxx)
  LIST(APPEND Prediction_header_files PredictionPRISM.h)
endif()

add_library(CAFAnaPrediction SHARED ${Prediction_implementation_files})
target_link_libraries(CAFAnaPrediction)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaPrediction gperftools)
  target_link_libraries(CAFAnaPrediction ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaPrediction DESTINATION lib)

install(FILES ${Prediction_header_files} DESTINATION include/CAFAna/Prediction)
