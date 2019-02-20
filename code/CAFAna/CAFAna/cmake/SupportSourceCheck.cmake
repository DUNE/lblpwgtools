
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

if($ENV{BOOST_INC} STREQUAL "")
  cmessage(FATAL_ERROR "Expected to be able to evaluate \$ENV{BOOST_INC}=\"$ENV{BOOST_INC}\" to an existing directory, is BOOST set up in the environment?")
elseif(NOT DEFINED ENV{BOOST_INC})
  cmessage(FATAL_ERROR "Expected to be able to evaluate \$ENV{BOOST_INC}=\"$ENV{BOOST_INC}\" to an existing directory, is BOOST set up in the environment?")
endif()
