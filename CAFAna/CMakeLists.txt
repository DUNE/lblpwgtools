cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

#Use the compilers found in the path
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

project(CAFAna)

#Changes default install path to be a subdirectory of the build dir.
#Can set build dir at configure time with -DCMAKE_INSTALL_PREFIX=/install/path
if(CMAKE_INSTALL_PREFIX STREQUAL "" OR CMAKE_INSTALL_PREFIX STREQUAL
  "/usr/local")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
elseif(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CMAKE_SYSTEM_NAME}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE DEBUG)
elseif(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

if(KNL STREQUAL "")
  set(KNL FALSE)
elseif(NOT DEFINED KNL)
  set(KNL FALSE)
endif()

if(USE_OPENMP STREQUAL "")
  set(USE_OPENMP FALSE)
elseif(NOT DEFINED USE_OPENMP)
  set(USE_OPENMP FALSE)
endif()

SET(DEF_NUM_THREADS 1)
if(USE_OPENMP)
  SET(DEF_NUM_THREADS 4)
endif()

SET(BUILD_DEBUG_MSGS TRUE)
include(${CMAKE_SOURCE_DIR}/cmake/cmessage.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/parseConfigApp.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/StringAndListUtils.cmake)

if(USED_UPS STREQUAL "")
  set(USED_UPS "0")
elseif(NOT DEFINED USED_UPS)
  set(USED_UPS "0")
endif()

set(VERBOSE true)

cmessage(STATUS "CMAKE_INSTALL_PREFIX: \"${CMAKE_INSTALL_PREFIX}\"")
cmessage(STATUS "CMAKE_BUILD_TYPE: \"${CMAKE_BUILD_TYPE}\"")

################################################################################
#                            Check Dependencies
################################################################################

##################################  ROOT  ######################################
include(${CMAKE_SOURCE_DIR}/cmake/ROOTSetup.cmake)
############################## GSL/BOOST/CLEHP #################################
include(${CMAKE_SOURCE_DIR}/cmake/ExternalLibrariesCheck.cmake)
################################## COMPILER ####################################
include_directories(
  ${SRC_ROOT_PARENT}
  ${CMAKE_BINARY_DIR}/Ext
  ${BOOST_INC}
  ${EIGEN_INC}
  ${SUNDIALS_INC}
  ${STAN_MATH_INC}
  ${STAN_INC})

LIST(APPEND EXTRA_CXX_FLAGS -DDONT_USE_SAM=1 -DUSE_CAFANA_ENVVAR=1 -DDONT_USE_FQ_HARDCODED_SYST_PATHS=1 -DALLOW_XROOTD_PATH_THROUGH_WILDCARDSOURCE=1)

include(${CMAKE_SOURCE_DIR}/cmake/c++CompilerSetup.cmake)
################################################################################

include(${CMAKE_SOURCE_DIR}/cmake/NOvAExternals.cmake)

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
add_subdirectory(bin)

add_subdirectory(scripts)

configure_file(cmake/CAFAnaEnv.sh.in
  ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFAnaEnv.sh @ONLY)
configure_file(cmake/CAFBuild.in
${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFBuild @ONLY)
configure_file(cmake/CAFEnvWrapper.sh.in
${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFEnvWrapper.sh @ONLY)

install(
  PROGRAMS
    ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFAnaEnv.sh
    ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFEnvWrapper.sh
    ${CMAKE_SOURCE_DIR}/cmake/ups_env_setup.sh
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/
  )


install(
  PROGRAMS
    ${CMAKE_SOURCE_DIR}/cafe_dune
  DESTINATION
    bin
  RENAME
    cafe
  )

install(
  PROGRAMS
    ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CAFBuild
  DESTINATION
    bin
  )

install(
  FILES
    ${CMAKE_SOURCE_DIR}/load_libs_dune.C
  DESTINATION
    ${CMAKE_INSTALL_PREFIX}/
  RENAME
    load_libs.C
  )
