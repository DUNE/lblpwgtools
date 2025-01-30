# Install script for directory: /exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaAnalysis.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaAnalysis.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaAnalysis.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Analysis/libCAFAnaAnalysis.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaAnalysis.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaAnalysis.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaAnalysis.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CAFAna/Analysis" TYPE FILE FILES
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/Calcs.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/CalcsNuFit.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/Fit.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/GradientDescent.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/Plots.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/Resolution.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/Surface.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/Exposures.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/Style.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/SeedList.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/RefineSeeds.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/AnalysisBinnings.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/AnalysisDialGroups.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/AnalysisVars.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/common_fit_definitions.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/AnalysisVersion.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/ExtraHeaders.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/CheckPointHelper.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Analysis/XSecSystList.h"
    )
endif()

