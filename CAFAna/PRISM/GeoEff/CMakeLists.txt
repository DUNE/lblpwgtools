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
################################################################################

option(GEOEFF_BUILD_EDEPSIM "Build runGeoEffEDepSimBeamDir (runs ROOT MakeProject on data/step1)" OFF)
option(GEOEFF_BUILD_PYTHON  "Build the pyGeoEff pybind11 module (needs pybind11 >= 2.10)" OFF)

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
set(GEOEFF_STEP3_OSC_APPS
  NtupleOutVetoAndTrimE_AssumeEqualEffAtAllOA_WithOscSpectrumNOCAFAna_FDRateAtND
  NtupleOutVetoAndTrimE_AllOApos_WithOscSpectrumNOCAFAna_FDRateAtND)

foreach(app ${GEOEFF_STEP3_NOOSC_APPS} ${GEOEFF_STEP3_OSC_APPS})
  set_source_files_properties(app/${app}.C PROPERTIES LANGUAGE CXX)
  add_executable(${app} app/${app}.C)
  target_include_directories(${app} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_link_libraries(${app} ROOT::ROOT ${ROOT_LIBRARIES})
  install(TARGETS ${app} DESTINATION bin)
endforeach()

# ana::DefaultOscCalc etc. come from CAFAna/Analysis/Calcs.cxx
foreach(app ${GEOEFF_STEP3_OSC_APPS})
  target_link_libraries(${app} CAFAnaAnalysis OscLib::All)
endforeach()

# --- Optional python bindings ------------------------------------------------
if(GEOEFF_BUILD_PYTHON)
  find_package(pybind11 CONFIG REQUIRED)
  pybind11_add_module(pyGeoEff src/pyGeoEff.cpp)
  target_link_libraries(pyGeoEff PRIVATE geoEff)
  install(TARGETS pyGeoEff LIBRARY DESTINATION lib/)
endif()
