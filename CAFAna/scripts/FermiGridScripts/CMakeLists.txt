set(scripts_to_install
  tarball.sh
  FarmBuildInterps.sh
  BuildInterps.sh
  FarmCAFENodeScript.sh
  CAFENodeScript.sh)

install(PROGRAMS ${scripts_to_install} DESTINATION scripts/FermiGridScripts)
