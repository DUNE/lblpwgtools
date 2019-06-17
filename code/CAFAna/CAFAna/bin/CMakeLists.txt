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

  if(NOT "${EXTERNAL_LINK_DIRS}" STREQUAL "")
    BuildFlagString(EXTERNAL_LINK_DIRS_STR "-L" ${EXTERNAL_LINK_DIRS} ${ROOT_LIBDIR})
    set_target_properties(${APP} PROPERTIES LINK_FLAGS ${EXTERNAL_LINK_DIRS_STR})
  endif()

  if(DEFINED USE_OPENMP AND USE_OPENMP)
    BuildFlagString(EXTERNAL_LIBS_CAFBUILD "-l" Minuit2 ${ROOT_LIBS} CLHEP gomp)
  else()
    BuildFlagString(EXTERNAL_LIBS_CAFBUILD "-l" Minuit2 ${ROOT_LIBS} CLHEP)
  endif()
  
  target_link_libraries(${APP} ${EXTERNAL_LIBS_CAFBUILD})

  install(TARGETS ${APP} DESTINATION bin)
endforeach()
