set(GAr_implementation_files
  GArParams.cxx
  GArUtils.cxx
  GArTruthCuts.cxx)

set(GAr_header_files
  GArParams.h
  GArUtils.h
  GArTruthCuts.h)

add_library(CAFAnaGAr SHARED ${GAr_implementation_files})
target_link_libraries(CAFAnaGAr
  CAFAnaCore)

set_target_properties(CAFAnaGAr PROPERTIES
  PUBLIC_HEADER "${GAr_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaGAr BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaGAr 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/GAr/)
