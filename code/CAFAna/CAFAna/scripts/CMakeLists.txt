set(scripts_to_install
  common_fit_definitions.C
  fit_covar.C
  remake_inputs.C
  bin_splines.C
  cpv_joint.C
  llh_scans.C
  make_cpv_throws.C
  make_octant_throws.C
  make_mh_throws.C
  mh_joint.C
  octant_joint.C
  spec_joint.C
  spec_variations.C)

set(scripts_to_build
  asimov_joint
  make_toy_throws
  make_toy_throws_fixed_seed
)

install(FILES ${scripts_to_install} DESTINATION scripts)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

foreach(SCRPT ${scripts_to_build})
  install(FILES ${SCRPT}.C DESTINATION scripts)

  add_executable(${SCRPT} ${SCRPT}.C)

  target_link_libraries(${SCRPT}
    CAFAnaAnalysis
    CAFAnaDecomp
    CAFAnaPrediction
    CAFAnaCore
    CAFAnaExperiment
    CAFAnaSysts
    CAFAnaCuts
    CAFAnaExtrap
    CAFAnaVars
    StandardRecord
    OscLibFunc
    UtilitiesFunc)

  if(NOT "${EXTERNAL_LINK_DIRS}" STREQUAL "")
    BuildFlagString(EXTERNAL_LINK_DIRS_STR "-L" ${EXTERNAL_LINK_DIRS} ${ROOT_LIBDIR})
    set_target_properties(${SCRPT} PROPERTIES LINK_FLAGS ${EXTERNAL_LINK_DIRS_STR})
  endif()

  target_link_libraries(${SCRPT} ${ROOT_LIBS})

  BuildFlagString(EXTERNAL_LIBS_CAFBUILD "-l" Minuit2 ${ROOT_LIBS} CLHEP)

  target_link_libraries(${SCRPT} ${EXTERNAL_LIBS_CAFBUILD})

  install(TARGETS ${SCRPT} DESTINATION bin)
endforeach()

add_subdirectory(FermiGridScripts)
