set(scripts_to_build
    MakePRISMPredInterps
    OffAxisEventRates
    OffAxisNDCAFCombiner
    #PRISMPrediction
)

foreach(SCRPT ${scripts_to_build})
  install(FILES ${SCRPT}.C DESTINATION scripts)

  add_executable(${SCRPT} ${SCRPT}.C)

  target_link_libraries(${SCRPT}
    CAFAnaAnalysis
    CAFAnaDecomp
    CAFAnaPrediction
    CAFAnaCore
    CAFAnaExperiment
    CAFAnaSysts
    CAFAnaCuts
    CAFAnaExtrap
    CAFAnaVars
    CAFAnaPRISM
    StandardRecord
    OscLibFunc
    UtilitiesFunc)

    target_link_libraries(${SCRPT} ${CAFANA_LINK_DIRS})
    target_link_libraries(${SCRPT} ${CAFANA_DEPEND_LIBS})

  if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
    add_dependencies(${SCRPT} gperftools)
    target_link_libraries(${SCRPT} ${GPERF_LINKER_FLAGS})
  endif()

  if(DEFINED USE_OPENMP AND USE_OPENMP)
    target_compile_options(${SCRPT} BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
  endif()

  install(TARGETS ${SCRPT} DESTINATION bin)
endforeach()
