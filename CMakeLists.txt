include_directories(${CMAKE_CURRENT_SOURCE_DIR})

LIST(APPEND PRISM_implementation_files
  PRISMExtrapolator.cxx
  PredictionPRISM.cxx
  PRISMUtils.cxx
  PRISMAnalysisDefinitions.cxx
  EigenUtils.cxx)
LIST(APPEND PRISM_header_files
  PRISMExtrapolator.h
  PredictionPRISM.h
  PRISMUtils.h
  PRISMAnalysisDefinitions.h
  EigenUtils.h)

add_library(CAFAnaPRISM SHARED ${PRISM_implementation_files})
target_link_libraries(CAFAnaPRISM)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaPRISM gperftools)
  target_link_libraries(CAFAnaPRISM ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaPRISM DESTINATION lib)

install(FILES ${PRISM_header_files} DESTINATION include/CAFAna/PRISM)

add_subdirectory(app)
add_subdirectory(flux)
