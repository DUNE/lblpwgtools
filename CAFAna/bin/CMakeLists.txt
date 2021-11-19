include(${CMAKE_SOURCE_DIR}/cmake/ROOTSetup.cmake)

set(APPS
  #hadd_cafana
  #diff_spectra
)

foreach(APP ${APPS})
  install(FILES ${APP}.cc DESTINATION scripts)

  add_executable(${APP} ${APP}.cc)

#<<<<<<< HEAD
    target_link_libraries(${APP} ${CAFANALIBS})

    target_link_libraries(${APP} ${CAFANA_LINK_DIRS})
    target_link_libraries(${APP} ${CAFANA_DEPEND_LIBS})
#=======
  target_link_libraries(${APP} "-L ${ROOT_LIBDIR}" ${ROOT_LIBS})
#>>>>>>> origin

  install(TARGETS ${APP} DESTINATION bin)
endforeach()
