find_package(perftools)

find_package(GSL REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

if (CAFANACORE_USE_STAN)
find_package(Stan REQUIRED)
else()
  # Stan would normally drag Eigen in, but if we're not using Stan, we can't count on that
  find_package(Eigen3Local REQUIRED)
endif()

find_package(OscLib REQUIRED)
find_package(CAFAnaCore REQUIRED)

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

STRING(STRIP ROOT_CXX_FLAGS ${ROOT_CXX_FLAGS})
STRING(REPLACE " " ";" ROOT_CXX_FLAGS ${ROOT_CXX_FLAGS})

if("${CMAKE_PROJECT_NAME} " STREQUAL "CAFAna ")
  list (FIND ROOT_CXX_FLAGS "-std=c++11" CPP11_INDEX)
  list (FIND ROOT_CXX_FLAGS "-std=c++14" CPP14_INDEX)
  list (FIND ROOT_CXX_FLAGS "-std=c++17" CPP17_INDEX)
  list (FIND ROOT_CXX_FLAGS "-std=c++20" CPP20_INDEX)
  if (${CPP17_INDEX} GREATER -1)
    SET(CMAKE_CXX_STANDARD 17)
  elseif (${CPP20_INDEX} GREATER -1)
    SET(CMAKE_CXX_STANDARD 20)
  else()
    SET(CMAKE_CXX_STANDARD 14)
  endif()
endif()
list(FILTER ROOT_CXX_FLAGS EXCLUDE REGEX "-std=.*")

execute_process (COMMAND root-config
  --version OUTPUT_VARIABLE ROOT_CONFIG_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

#We should let CMake set this
list(REMOVE_ITEM ROOT_CXX_FLAGS "-fPIC")

if(NOT TARGET ROOT::ROOT)
  add_library(ROOT::ROOT INTERFACE IMPORTED)

  LIST(APPEND ROOT_LIB_NAMES
    Core
    RIO
    Net
    Hist
    Graf
    Graf3d
    Gpad
    Tree
    Rint
    Postscript
    Matrix
    Physics
    MathCore
    Thread
    TMVA)

# function(dump_cmake_variables)
#     get_cmake_property(_variableNames VARIABLES)
#     list (SORT _variableNames)
#     foreach (_variableName ${_variableNames})
#         if (ARGV0)
#             unset(MATCHED)
#             string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})
#             if (NOT MATCHED)
#                 continue()
#             endif()
#         endif()
#         cmessage(DEBUG "${_variableName}=${${_variableName}}")
#     endforeach()
# endfunction()
#   dump_cmake_variables(ROOT)

  foreach(LN ${ROOT_LIB_NAMES})
    if(NOT EXISTS ${ROOT_LIBRARY_DIR}/lib${LN}.so)
      cmessage(FATAL_ERROR "Cannot find ROOT library: ${LN} in expected directory: ${ROOT_LIBRARY_DIR}")
    endif()
  endforeach()

  set_target_properties(ROOT::ROOT PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${ROOT_INCLUDE_DIRS}"
      INTERFACE_COMPILE_OPTIONS "${ROOT_CXX_FLAGS}"
      INTERFACE_LINK_DIRECTORIES "${ROOT_LIBRARY_DIR}"
      INTERFACE_LINK_LIBRARIES "${ROOT_LIB_NAMES}"
  )
  cmessage(STATUS "Built ROOT::ROOT Imported target")
  cmessage(STATUS "        ROOT_INCLUDE_DIRS: ${ROOT_INCLUDE_DIRS}: ")
  cmessage(STATUS "        ROOT_CXX_FLAGS: ${ROOT_CXX_FLAGS}")
  cmessage(STATUS "        ROOT_LIB_DIR: ${ROOT_LIBRARY_DIR}")
  cmessage(STATUS "        ROOT_LIBRARIES: ${ROOT_LIB_NAMES}")
endif()


#These need to come after ROOT or they will set up ROOT themselves (with the wrong libraries required)
find_package(TH2Jagged REQUIRED)
add_compile_options(-DUSE_TH2JAGGED)

# This is the only real SO dependency if we are using CVMFS to pull in the others,
# so dump this in the lib/ folder to aid cluster deployment
install(FILES
  ${TH2Jagged_LIB_DIR}/libTH2Jagged_rdict.pcm
  ${TH2Jagged_LIB_DIR}/libTH2Jagged.rootmap
  DESTINATION lib/)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
  install(FILES
  ${TH2Jagged_LIB_DIR}/libTH2Jagged.so
  DESTINATION lib/)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
  install(FILES
    ${TH2Jagged_LIB_DIR}/libTH2Jagged.dylib
    DESTINATION lib/)
endif()

if(DEFINED ENV{FHICLCPP_FQ_DIR})
  # If fhiclcpp comes from ups, then look for it in this path
  find_package(fhiclcpp REQUIRED PATHS $ENV{FHICLCPP_FQ_DIR})
  # and find tbb which apparently doesn't happen by default
  find_package(TBB REQUIRED PATHS $ENV{TBB_FQ_DIR})
else()
  # Otherise it comes from support/
  find_package(fhiclcpp REQUIRED)
endif()
