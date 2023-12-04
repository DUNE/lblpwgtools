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
  Cut.h
  FitVarWithPrior.h
  HistAxis.h
  IFitVar.h
  ISyst.h
  Loaders.h
  LoadFromFile.h
  MathUtil.h
  MultiVar.h
  OscCalcFwdDeclare.h
  OscCurve.h
  OscillatableSpectrum.h
  SpectrumLoader.h
  SpectrumLoaderBase.h
  StanTypedefs.h
  StanUtils.h
  SystShifts.h
  Utilities.h
  Var.h
  Weight.h
  ModeConversionUtilities.h
  rootlogon.C)

add_library(CAFAnaCore SHARED ${Core_implementation_files})
target_link_libraries(CAFAnaCore 
  StandardRecord 
  CAFAnaCore::Ext 
  OscLib::All
  ROOT::ROOT 
  Stan::All 
  Boost::filesystem)

set_target_properties(CAFAnaCore PROPERTIES
  PUBLIC_HEADER "${Core_header_files}")

if(TARGET perftools::All)
  target_link_libraries(CAFAnaCore perftools::All)
endif()

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaCore BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaCore 
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Core/)
