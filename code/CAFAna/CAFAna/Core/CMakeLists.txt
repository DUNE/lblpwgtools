set(Core_implementation_files
  Binning.cxx
  Cut.cxx
  FileListSource.cxx
  GenieWeightList.cxx
  HistAxis.cxx
  HistCache.cxx
  IFitVar.cxx
  ISyst.cxx
  Loaders.cxx
  LoadFromFile.cxx
  MultiVar.cxx
  OscCurve.cxx
  OscillatableSpectrum.cxx
  # ProfilerSupport.cxx
  Progress.cxx
  Ratio.cxx
  ReweightableSpectrum.cxx
  #Not using SAM
  # SAMProjectSource.cxx
  # SAMQuerySource.cxx
  Spectrum.cxx
  SpectrumLoader.cxx
  SpectrumLoaderBase.cxx
  SystRegistry.cxx
  SystShifts.cxx
  ThreadPool.cxx
  Utilities.cxx
  Var.cxx
  WildcardSource.cxx)

set(Core_header_files
  Binning.h
  Cut.h
  FileListSource.h
  GenieWeightList.h
  HistAxis.h
  HistCache.h
  IFitVar.h
  ISyst.h
  Loaders.h
  LoadFromFile.h
  MultiVar.h
  OscCurve.h
  OscillatableSpectrum.h
  Progress.h
  Ratio.h
  ReweightableSpectrum.h
  #Not using SAM
  # SAMProjectSource.h
  # SAMQuerySource.h
  Spectrum.h
  SpectrumLoader.h
  SpectrumLoaderBase.h
  SystRegistry.h
  SystShifts.h
  ThreadPool.h
  Utilities.h
  Var.h
  WildcardSource.h
  IFileSource.h
  ModeConversionUtilities.h)

add_library(CAFAnaCore SHARED ${Core_implementation_files})
target_link_libraries(CAFAnaCore)

if(DEFINED USE_GPERFTOOLS AND USE_GPERFTOOLS)
  add_dependencies(CAFAnaCore gperftools)
  target_link_libraries(CAFAnaCore ${GPERF_LINKER_FLAGS})
endif()

install(TARGETS CAFAnaCore DESTINATION lib)

install(FILES ${Core_header_files} DESTINATION include/CAFAna/Core)
