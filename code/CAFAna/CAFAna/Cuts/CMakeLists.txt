set(Cuts_implementation_files
  AnaCuts.cxx
  TruthCuts.cxx)

set(Cuts_header_files
  AnaCuts.h
  TruthCuts.h)

add_library(CAFAnaCuts SHARED ${Cuts_implementation_files})
target_link_libraries(CAFAnaCuts)

install(TARGETS CAFAnaCuts DESTINATION lib)

install(FILES ${Cuts_header_files} DESTINATION include/CAFAna/Cuts)
