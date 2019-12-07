set(Decomp_implementation_files
  IDecomp.cxx
  NumuDecomp.cxx)

set(Decomp_header_files
  IDecomp.h
  NumuDecomp.h)

add_library(CAFAnaDecomp SHARED ${Decomp_implementation_files})
target_link_libraries(CAFAnaDecomp)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaDecomp gperftools)
  target_link_libraries(CAFAnaDecomp ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaDecomp DESTINATION lib)

install(FILES ${Decomp_header_files} DESTINATION include/CAFAna/Decomp)
