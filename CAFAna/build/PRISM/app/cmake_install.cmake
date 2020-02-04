# Install script for directory: /dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/PRISM/app

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Linux")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "DEBUG")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/scripts" TYPE FILE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/PRISM/app/MakePRISMPredInterps.C")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MakePRISMPredInterps" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MakePRISMPredInterps")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MakePRISMPredInterps"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM/app/MakePRISMPredInterps")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MakePRISMPredInterps" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MakePRISMPredInterps")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MakePRISMPredInterps"
         OLD_RPATH "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Analysis:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Fit:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Decomp:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Prediction:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Core:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Experiment:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Systs:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Cuts:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Extrap:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Vars:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/MakePRISMPredInterps")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/scripts" TYPE FILE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/PRISM/app/OffAxisEventRates.C")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisEventRates" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisEventRates")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisEventRates"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM/app/OffAxisEventRates")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisEventRates" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisEventRates")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisEventRates"
         OLD_RPATH "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Analysis:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Fit:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Decomp:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Prediction:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Core:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Experiment:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Systs:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Cuts:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Extrap:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Vars:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisEventRates")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/scripts" TYPE FILE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/PRISM/app/OffAxisNDCAFCombiner.C")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisNDCAFCombiner" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisNDCAFCombiner")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisNDCAFCombiner"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM/app/OffAxisNDCAFCombiner")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisNDCAFCombiner" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisNDCAFCombiner")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisNDCAFCombiner"
         OLD_RPATH "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Analysis:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Fit:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Decomp:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Prediction:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Core:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Experiment:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Systs:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Cuts:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Extrap:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Vars:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/OffAxisNDCAFCombiner")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/scripts" TYPE FILE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/PRISM/app/PRISMPrediction.C")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMPrediction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMPrediction")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMPrediction"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM/app/PRISMPrediction")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMPrediction" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMPrediction")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMPrediction"
         OLD_RPATH "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Analysis:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Fit:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Decomp:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Prediction:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Core:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Experiment:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Systs:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Cuts:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Extrap:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Vars:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMPrediction")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/scripts" TYPE FILE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/PRISM/app/PRISMDChi2Scan.C")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMDChi2Scan" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMDChi2Scan")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMDChi2Scan"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM/app/PRISMDChi2Scan")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMDChi2Scan" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMDChi2Scan")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMDChi2Scan"
         OLD_RPATH "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Analysis:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Fit:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Decomp:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Prediction:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Core:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Experiment:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Systs:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/PRISM:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Cuts:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Extrap:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Vars:/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/PRISMDChi2Scan")
    endif()
  endif()
endif()

