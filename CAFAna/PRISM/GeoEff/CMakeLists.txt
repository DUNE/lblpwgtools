################################################################################
# PRISM/GeoEff -- DUNE ND geometric efficiency (imported from DUNE_ND_GeoEff)
#
#   include/   geoEff.h, step1/step3 helper headers
#   src/       geoEff library (+ optional pybind11 module)
#   app/       step 1 (hadron geo-eff) and step 3 (Etrim) executables,
#              step 2 python scripts (not built)
#   macros/, scripts/, data/   ROOT macros, env/grid scripts, inputs (not built)
#
# The step 3 apps read inputs via relative paths "data/step3/...", so run
# them from this directory (or one containing data/step3/).
#
# Two ways to build:
#   * as part of CAFAna (CAFAna/PRISM/CMakeLists.txt does add_subdirectory)
#   * standalone, without building CAFAna (used by the grid scripts):
#       cmake -S CAFAna/PRISM/GeoEff -B build && cmake --build build -j
#     ROOT + Eigen are required; OscLib (OSCLIB_INC/OSCLIB_LIB) is needed for
#     the oscillated step 3 apps, which then compile CAFAna/Analysis/Calcs.cxx
#     directly instead of linking CAFAnaAnalysis.
################################################################################

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  cmake_minimum_required(VERSION 3.14)
  project(GeoEff LANGUAGES CXX)
  set(GEOEFF_STANDALONE TRUE)

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
  endif()
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  add_compile_options(-Wall -Wextra)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

  find_package(ROOT REQUIRED)
  if(NOT TARGET ROOT::ROOT)
    add_library(ROOT::ROOT INTERFACE IMPORTED)
    set_target_properties(ROOT::ROOT PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${ROOT_INCLUDE_DIRS}"
      INTERFACE_LINK_LIBRARIES "${ROOT_LIBRARIES}")
  endif()
  find_package(Eigen3 REQUIRED)

  option(GEOEFF_BUILD_STEP3_OSC "Build the oscillated step 3 apps (needs OscLib)" ON)
else()
  set(GEOEFF_STANDALONE FALSE)
  set(GEOEFF_BUILD_STEP3_OSC ON)
endif()

option(GEOEFF_BUILD_EDEPSIM "Build runGeoEffEDepSimBeamDir (runs ROOT MakeProject on data/step1)" OFF)
option(GEOEFF_BUILD_PYTHON  "Build the pyGeoEff pybind11 module (needs pybind11 >= 2.10)" OFF)

# ana::DefaultOscCalc etc. (CAFAna/Analysis/Calcs.cxx) for the oscillated
# step 3 apps: link CAFAnaAnalysis inside CAFAna, compile it directly standalone.
if(GEOEFF_BUILD_STEP3_OSC AND GEOEFF_STANDALONE)
  get_filename_component(GEOEFF_LBLPWGTOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../.. ABSOLUTE)
  find_path(OSCLIB_INCLUDE_DIR OscLib/OscCalcPMNSOpt.h
    HINTS $ENV{OSCLIB_INC} $ENV{OSCLIB_ROOT}/include)
  find_library(OSCLIB_LIBRARY NAMES OscLib
    HINTS $ENV{OSCLIB_LIB} $ENV{OSCLIB_ROOT}/lib)
  if(OSCLIB_INCLUDE_DIR AND OSCLIB_LIBRARY
     AND EXISTS ${GEOEFF_LBLPWGTOOLS_DIR}/CAFAna/Analysis/Calcs.cxx)
    find_library(GSL_LIBRARY NAMES gsl HINTS $ENV{GSL_LIB} $ENV{GSL_ROOT}/lib)
    find_library(GSL_CBLAS_LIBRARY NAMES gslcblas HINTS $ENV{GSL_LIB} $ENV{GSL_ROOT}/lib)
    add_library(geoEffCalcs STATIC ${GEOEFF_LBLPWGTOOLS_DIR}/CAFAna/Analysis/Calcs.cxx)
    target_include_directories(geoEffCalcs PUBLIC ${GEOEFF_LBLPWGTOOLS_DIR} ${OSCLIB_INCLUDE_DIR})
    target_link_libraries(geoEffCalcs PUBLIC ${OSCLIB_LIBRARY} Eigen3::Eigen ROOT::ROOT)
    if(GSL_LIBRARY AND GSL_CBLAS_LIBRARY)
      target_link_libraries(geoEffCalcs PUBLIC ${GSL_LIBRARY} ${GSL_CBLAS_LIBRARY})
    else()
      message(WARNING "GSL not found; OscLib-based step 3 apps may fail to link. Set GSL_LIB.")
    endif()
    set(GEOEFF_CALCS_LIBS geoEffCalcs)
  else()
    message(WARNING "OscLib (OSCLIB_INC/OSCLIB_LIB) or CAFAna/Analysis/Calcs.cxx not found: "
                    "building only the no-oscillation step 3 app.")
    set(GEOEFF_BUILD_STEP3_OSC OFF)
  endif()
else()
  set(GEOEFF_CALCS_LIBS CAFAnaAnalysis OscLib::All)
endif()

# --- geoEff library ----------------------------------------------------------
add_library(geoEff SHARED src/geoEff.cpp)
target_include_directories(geoEff PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/GeoEff>)
target_link_libraries(geoEff PUBLIC Eigen3::Eigen)

install(TARGETS geoEff LIBRARY DESTINATION lib/)
install(FILES include/geoEff.h DESTINATION include/GeoEff)

# --- Step 1: FD hadron geometric efficiency ----------------------------------
add_executable(runGeoEffFDEvtSim app/runGeoEffFDEvtSim.cpp)
target_link_libraries(runGeoEffFDEvtSim geoEff ROOT::ROOT ${ROOT_LIBRARIES})
install(TARGETS runGeoEffFDEvtSim DESTINATION bin)

if(GEOEFF_BUILD_EDEPSIM)
  set(EDEPSIM_ROOT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/data/step1/EDepSimForMakeProject_20200424.root)
  set(EDEPSIM_ROOT_MAKE_PROJECT_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/app/makeEDepSimProject.C)
  set(EDEPSIM_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/EDepSimEvents)
  set(EDEPSIM_LIB ${CMAKE_CURRENT_BINARY_DIR}/lib/libEDepSimEvents.so)
  set(EDEPSIM_HEADER_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)

  add_custom_command(
    OUTPUT ${EDEPSIM_LIB}
    COMMAND root -b -q ${EDEPSIM_ROOT_FILE} ${EDEPSIM_ROOT_MAKE_PROJECT_SCRIPT}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${EDEPSIM_HEADER_DIR} ${CMAKE_CURRENT_BINARY_DIR}/lib
    COMMAND ${CMAKE_COMMAND} -E copy ${EDEPSIM_BUILD_DIR}/EDepSimEvents.so ${EDEPSIM_LIB}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${EDEPSIM_BUILD_DIR} ${EDEPSIM_HEADER_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${EDEPSIM_ROOT_FILE} ${EDEPSIM_ROOT_MAKE_PROJECT_SCRIPT}
    COMMENT "Generating EDepSimEvents library and headers")
  add_custom_target(compile_EDepSimEvents DEPENDS ${EDEPSIM_LIB})

  add_library(EDepSimEvents SHARED IMPORTED)
  set_property(TARGET EDepSimEvents PROPERTY IMPORTED_LOCATION ${EDEPSIM_LIB})
  set_property(TARGET EDepSimEvents PROPERTY IMPORTED_NO_SONAME TRUE)
  add_dependencies(EDepSimEvents compile_EDepSimEvents)

  add_executable(runGeoEffEDepSimBeamDir app/runGeoEffEDepSimBeamDir.cpp)
  target_include_directories(runGeoEffEDepSimBeamDir PRIVATE ${EDEPSIM_HEADER_DIR})
  target_link_libraries(runGeoEffEDepSimBeamDir geoEff EDepSimEvents ROOT::ROOT ${ROOT_LIBRARIES})
  install(TARGETS runGeoEffEDepSimBeamDir DESTINATION bin)
endif()

# --- Step 3: Etrim / PRISM event rates ---------------------------------------
set(GEOEFF_STEP3_NOOSC_APPS
  NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_NOOscSpectrumNOCAFAna_FDRateAtND)
set(GEOEFF_STEP3_OSC_APPS)
if(GEOEFF_BUILD_STEP3_OSC)
  set(GEOEFF_STEP3_OSC_APPS
    NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND
    NtupleOutVetoAndTrimE_AllOApos_WithOscSpectrumNOCAFAna_FDRateAtND)
endif()

foreach(app ${GEOEFF_STEP3_NOOSC_APPS} ${GEOEFF_STEP3_OSC_APPS})
  set_source_files_properties(app/${app}.C PROPERTIES LANGUAGE CXX)
  add_executable(${app} app/${app}.C)
  target_include_directories(${app} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(${app} ROOT::ROOT ${ROOT_LIBRARIES})
  install(TARGETS ${app} DESTINATION bin)
endforeach()

foreach(app ${GEOEFF_STEP3_OSC_APPS})
  target_link_libraries(${app} ${GEOEFF_CALCS_LIBS})
endforeach()

# --- Optional python bindings ------------------------------------------------
if(GEOEFF_BUILD_PYTHON)
  find_package(pybind11 CONFIG REQUIRED)
  pybind11_add_module(pyGeoEff src/pyGeoEff.cpp)
  target_link_libraries(pyGeoEff PRIVATE geoEff)
  install(TARGETS pyGeoEff LIBRARY DESTINATION lib/)
endif()
