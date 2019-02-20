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

###############################  GSL  ######################################
if(NOT DEFINED GSL_LIB OR GSL_LIB STREQUAL "")
  GETLIBDIR(gsl-config --libs GSL_LIB)
  if(GSL_LIB STREQUAL "")
    message(FATAL_ERROR "Variable GSL_LIB is not defined and could not be found with gsl-config. The location of a pre-built gsl install must be defined either as $ cmake -DGSL_LIB=/path/to/GSL_libraries")
  endif()
endif()

if(NOT DEFINED GSL_INC OR GSL_INC STREQUAL "")
  GETINCDIR(gsl-config --cflags GSL_INC)
  if(GSL_INC STREQUAL "")
    message(FATAL_ERROR "Variable GSL_INC is not defined and could not be found with gsl-config. The location of a pre-built gsl install must be defined either as $ cmake -DGSL_INC=/path/to/GSL_includes")
  endif()
endif()

if(NOT DEFINED GSL_LIB_LIST OR GSL_LIB_LIST STREQUAL "")
  GETLIBS(gsl-config --libs GSL_LIB_LIST)
  if(GSL_LIB_LIST STREQUAL "")
    message(FATAL_ERROR "Variable GSL_LIB_LIST is not defined and could not be found with gsl-config. The location of a pre-built gsl install must be defined either as $ cmake -DGSL_LIB_LIST=liba;libb")
  endif()
endif()

PrefixList(GSL_INC_STR "-I" ${GSL_INC})
PrefixList(GSL_LIB_DIR_STR "-L" ${GSL_LIB})
PrefixList(GSL_LIBS_STR "-l" ${GSL_LIB_LIST})

################################################################################


##### OscLib

set(OscLib_implementation_files
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/EarthModel.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/IOscCalculator.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculator.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorCPT.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorGeneral.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNSOpt.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS_CPT.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS_NSI.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorSterile.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorSterileBeam.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS.cxx
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNSOpt.cxx
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
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS_CPT.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorPMNS_NSI.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorSterile.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/OscCalculatorSterileBeam.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNSOpt.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS_NSI.h
  ${CMAKE_BINARY_DIR}/Ext/OscLib/func/PMNS_Sterile.h
)

add_library(OscLib SHARED ${OscLib_implementation_files})
target_compile_options(OscLib PUBLIC ${GSL_INC_STR})

CatStringsIfNotEmpty(GSL_LINKER_FLAGS ${GSL_LIB_DIR_STR} ${GSL_LIBS_STR})
target_link_libraries(OscLib ${GSL_LINKER_FLAGS})

install(TARGETS OscLib DESTINATION lib)

install(FILES ${OscLib_header_files} DESTINATION include/OscLib/func)

##### Utilities

###############################  GSL  ######################################
if(NOT DEFINED CLHEP_PREFIX OR CLHEP_PREFIX STREQUAL "")

  SET(CLHEP_CONFIG "CLHEP_CONFIG-NOTFOUND")
  find_program(CLHEP_CONFIG clhep-config)
  if(NOT CLHEP_CONFIG STREQUAL "CONFIGAPP_LOCATION-NOTFOUND")
    execute_process (COMMAND ${CLHEP_CONFIG}
    --prefix OUTPUT_VARIABLE CLHEP_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
  else()
  endif()

  if(CLHEP_PREFIX STREQUAL "")
    message(FATAL_ERROR "Variable CLHEP_PREFIX is not defined and could not be found with clhep-config. The location of a pre-built gsl install must be defined either as $ cmake -DCLHEP_PREFIX=/path/to/CLHEP_PREFIX")
  endif()

  string(REPLACE "\"" "" CLHEP_PREFIX ${CLHEP_PREFIX})

endif()

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
)

add_library(Utilities SHARED ${Utilities_implementation_files})
target_compile_options(Utilities PUBLIC -I${CLHEP_PREFIX}/include)

target_link_libraries(Utilities -L${CLHEP_PREFIX}/lib -lCLHEP)

install(TARGETS Utilities DESTINATION lib)

install(FILES ${Utilities_header_files} DESTINATION include/Utilities/func)

install(FILES ${CMAKE_BINARY_DIR}/Ext/Utilities/rootlogon.C DESTINATION include/Utilities)
