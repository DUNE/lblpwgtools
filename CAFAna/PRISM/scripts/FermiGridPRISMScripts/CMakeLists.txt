set(scripts_to_install
  tarballPRISM.sh
  #WrapperFarmBuildPredInterps.sh
  FarmBuildPRISMInterps.sh
  BuildPRISMInterps.sh
  FarmCAFPRISMNodeScript.sh
  CAFPRISMNodeScript.sh)

install(PROGRAMS ${scripts_to_install} DESTINATION scripts/FermiGridScripts)
