set(scripts_to_install
  common_fit_definitions.C
  fit_covar.C
  remake_inputs.C)

install(FILES ${scripts_to_install} DESTINATION scripts)

add_subdirectory(FermiGridScripts)
