set(scripts_to_install
  common_fit_definitions.C
  fit_covar.C
  remake_inputs.C
  asimov_joint.C
  bin_splines.C
  cpv_joint.C
  llh_scans.C
  make_cpv_throws.C
  make_toy_throws.C
  make_mh_throws.C	
  mh_joint.C
  spec_joint.C
  spec_variations.C)

install(FILES ${scripts_to_install} DESTINATION scripts)

add_subdirectory(FermiGridScripts)
