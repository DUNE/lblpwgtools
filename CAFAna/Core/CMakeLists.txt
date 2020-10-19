set(Core_implementation_files
  Binning.cxx
  IFitVar.cxx
  Instantiations.cxx
  ISyst.cxx
  Loaders.cxx
  LoadFromFile.cxx
  OscCurve.cxx
  OscillatableSpectrum.cxx
  ProfilerSupport.cxx
  Registry.cxx
  SpectrumLoader.cxx
  SpectrumLoaderBase.cxx
  StanUtils.cxx
  SystShifts.cxx
  Utilities.cxx
  )

set(Core_header_files
  Binning.h
  FitVarWithPrior.h
  IFitVar.h
  ISyst.h
  Loaders.h
  LoadFromFile.h
  OscCurve.h
  OscillatableSpectrum.h
  Registry.h
  SpectrumLoader.h
  SpectrumLoaderBase.h
  StanTypedefs.h
  StanUtils.h
  SystShifts.h
  Utilities.h
  ModeConversionUtilities.h
  )

add_library(CAFAnaCore SHARED ${Core_implementation_files})
target_link_libraries(CAFAnaCore ${CAFANACOREEXT})

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaCore gperftools)
  target_link_libraries(CAFAnaCore ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaCore DESTINATION lib)

install(FILES ${Core_header_files} DESTINATION include/CAFAna/Core)
