set(Vars_implementation_files
  DUNENeutNuWROReweight.cxx
  FitVars.cxx
  Vars.cxx)

set(Vars_header_files
  DUNENeutNuWROReweight.h
  FitVars.h
  Vars.h)

add_library(CAFAnaVars SHARED ${Vars_implementation_files})
target_link_libraries(CAFAnaVars CAFAnaCore)

set_target_properties(StandardRecord PROPERTIES
  PUBLIC_HEADER "${Vars_header_files}")

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaVars BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaVars 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Vars/)