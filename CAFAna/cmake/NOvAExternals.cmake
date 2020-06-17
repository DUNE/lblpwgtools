##### StandardRecord

set(StandardRecord_implementation_files
  ${SRC_ROOT_PARENT}/StandardRecord/StandardRecord.cxx
)

set(StandardRecord_header_files
    ${SRC_ROOT_PARENT}/StandardRecord/SRDune.h
    ${SRC_ROOT_PARENT}/StandardRecord/StandardRecord.h
)

add_library(StandardRecord SHARED ${StandardRecord_implementation_files})
target_link_libraries(StandardRecord)

install(TARGETS StandardRecord DESTINATION lib)

install(FILES ${StandardRecord_header_files} DESTINATION include/StandardRecord)

##### OscLib

set(OscLib_implementation_files
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/EarthModel.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/IOscCalculator.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculator.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorCPT.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorGeneral.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNSOpt.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorDMP.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS_CPT.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS_NSI.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorSterile.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorSterileBeam.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNSOpt.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS_DMP.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS_NSI.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS_Sterile.cxx
)

set(OscLib_header_files
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/EarthModel.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/IOscCalculator.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculator.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorCPT.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorGeneral.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNSOpt.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorDMP.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS_CPT.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS_NSI.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorSterile.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorSterileBeam.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscParameters.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNSOpt.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS_NSI.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS_DMP.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS_Sterile.h
)

add_library(OscLibFunc SHARED ${OscLib_implementation_files})
target_compile_options(OscLibFunc PUBLIC ${GSL_INC_STR} ${EIGEN_INC_STR} ${STAN_INC_STR} ${STAN_MATH_INC_STR})

CatStringsIfNotEmpty(GSL_LINKER_FLAGS ${GSL_LIB_DIR_STR} ${GSL_LIBS_STR})
target_link_libraries(OscLibFunc ${GSL_LINKER_FLAGS})

install(TARGETS OscLibFunc DESTINATION lib)

install(FILES ${OscLib_header_files} DESTINATION include/OscLib/func)

##### Utilities

set(Utilities_implementation_files
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/MathUtil.cxx
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/NuMITimeParams.cxx
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/RandHisto.cxx
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/EnvExpand.cxx
)

set(Utilities_header_files
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/MathUtil.h
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/NuMITimeParams.h
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/RandHisto.h
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/ROOTHelpers.h
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/EnvExpand.h
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/Stan.h
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/StanUtils.h
  ${CMAKE_BINARY_DIR}/Ext/Utilities/func/StanVar.h
)

add_library(UtilitiesFunc SHARED ${Utilities_implementation_files})
target_compile_options(UtilitiesFunc PUBLIC -I${CLHEP_PREFIX}/include ${EIGEN_INC_STR} ${STAN_INC_STR} ${STAN_MATH_INC_STR})

target_link_libraries(UtilitiesFunc -L${CLHEP_PREFIX}/lib -lCLHEP)

install(TARGETS UtilitiesFunc DESTINATION lib)

install(FILES ${Utilities_header_files} DESTINATION include/Utilities/func)

install(FILES ${CMAKE_BINARY_DIR}/Ext/Utilities/rootlogon.C DESTINATION include/Utilities)
