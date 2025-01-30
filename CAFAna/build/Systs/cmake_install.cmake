# Install script for directory: /exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaSysts.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaSysts.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaSysts.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/build/Systs/libCAFAnaSysts.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaSysts.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaSysts.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaSysts.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CAFAna/Systs" TYPE FILE FILES
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/DUNEFluxSysts.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/DUNENDSysts.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/DUNEXSecSysts.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/DUNEXSecSystsPCA.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/EnergySysts.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/FDRecoSysts.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/XSecSysts.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/NDRecoSysts.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/NuOnESysts.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/SystComponentScale.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/Systs.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/MissingProtonFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/BDTReweighter.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GeneratorReweight_nuebar_FD_RHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GeneratorReweight_nue_FD_FHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GeneratorReweight_nue_FD_RHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GeneratorReweight_numubar_FD_RHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GeneratorReweight_numubar_ND_RHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GeneratorReweight_numu_FD_FHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GeneratorReweight_numu_FD_RHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GeneratorReweight_numu_ND_FHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GeneratorReweight_numu_ND_RHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/MissingProtonFakeData_BDTRW_FHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/MissingProtonFakeData_BDTRW_RHC.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/NuWroReweightFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/NuWroReweightFakeDataOld.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/NEUTReweightFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GENIEv3_G18_10b_00_000ReweightFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/GENIEv3_G18_10a_02_11aReweightFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/CrazyFluxFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/ProtonNeutronEnergyShiftFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/ArCScaling2DFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/TMSBeamMonitorFDS.cxx"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/FluxWiggleFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/FluxWiggleSqueezeFakeData.h"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/ReweightFakeDataBase.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Systs" TYPE FILE FILES
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/flux_shifts_CDR.root"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/flux_shifts_Nov17.root"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/xsec_shifts.root"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/modelComp.root"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/ND_eff_syst.root"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/det_sys_cov.root"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/det_sys_cov_v3binning.root"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/ArCScaling2DFakeData.root"
    "/exp/dune/app/users/gyang/lblpwgtools/CAFAna/Systs/tms_beamMonitoring_weights.root"
    )
endif()

