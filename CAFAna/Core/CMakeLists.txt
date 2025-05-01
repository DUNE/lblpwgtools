set(Core_implementation_files
  Binning.cxx
  Cut.cxx
  IFitVar.cxx
  IRecordSource.cxx
  ISyst.cxx
  Multiverse.cxx
  Loaders.cxx
  LoadFromFile.cxx
  OscCurve.cxx
  EnsembleRatio.cxx
  EnsembleSpectrum.cxx
  EnsembleReweightableSpectrum.cxx 
  OscillatableSpectrum.cxx
  ProfilerSupport.cxx
  Registry.cxx
  SpectrumLoader.cxx
  SpectrumLoaderBase.cxx
  StanUtils.cxx
  SystShifts.cxx
  TruthMatching.cxx
  Utilities.cxx
  Var.cxx
  )
if(CAFANA_USE_STAN)
  set(Core_implementation_files ${Core_implementation_files}

  )
endif()

set(Core_header_files
  Binning.h
  Cut.h
  HistAxis.h
  IFitVar.h
  IRecordSink.h
  IRecordSource.h
  ISyst.h
  FitMultiverse.h
  Multiverse.h
  Loaders.h
  LoadFromFile.h
  MathUtil.h
  MultiVar.h
  OscCalcFwdDeclare.h
  OscCurve.h
  EnsembleRatio.h
  EnsembleSpectrum.h
  EnsembleReweightableSpectrum.h
  OscillatableSpectrum.h
  Sources.h
  Sources.ixx
  SpectrumLoader.h
  SpectrumLoaderBase.h
  StanUtils.h
  SystShifts.h
  TruthMatching.h
  Utilities.h
  Var.h
  Weight.h
  ModeConversionUtilities.h
  rootlogon.C
)
if(CAFANA_USE_STAN)
  set(Core_header_files ${Core_header_files}
      StanTypedefs.h
      FitVarWithPrior.h
  )
endif()
add_library(CAFAnaCore SHARED ${Core_implementation_files})
target_link_libraries(CAFAnaCore
  ${STANDARDRECORDPROXY}
  CAFAnaCore::Ext 
  OscLib::All
  ROOT::ROOT 
  Boost::filesystem)
if(CAFANA_USE_STAN)
  target_link_libraries(CAFAnaCore Stan::All)
else()
  # normally Stan would also drag in Eigen, but we can't count on that if we're not using Stan
  target_link_libraries(CAFAnaCore Eigen3::Eigen)
endif()


set_target_properties(CAFAnaCore PROPERTIES
  PUBLIC_HEADER "${Core_header_files}")

if(TARGET perftools::All)
  target_link_libraries(CAFAnaCore perftools::All)
endif()

if(DEFINED USE_OPENMP AND USE_OPENMP)
  target_compile_options(CAFAnaCore BEFORE PUBLIC -DUSE_PREDINTERP_OMP -fopenmp)
endif()

install(TARGETS CAFAnaCore
  EXPORT CAFAnaTargets
  LIBRARY DESTINATION lib/
  PUBLIC_HEADER DESTINATION include/CAFAna/Core/)

add_cafana_lib(CAFAnaCore)