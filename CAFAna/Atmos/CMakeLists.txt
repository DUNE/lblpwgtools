set(Atmos_implementation_files
  AtmosOscSpect.cxx
  AtmosOscillogram.cxx
  AtmosOscillogramLE.cxx
  PredictAtmos.cxx)

set(Atmos_header_files
  AtmosOscSpect.h
  AtmosOscillogram.h
  AtmosOscillogramLE.h
  PredictAtmos.h)

add_library(CAFAnaAtmos SHARED ${Atmos_implementation_files})
target_link_libraries(CAFAnaAtmos)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaAtmos gperftools)
  target_link_libraries(CAFAnaAtmos ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaAtmos DESTINATION lib)

install(FILES ${Atmos_header_files} DESTINATION include/CAFAna/Atmos)
