include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(APPS
  #hadd_state
  #hadd_fits
)

foreach(APP ${APPS})
  install(FILES ${APP}.C DESTINATION scipts)

  add_executable(${APP} ${APP}.C)

  target_link_libraries(${APP} ${CAFAna_Libs})

  install (TARGETS ${APP} DESTINATION bin)
endforeach()

add_subdirectory(FermiGridPRISMScripts)
