# Install script for directory: /exp/dune/app/users/gyang/lblpwgtools/CAFAna

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Linux")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RELEASE")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libStandardRecord.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libStandardRecord.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libStandardRecord.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/libStandardRecord.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libStandardRecord.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libStandardRecord.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libStandardRecord.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/StandardRecord" TYPE FILE FILES
    "/exp/dune/app/users/gyang/lblpwgtools/StandardRecord/SRDune.h"
    "/exp/dune/app/users/gyang/lblpwgtools/StandardRecord/StandardRecord.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOscLibFunc.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOscLibFunc.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOscLibFunc.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/libOscLibFunc.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOscLibFunc.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOscLibFunc.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libOscLibFunc.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OscLib/func" TYPE FILE FILES
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/EarthModel.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/IOscCalculator.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/OscCalculator.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/OscCalculatorCPT.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/OscCalculatorGeneral.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/OscCalculatorPMNS.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/OscCalculatorPMNSOpt.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/OscCalculatorPMNS_CPT.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/OscCalculatorPMNS_NSI.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/OscCalculatorSterile.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/OscCalculatorSterileBeam.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/PMNS.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/PMNSOpt.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/PMNS_NSI.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/OscLib/func/PMNS_Sterile.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtilitiesFunc.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtilitiesFunc.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtilitiesFunc.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/libUtilitiesFunc.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtilitiesFunc.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtilitiesFunc.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libUtilitiesFunc.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities/func" TYPE FILE FILES
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/Utilities/func/MathUtil.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/Utilities/func/NuMITimeParams.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/Utilities/func/RandHisto.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/Utilities/func/ROOTHelpers.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/Utilities/func/EnvExpand.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Utilities" TYPE FILE FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Ext/Utilities/rootlogon.C")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Linux/CAFAnaEnv.sh;/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Linux/CAFEnvWrapper.sh;/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Linux/ups_env_setup.sh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Linux" TYPE PROGRAM FILES
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/CMakeFiles/CAFAnaEnv.sh"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/CMakeFiles/CAFEnvWrapper.sh"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/cmake/ups_env_setup.sh"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM RENAME "cafe" FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/cafe_dune")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/CMakeFiles/CAFBuild")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Linux/load_libs.C")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Linux" TYPE FILE RENAME "load_libs.C" FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/load_libs_dune.C")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Analysis/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Core/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Cuts/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Decomp/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Experiment/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Extrap/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Prediction/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Systs/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Vars/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/bin/cmake_install.cmake")
  include("/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/scripts/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
