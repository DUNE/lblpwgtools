include(${CMAKE_SOURCE_DIR}/cmake/ROOTSetup.cmake)

set(APPS
  hadd_cafana
  diff_spectra
)

foreach(APP ${APPS})
  install(FILES ${APP}.cc DESTINATION scripts)

  add_executable(${APP} ${APP}.cc)

  target_link_libraries(${APP} "-L ${ROOT_LIBDIR}" ${ROOT_LIBS})

  install(TARGETS ${APP} DESTINATION bin)
endforeach()
