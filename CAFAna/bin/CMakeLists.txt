set(APPS
  hadd_cafana
  diff_spectra
)

foreach(APP ${APPS})
  install(FILES ${APP}.cc DESTINATION scripts)

  add_executable(${APP} ${APP}.cc)

  target_link_libraries(${APP} CAFAnaCore)

  install(TARGETS ${APP} DESTINATION bin)
endforeach()
