set(Fit_implementation_files
  Fit.cxx
  Surface.cxx
  GradientDescent.cxx
  Surface.cxx
  RefineSeeds.cxx
  SeedList.cxx)

set(Fit_header_files
  Fit.h
  Surface.h
  GradientDescent.h
  Surface.h
  RefineSeeds.h
  SeedList.h)

add_library(CAFAnaFit SHARED ${Fit_implementation_files})
target_link_libraries(CAFAnaFit)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaFit gperftools)
  target_link_libraries(CAFAnaFit ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaFit DESTINATION lib)

install(FILES ${Fit_header_files} DESTINATION include/CAFAna/Fit)
