set(scripts_to_install
  tarballPRISM.sh
  FarmBuildPRISMInterps.sh
  BuildPRISMInterps.sh
  FarmCAFPRISMNodeScript.sh
  CAFPRISMNodeScript.sh)

install(PROGRAMS ${scripts_to_install} DESTINATION scripts/FermiGridScripts)
