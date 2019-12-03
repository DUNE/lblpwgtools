set(APPS
  hadd_cafana
)

foreach(APP ${APPS})
  install(FILES ${APP}.cc DESTINATION scripts)

  add_executable(${APP} ${APP}.cc)

  target_link_libraries(${APP}
    CAFAnaAnalysis
    CAFAnaDecomp
    CAFAnaPrediction
    CAFAnaCore
    CAFAnaExperiment
    CAFAnaSysts
    CAFAnaCuts
    CAFAnaExtrap
    CAFAnaVars
    StandardRecord
    OscLibFunc
    UtilitiesFunc)

    target_link_libraries(${APP} ${CAFANA_LINK_DIRS})
    target_link_libraries(${APP} ${CAFANA_DEPEND_LIBS})

  install(TARGETS ${APP} DESTINATION bin)
endforeach()
