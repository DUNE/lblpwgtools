set(scripts_to_install
  RunTwoDetFit.C
  )

set(scripts_to_build
  )
if(DEFINED USE_OPENMP AND USE_OPENMP)
  LIST(APPEND scripts_to_build pred_thread_test fit_thread_test)
endif()

install(FILES ${scripts_to_install} DESTINATION TwoDetFit)

find_library(OSCLIB OscLib $ENV{OSCLIB_LIB} $ENV{OSCLIB_INC})

foreach(SCRPT ${scripts_to_build})
  install(FILES ${SCRPT}.C DESTINATION TwoDetFit)

  add_executable(${SCRPT} ${SCRPT}.C)

  target_link_libraries(All INTERFACE ${CAFANA_LIBS})

  if(DEFINED USE_OPENMP AND USE_OPENMP)
    target_compile_options(${SCRPT} BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
  endif()

  install(TARGETS ${SCRPT} DESTINATION bin)
endforeach()