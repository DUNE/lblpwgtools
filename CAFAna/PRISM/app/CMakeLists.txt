message(STATUS "CAFAna_Libs = ${CAFAna_Libs}")
foreach(lib ${CAFAna_Libs})
  find_library(lib_path NAMES ${lib})
  if(lib_path)
    message(STATUS "Found library ${lib} at ${lib_path}")
  else()
    message(WARNING "Library ${lib} not found!")
  endif()
endforeach()

message(STATUS "USE_OPENMP is set to: ${USE_OPENMP}")


set(scripts_to_build
  MakePRISMPredInterps
  #OffAxisEventRates
  #OffAxisNDCAFCombiner
  PRISMPrediction
  PRISM_4Flavour_dChi2Scan
  #PRISMSystThrows
  #PRISMDChi2Scan
  #DumpEventRateTable
  #PRISMRegOptimizer
  #PRISMExtrapRegOptimizer
 #FluxPredictionTester
 #EventLoop
)

foreach(SCRPT ${scripts_to_build})
  install(FILES ${SCRPT}.C DESTINATION scripts)

  add_executable(${SCRPT} ${SCRPT}.C)

  target_link_libraries(${SCRPT} ${CAFAna_Libs})

  if(DEFINED USE_OPENMP AND USE_OPENMP)
    target_compile_options(${SCRPT} BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
  endif()

  install(TARGETS ${SCRPT} DESTINATION bin)
endforeach()

add_cafana_lib(${CAFAna_Libs})
