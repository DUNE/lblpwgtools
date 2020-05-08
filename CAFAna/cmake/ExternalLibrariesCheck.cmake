
if(SRC_ROOT_PARENT STREQUAL "")
  cmessage(FATAL_ERROR "Must configure with hard-coded SRC_ROOT_PARENT directory that contains the directory CAFAna. (likely to be -DSRC_ROOT_PARENT=$(readlink -f ../../)")
elseif(NOT DEFINED SRC_ROOT_PARENT)
  cmessage(FATAL_ERROR "Must configure with hard-coded SRC_ROOT_PARENT directory that contains the directory CAFAna. (likely to be -DSRC_ROOT_PARENT=$(readlink -f ../../)")
endif()

if(NOT EXISTS ${SRC_ROOT_PARENT}/CAFAna)
  cmessage(FATAL_ERROR "Cannot resolve: SRC_ROOT_PARENT=\"${SRC_ROOT_PARENT}/CAFAna\" to a valid directory, please reconfigure with a valid path to the parent directory of CAFAna.")
endif()

if(NOT EXISTS ${SRC_ROOT_PARENT}/StandardRecord)
  cmessage(FATAL_ERROR "Cannot resolve: SRC_ROOT_PARENT=\"${SRC_ROOT_PARENT}/StandardRecord\" to a valid directory, please reconfigure with a valid path to the parent directory of StandardRecord.")
endif()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/Ext/OscLib)
  cmessage(FATAL_ERROR "Expected to be able to find OscLib in: \"${CMAKE_BINARY_DIR}/Ext/OscLib\", did you build with the helper script?")
endif()

if(NOT EXISTS ${CMAKE_BINARY_DIR}/Ext/Utilities)
  cmessage(FATAL_ERROR "Expected to be able to find Utilities in: \"${CMAKE_BINARY_DIR}/Ext/Utilities\", did you build with the helper script?")
endif()

###############################  GSL  ######################################
if(NOT DEFINED GSL_LIB OR "x${GSL_LIB}" STREQUAL "x")
  if(DEFINED ENV{GSL_LIB} AND NOT "x$ENV{GSL_LIB}" STREQUAL "x")
    SET(GSL_LIB $ENV{GSL_LIB})
  else()
    GETLIBDIR(gsl-config --libs GSL_LIB)
    if(GSL_LIB STREQUAL "")
      message(FATAL_ERROR "Variable GSL_LIB is not defined and could not be found with gsl-config. The location of a pre-built gsl install must be defined either as $ cmake -DGSL_LIB=/path/to/GSL_libraries")
    endif()
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

LIST(APPEND EXTRA_LINK_DIRS ${GSL_LIB})
LIST(APPEND EXTRA_LIBS ${GSL_LIB_LIST})

################################################################################

###############################  CLHEP  ######################################
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

LIST(APPEND EXTRA_LINK_DIRS ${CLHEP_PREFIX}/lib)
LIST(APPEND EXTRA_LIBS CLHEP)

###############################  BOOST  ######################################

if(${BOOST_INC} STREQUAL "")
  cmessage(FATAL_ERROR "Expected to be able to evaluate \${BOOST_INC}=\"${BOOST_INC}\" to an existing directory, is BOOST set up in the environment?")
elseif(NOT DEFINED BOOST_INC)
  cmessage(FATAL_ERROR "Expected to be able to evaluate \${BOOST_INC}=\"${BOOST_INC}\" to an existing directory, is BOOST set up in the environment?")
endif()

if(${BOOST_LIB} STREQUAL "")
  cmessage(FATAL_ERROR "Expected to be able to evaluate \${BOOST_LIB}=\"${BOOST_LIB}\" to an existing directory, is BOOST set up in the environment?")
elseif(NOT DEFINED BOOST_LIB)
  cmessage(FATAL_ERROR "Expected to be able to evaluate \${BOOST_LIB}=\"${BOOST_LIB}\" to an existing directory, is BOOST set up in the environment?")
endif()

LIST(APPEND EXTRA_LINK_DIRS ${BOOST_LIB})
LIST(APPEND EXTRA_LIBS boost_filesystem boost_system)

###############################  Eigen  ######################################

include(cmake/eigenSetup.cmake)

############################  TH2Jagged  #####################################

include(cmake/TH2JaggedSetup.cmake)

############################  FHiCL  #####################################

include(cmake/FHiCLSetup.cmake)
