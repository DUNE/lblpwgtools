set(Vars_implementation_files
  DUNENeutNuWROReweight.cxx
  FitVars.cxx)

set(Vars_header_files
  DUNENeutNuWROReweight.h
  FitVars.h)

add_library(CAFAnaVars SHARED ${Vars_implementation_files})
target_link_libraries(CAFAnaVars)

install(TARGETS CAFAnaVars DESTINATION lib)

install(FILES ${Vars_header_files} DESTINATION include/CAFAna/Vars)
