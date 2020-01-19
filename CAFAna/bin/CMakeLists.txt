set(APPS
  hadd_cafana
)

foreach(APP ${APPS})
  install(FILES ${APP}.cc DESTINATION scripts)

  add_executable(${APP} ${APP}.cc)

    target_link_libraries(${APP} ${CAFANALIBS})

    target_link_libraries(${APP} ${CAFANA_LINK_DIRS})
    target_link_libraries(${APP} ${CAFANA_DEPEND_LIBS})

  install(TARGETS ${APP} DESTINATION bin)
endforeach()
