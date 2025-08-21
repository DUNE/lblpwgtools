set(Decomp_implementation_files
  IDecomp.cxx
  ProportionalDecomp.cxx
  )

set(Decomp_header_files
  IDecomp.h
  ProportionalDecomp.h)

add_library(CAFAnaDecomp SHARED ${Decomp_implementation_files})
target_link_libraries(CAFAnaDecomp CAFAnaCore)

set_target_properties(CAFAnaDecomp PROPERTIES
  PUBLIC_HEADER "${Extrap_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaDecomp BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaDecomp
  EXPORT CAFAnaTargets
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Decomp/)

add_cafana_lib(CAFAnaDecomp)
