if(DEFINED USE_EIGEN AND USE_EIGEN)

  LIST(APPEND PRISM_implementation_files PRISMExtrapolator.cxx PredictionPRISM.cxx)
  LIST(APPEND PRISM_header_files PRISMExtrapolator.h PredictionPRISM.h PRISMUtils.h)

  add_library(CAFAnaPRISM SHARED ${PRISM_implementation_files})
  target_link_libraries(CAFAnaPRISM)

  if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
    add_dependencies(CAFAnaPRISM gperftools)
    target_link_libraries(CAFAnaPRISM ${GPERF_LINKER_FLAGS})
  endif()

  install(TARGETS CAFAnaPRISM DESTINATION lib)

  install(FILES ${PRISM_header_files} DESTINATION include/CAFAna/PRISM)

endif()
