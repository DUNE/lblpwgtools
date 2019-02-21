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

install(TARGETS CAFAnaExtrap DESTINATION lib)

install(FILES ${Extrap_header_files} DESTINATION include/CAFAna/Extrap)
