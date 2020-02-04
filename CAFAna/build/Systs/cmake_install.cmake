# Install script for directory: /dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaSysts.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaSysts.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCAFAnaSysts.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Systs/libCAFAnaSysts.so")
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
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/DUNEFluxSysts.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/DUNENDSysts.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/DUNEXSecSysts.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/DUNEXSecSystsPCA.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/EnergySysts.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/FDRecoSysts.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/XSecSysts.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/NDRecoSysts.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/NuOnESysts.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/SystComponentScale.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/Systs.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/MissingProtonFakeData.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/BDTReweighter.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/GeneratorReweight_nuebar_FD_RHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/GeneratorReweight_nue_FD_FHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/GeneratorReweight_nue_FD_RHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/GeneratorReweight_numubar_FD_RHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/GeneratorReweight_numubar_ND_RHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/GeneratorReweight_numu_FD_FHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/GeneratorReweight_numu_FD_RHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/GeneratorReweight_numu_ND_FHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/GeneratorReweight_numu_ND_RHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/MissingProtonFakeData_BDTRW_FHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/MissingProtonFakeData_BDTRW_RHC.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/NuWroReweightFakeData.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/CrazyFluxFakeData.h"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/XSecSystList.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Systs" TYPE FILE FILES
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/flux_shifts_CDR.root"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/flux_shifts_Nov17.root"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/flux_shifts_OffAxis.root"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/xsec_shifts.root"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/modelComp.root"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/ND_eff_syst.root"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/det_sys_cov.root"
    "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/Systs/det_sys_cov_v3binning.root"
    )
endif()

