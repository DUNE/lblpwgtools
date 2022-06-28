set(Cuts_implementation_files
  AnaCuts.cxx
  TruthCuts.cxx)

set(Cuts_header_files
  AnaCuts.h
  TruthCuts.h)

add_library(CAFAnaCuts SHARED ${Cuts_implementation_files})
target_link_libraries(CAFAnaCuts CAFAnaCore)

set_target_properties(StandardRecord PROPERTIES
  PUBLIC_HEADER "${Cuts_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaCuts BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaCuts 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Cuts/)
