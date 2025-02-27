cmake_minimum_required (VERSION 3.14 FATAL_ERROR)

#Use the compilers found in the path
find_program(CMAKE_C_COMPILER NAMES $ENV{CC} gcc PATHS ENV PATH NO_DEFAULT_PATH)
find_program(CMAKE_CXX_COMPILER NAMES $ENV{CXX} g++ PATHS ENV PATH NO_DEFAULT_PATH)

project(CAFAna VERSION 2.00.00)

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

################################################################################
#                            Check Dependencies
################################################################################

include_directories(
  ${SRC_ROOT_PARENT}
  ${CMAKE_BINARY_DIR}/Ext
  ${BOOST_INC}
  ${EIGEN_INC}
  ${SUNDIALS_INC}
  ${STAN_MATH_INC}
  ${STAN_INC}
  ${TBB_INC}
  $ENV{OSCLIB_INC}
  $ENV{CAFANACORE_INC}
  $ENV{DUNEANAOBJ_INC}
)

find_library(CAFANACOREEXT CAFAnaCoreExt $ENV{CAFANACORE_LIB})
find_library(STANDARDRECORD duneanaobj_StandardRecord $ENV{DUNEANAOBJ_LIB})
find_library(STANDARDRECORDPROXY duneanaobj_StandardRecordProxy $ENV{DUNEANAOBJ_LIB})

include_directories($ENV{SRPROXY_INC})

include_directories(..) # allow paths starting CAFAna/ to work

LIST(APPEND EXTRA_CXX_FLAGS -DDONT_USE_SAM=1 -DUSE_CAFANA_ENVVAR=1 -DDONT_USE_FQ_HARDCODED_SYST_PATHS=1 -DALLOW_XROOTD_PATH_THROUGH_WILDCARDSOURCE=1 -DTBB_INTERFACE_NEW)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL Darwin)
  set(CMAKE_MACOSX_RPATH ON)
  LIST(APPEND CMAKE_INSTALL_RPATH "@executable_path/../lib")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(${CMAKE_SOURCE_DIR}/cmake/FindExternals.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/c++CompilerSetup.cmake)
################################################################################

# we're going to create an interface library that bundles all the CAFAna libs together
# easier if we don't have to know all their names in advance...
macro(add_cafana_lib newlib)
  list(APPEND CAFANA_LIBS ${newlib})
  set(CAFANA_LIBS ${CAFANA_LIBS} PARENT_SCOPE)
#  message(STATUS "   $CAFANA_LIBS now contains: '${CAFANA_LIBS}'")
endmacro()

################################################################################

set(CAFANA_SUBDIRS Analysis Core Cuts Experiment Extrap Fit Prediction Systs Vars bin scripts fcl)
if(DEFINED BUILD_PRISM)
  set(CAFANA_SUBDIRS ${CAFANA_SUBDIRS} PRISM)
else()
  message(WARNING "Not building PRISM.  Pass '-DBUILD_PRISM' using '-e BUILD_PRISM' in standalone_configure_and_build.sh to change this.")
endif()
foreach(SUBDIR ${CAFANA_SUBDIRS})

  add_subdirectory(${SUBDIR})
endforeach()

# add an 'interface' (= no actual file) library
# that one can import in another CMake project
# to be able to link against all of the CAFAna libs
add_library(All INTERFACE)
message(STATUS "$CAFANA_LIBS contains: '${CAFANA_LIBS}'")
target_link_libraries(All INTERFACE ${CAFANA_LIBS})
target_include_directories(All INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

################################################################################

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/CAFAnaConfigVersion.cmake"
  VERSION ${CMAKE_PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)
configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/templates/CAFAnaConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/CAFAnaConfig.cmake"
        INSTALL_DESTINATION lib/cmake/CAFAna
)

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

# install our CMake files so that others can crib them too
file(GLOB cmake_files ${CMAKE_SOURCE_DIR}/cmake/*.cmake)
install(
  FILES
    ${cmake_files}
  DESTINATION
    cmake
)

install(TARGETS All
    EXPORT CAFAnaTargets
)

install(EXPORT CAFAnaTargets NAMESPACE CAFAna:: DESTINATION lib/cmake/CAFAna)
install(FILES "${PROJECT_BINARY_DIR}/CAFAnaConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/CAFAnaConfig.cmake"
        DESTINATION lib/cmake/CAFAna)
