set(Extrap_implementation_files
  IExtrap.cxx
  ModularExtrap.cxx
  ModularExtrapComponent.cxx
  ModularExtrapComponentPlot.cxx
  TrivialExtrap.cxx)

set(Extrap_header_files
  IExtrap.h
  ModularExtrap.h
  ModularExtrapComponent.h
  TrivialExtrap.h)

add_library(CAFAnaExtrap SHARED ${Extrap_implementation_files})
target_link_libraries(CAFAnaExtrap)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaExtrap gperftools)
  target_link_libraries(CAFAnaExtrap ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaExtrap DESTINATION lib)

install(FILES ${Extrap_header_files} DESTINATION include/CAFAna/Extrap)
