set(Vars_implementation_files
  DUNENeutNuWROReweight.cxx
  FitVars.cxx)

set(Vars_header_files
  DUNENeutNuWROReweight.h
  FitVars.h)

add_library(CAFAnaVars SHARED ${Vars_implementation_files})
target_link_libraries(CAFAnaVars)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaVars gperftools)
  target_link_libraries(CAFAnaVars ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaVars DESTINATION lib)

install(FILES ${Vars_header_files} DESTINATION include/CAFAna/Vars)
