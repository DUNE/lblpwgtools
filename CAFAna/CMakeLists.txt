cmake_minimum_required (VERSION 3.14 FATAL_ERROR)

#Use the compilers found in the path
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

project(CAFAna)

#Changes default install path to be a subdirectory of the build dir.
#Can set build dir at configure time with -DCMAKE_INSTALL_PREFIX=/install/path
if(NOT DEFINED CMAKE_INSTALL_PREFIX OR 
    "${CMAKE_INSTALL_PREFIX}x" STREQUAL "x" OR 
    "${CMAKE_INSTALL_PREFIX}x" STREQUAL "/usr/localx")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE OR 
    "${CMAKE_BUILD_TYPE}x" STREQUAL "x")
  set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT DEFINED USE_OPENMP OR 
    "${USE_OPENMP}x" STREQUAL "x")
  set(USE_OPENMP FALSE)
endif()

SET(DEF_NUM_THREADS 1)
if(USE_OPENMP)
  SET(DEF_NUM_THREADS 4)
endif()

SET(BUILD_DEBUG_MSGS TRUE)
include(${CMAKE_SOURCE_DIR}/cmake/cmessage.cmake)

if(USED_UPS STREQUAL "")
  set(USED_UPS "0")
elseif(NOT DEFINED USED_UPS)
  set(USED_UPS "0")
endif()

set(VERBOSE true)

cmessage(STATUS "CMAKE_INSTALL_PREFIX: \"${CMAKE_INSTALL_PREFIX}\"")
cmessage(STATUS "CMAKE_BUILD_TYPE: \"${CMAKE_BUILD_TYPE}\"")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(${CMAKE_SOURCE_DIR}/cmake/FindExternals.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/c++CompilerSetup.cmake)
################################################################################

## StandardRecord
get_filename_component(SRC_ROOT_PARENT ${CMAKE_SOURCE_DIR}/../ ABSOLUTE)

include_directories(${SRC_ROOT_PARENT})

set(StandardRecord_implementation_files
    ${SRC_ROOT_PARENT}/StandardRecord/StandardRecord.cxx
)

set(StandardRecord_header_files
    ${SRC_ROOT_PARENT}/StandardRecord/StandardRecord.h
    ${SRC_ROOT_PARENT}/StandardRecord/SRProxy.h
    ${SRC_ROOT_PARENT}/StandardRecord/FwdDeclare.h
)

add_library(StandardRecord SHARED ${StandardRecord_implementation_files})

set_target_properties(StandardRecord PROPERTIES
  PUBLIC_HEADER "${StandardRecord_header_files}")

install(TARGETS StandardRecord 
    LIBRARY DESTINATION lib/
    PUBLIC_HEADER DESTINATION include/)

SET(CAFAna_Libs
    CAFAnaAnalysis
    CAFAnaDecomp
    CAFAnaFit
    CAFAnaPrediction
    CAFAnaExperiment
    CAFAnaPrediction
    CAFAnaPRISM
    CAFAnaCuts
    CAFAnaExtrap
    CAFAnaVars
    CAFAnaCore
    CAFAnaSysts)

add_subdirectory(Analysis)
add_subdirectory(Core)
add_subdirectory(Cuts)
add_subdirectory(Decomp)
add_subdirectory(Experiment)
add_subdirectory(Extrap)
add_subdirectory(Fit)
add_subdirectory(Prediction)
add_subdirectory(Systs)
add_subdirectory(Vars)
add_subdirectory(PRISM)
add_subdirectory(bin)
add_subdirectory(scripts)
add_subdirectory(fcl)

configure_file(cmake/CAFAnaEnv.sh.in
  ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFAnaEnv.sh @ONLY)

install(
  PROGRAMS
    ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFAnaEnv.sh
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/
  )

install(
  PROGRAMS
    ${BUILD_ENV_SCRIPTS}
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/env/
  )

install(
  PROGRAMS
    ${CMAKE_SOURCE_DIR}/cafe
  DESTINATION
    bin
  )
