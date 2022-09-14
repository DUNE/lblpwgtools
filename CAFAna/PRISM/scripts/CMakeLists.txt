set(APPS
  hadd_state
  hadd_fits
  OffAxisPOTCalc
  produce_flux_syst_inputs
)

foreach(APP ${APPS})
  install(FILES ${APP}.C DESTINATION scripts)

  add_executable(${APP} ${APP}.C)

  target_link_libraries(${APP} ${CAFAna_Libs})

  install (TARGETS ${APP} DESTINATION bin)
endforeach()

add_subdirectory(FermiGridPRISMScripts)
