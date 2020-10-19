##### StandardRecord

set(StandardRecord_implementation_files
  ${SRC_ROOT_PARENT}/StandardRecord/StandardRecord.cxx
)

set(StandardRecord_header_files
    ${SRC_ROOT_PARENT}/StandardRecord/StandardRecord.h
    ${SRC_ROOT_PARENT}/StandardRecord/SRProxy.h
)

add_library(StandardRecord SHARED ${StandardRecord_implementation_files})
target_link_libraries(StandardRecord)

install(TARGETS StandardRecord DESTINATION lib)

install(FILES ${StandardRecord_header_files} DESTINATION include/StandardRecord)

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

find_library(CAFANACOREEXT CAFAnaCoreExt $ENV{CAFANACORE_LIB})
