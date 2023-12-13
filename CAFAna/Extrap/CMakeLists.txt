set(Extrap_implementation_files
  IExtrap.cxx
  TrivialExtrap.cxx)

set(Extrap_header_files
  IExtrap.h
  TrivialExtrap.h)

add_library(CAFAnaExtrap SHARED ${Extrap_implementation_files})
target_link_libraries(CAFAnaExtrap CAFAnaCore)

set_target_properties(CAFAnaExtrap PROPERTIES
  PUBLIC_HEADER "${Extrap_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaExtrap BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaExtrap 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Extrap/)
