set(Decomp_implementation_files
  IDecomp.cxx
  NumuDecomp.cxx)

set(Decomp_header_files
  IDecomp.h
  NumuDecomp.h)

add_library(CAFAnaDecomp SHARED ${Decomp_implementation_files})
target_link_libraries(CAFAnaDecomp CAFAnaCore)

set_target_properties(StandardRecord PROPERTIES
  PUBLIC_HEADER "${Decomp_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaDecomp BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaDecomp 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Decomp/)
