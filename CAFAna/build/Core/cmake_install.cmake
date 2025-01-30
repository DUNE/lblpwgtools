# Install script for directory: /exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaCore.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaCore.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaCore.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Core/libCAFAnaCore.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaCore.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaCore.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaCore.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CAFAna/Core" TYPE FILE FILES
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/Binning.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/Cut.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/FileListSource.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/HistAxis.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/HistCache.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/IFitVar.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/ISyst.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/Loaders.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/LoadFromFile.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/MultiVar.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/OscCurve.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/OscillatableSpectrum.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/Progress.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/Ratio.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/ReweightableSpectrum.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/Spectrum.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/SpectrumLoader.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/SpectrumLoaderBase.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/SystRegistry.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/SystShifts.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/ThreadPool.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/Utilities.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/Var.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/WildcardSource.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/IFileSource.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/ModeConversionUtilities.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/FitqunConverter.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Core/Hist.h"
    )
endif()

