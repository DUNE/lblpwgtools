set(GArTKI_implementation_files
  GArTKIBase.cxx
  GArTKIuBooNE.cxx
  GArTKINp0pi.cxx)

set(GArTKI_header_files
  GArTKIBase.h
  GArTKIuBooNE.h
  GArTKINp0pi.h)

add_library(CAFAnaGArTKI SHARED ${GArTKI_implementation_files})
target_link_libraries(CAFAnaGArTKI
  CAFAnaGAr
  CAFAnaCore)

set_target_properties(CAFAnaGArTKI PROPERTIES
  PUBLIC_HEADER "${GArTKI_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaGArTKI BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaGArTKI 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/GArTKI/)
