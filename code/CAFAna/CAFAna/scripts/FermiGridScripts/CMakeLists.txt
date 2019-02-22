set(scripts_to_install
  tarball.sh
  BuildInterps.sh
  FarmBuildInterps.sh
  FarmCAFENodeScript.sh)

install(PROGRAMS ${scripts_to_install} DESTINATION scripts/FermiGridScripts)
