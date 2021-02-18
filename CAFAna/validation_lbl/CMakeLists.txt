set(scripts_to_build
    validate_lbl_fit)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_library(OSCLIB OscLib $ENV{OSCLIB_LIB})

foreach(SCRPT ${scripts_to_build})
  install(FILES ${SCRPT}.C DESTINATION validation_lbl_fit)

  add_executable(${SCRPT} ${SCRPT}.C)

  target_link_libraries(${SCRPT}
    CAFAnaAnalysis
    CAFAnaDecomp
    CAFAnaFit
    CAFAnaPrediction
    CAFAnaCore
    CAFAnaExperiment
    CAFAnaSysts
    CAFAnaCuts
    CAFAnaExtrap
    CAFAnaVars
    StandardRecord
    ${OSCLIB}
    )

    target_link_libraries(${SCRPT} ${CAFANA_LINK_DIRS})
    target_link_libraries(${SCRPT} ${CAFANA_DEPEND_LIBS})

  install(TARGETS ${SCRPT} DESTINATION bin)
endforeach()