set(scripts_to_install
  bin_splines.C
  cpv_joint.C
  fit_3flavor_MCMC.C
  make_all_throws.C
  make_cpv_throws.C
  make_octant_throws.C
  make_mh_throws.C
  mh_joint.C
  octant_joint.C
  spec_variations.C
  )

# 2025-02-25 JW: the scripts are left as legacy examples as to what one might want to do, but they won't build any more
set(scripts_to_build
  )
if(DEFINED USE_OPENMP AND USE_OPENMP)
  LIST(APPEND scripts_to_build pred_thread_test fit_thread_test)
endif()

install(FILES ${scripts_to_install} DESTINATION scripts)

find_library(OSCLIB OscLib $ENV{OSCLIB_LIB})

foreach(SCRPT ${scripts_to_build})
  install(FILES ${SCRPT}.C DESTINATION scripts)

  add_executable(${SCRPT} ${SCRPT}.C)

  target_link_libraries(${SCRPT} CAFAnaCore CAFAnaCuts CAFAnaVars CAFAnaSysts CAFAnaAnalysis ${STANDARDRECORD} ${STANDARDRECORDPROXY})

  if(DEFINED USE_OPENMP AND USE_OPENMP)
    target_compile_options(${SCRPT} BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
  endif()

  install(TARGETS ${SCRPT} DESTINATION bin)
endforeach()

add_subdirectory(FermiGridScripts)
