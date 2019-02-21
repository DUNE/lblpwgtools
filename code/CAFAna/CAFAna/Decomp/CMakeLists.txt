set(Decomp_implementation_files
  IDecomp.cxx
  NumuDecomp.cxx)

set(Decomp_header_files
  IDecomp.h
  NumuDecomp.h)

add_library(CAFAnaDecomp SHARED ${Decomp_implementation_files})
target_link_libraries(CAFAnaDecomp)

install(TARGETS CAFAnaDecomp DESTINATION lib)

install(FILES ${Decomp_header_files} DESTINATION include/CAFAna/Decomp)
