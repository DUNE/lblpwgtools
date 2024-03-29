set(scripts_to_build
  MakePRISMPredInterps
  #OffAxisEventRates
  #OffAxisNDCAFCombiner
  PRISMPrediction
  PRISM_4Flavour_dChi2Scan
  PRISMSystThrows
  #PRISMDChi2Scan
  #DumpEventRateTable
  #PRISMRegOptimizer
  #PRISMExtrapRegOptimizer
 #FluxPredictionTester
 #EventLoop
)

foreach(SCRPT ${scripts_to_build})
  install(FILES ${SCRPT}.C DESTINATION scripts)

  add_executable(${SCRPT} ${SCRPT}.C)

  target_link_libraries(${SCRPT} CAFAnaCore CAFAnaPRISM CAFAnaCuts CAFAnaVars CAFAnaPrediction CAFAnaSysts CAFAnaAnalysis ${STANDARDRECORD} ${StandardRecordProxy})

  if(DEFINED ENV{FHICLCPP_FQ_DIR})
    # fhiclcpp from ups
    target_link_libraries(${SCRPT} fhiclcpp::fhiclcpp)
  else()
    # need different target name for fhiclcpp from support/
    target_link_libraries(${SCRPT} fhiclcpp::Headers)
  endif()

  if(DEFINED USE_OPENMP AND USE_OPENMP)
    target_compile_options(${SCRPT} BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
  endif()

  install(TARGETS ${SCRPT} DESTINATION bin)
endforeach()
