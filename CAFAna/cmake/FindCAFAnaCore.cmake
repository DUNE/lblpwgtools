include(FindPackageHandleStandardArgs)

if(DEFINED ENV{CAFANACORE_INC})
  SET(CAFANACORE_INC $ENV{CAFANACORE_INC})
endif()

if(DEFINED ENV{CAFANACORE_LIB})
  SET(CAFANACORE_LIB $ENV{CAFANACORE_LIB})
endif()

find_path(CAFAnaCore_INC_DIR
  NAMES CAFAnaCore/CAFAna/Core/Spectrum.h
  PATHS ${CAFANACORE_INC}
  NO_DEFAULT_PATH)

find_path(CAFAnaCore_LIB_DIR
  NAMES libCAFAnaCoreExt.so libCAFAnaCoreExt.dylib
  PATHS ${CAFANACORE_LIB}
  NO_DEFAULT_PATH)

SET(CAFAnaCore_LIB_DIRS ${CAFAnaCore_LIB_DIR})
SET(CAFAnaCore_INC_DIRS ${CAFAnaCore_INC_DIR})
SET(CAFAnaCore_LIBS CAFAnaCoreExt)

find_path(CAFAnaCoreSAM
  NAMES CAFAnaCore/CAFAna/Core/SAMProjectSource.h
  PATHS ${CAFANACORE_INC}
  NO_DEFAULT_PATH)

SET(CAFAnaCore_CompileOptions)
if(NOT "${CAFAnaCoreSAM}x" STREQUAL "CAFAnaCoreSAM-NOTFOUNDx")

  if(DEFINED ENV{IFDHC_INC})
    SET(IFDHC_INC $ENV{IFDHC_INC})
  endif()

  if(DEFINED ENV{IFDHC_LIB})
    SET(IFDHC_LIB $ENV{IFDHC_LIB})
  endif()

  find_path(IFDHC_INC_DIR
    NAMES ifdh.h
    PATHS ${IFDHC_INC}
    REQUIRED
    NO_DEFAULT_PATH)

  find_path(IFDHC_LIB_DIR
    NAMES libifdh.so
    PATHS ${IFDHC_LIB}
    REQUIRED
    NO_DEFAULT_PATH)

  LIST(APPEND CAFAnaCore_CompileOptions -DWITH_SAM)
  LIST(APPEND CAFAnaCore_LIB_DIRS ${IFDHC_LIB_DIR})
  LIST(APPEND CAFAnaCore_INC_DIRS ${IFDHC_INC_DIR})
  LIST(APPEND CAFAnaCore_LIBS ifdh)
endif()

find_package_handle_standard_args(CAFAnaCore
	REQUIRED_VARS CAFAnaCore_INC_DIR CAFAnaCore_LIB_DIR
)

if(CAFAnaCore_FOUND)


  cmessage(STATUS "CAFAnaCore")
  cmessage(STATUS "     LIB_DIRS: ${CAFAnaCore_LIB_DIRS}")
  cmessage(STATUS "     INC_DIRS: ${CAFAnaCore_INC_DIR}")
  cmessage(STATUS "         LIBS: ${CAFAnaCore_LIBS}")

  if(NOT TARGET CAFAnaCore::Ext)
    add_library(CAFAnaCore::Ext INTERFACE IMPORTED)
    set_target_properties(CAFAnaCore::Ext PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CAFAnaCore_INC_DIRS}"
        INTERFACE_COMPILE_OPTIONS "${CAFAnaCore_CompileOptions}"
        INTERFACE_LINK_DIRECTORIES "${CAFAnaCore_LIB_DIRS}"
        INTERFACE_LINK_LIBRARIES "${CAFAnaCore_LIBS}"
    )
    LIST(APPEND CMAKE_INSTALL_RPATH "${CAFAnaCore_LIB_DIR}")
  endif()
endif()
