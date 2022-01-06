include(FindPackageHandleStandardArgs)

if(DEFINED ENV{CAFANACORE_INC})
  SET(CAFANACORE_INC $ENV{CAFANACORE_INC})
endif()

if(DEFINED ENV{CAFANACORE_LIB})
  SET(CAFANACORE_LIB $ENV{CAFANACORE_LIB})
endif()

find_path(CAFAnaCore_INC_DIR
  NAMES CAFAnaCore/CAFAna/Core/Spectrum.h
  PATHS ${CAFANACORE_INC})

find_path(CAFAnaCore_LIB_DIR
  NAMES libCAFAnaCoreExt.so
  PATHS ${CAFANACORE_LIB})

find_path(CAFAnaCoreSAM
  NAMES CAFAnaCore/CAFAna/Core/SAMProjectSource.h
  PATHS ${CAFANACORE_INC})

SET(CAFAnaCore_CompileOptions)
if(NOT "${CAFAnaCoreSAM}x" STREQUAL "CAFAnaCoreSAM-NOTFOUNDx")
  SET(CAFAnaCore_CompileOptions -DWITH_SAM)
endif()

find_package_handle_standard_args(CAFAnaCore
	REQUIRED_VARS CAFAnaCore_INC_DIR CAFAnaCore_LIB_DIR
)

if(CAFAnaCore_FOUND)

  SET(CAFAnaCore_LIB_DIRS ${CAFAnaCore_LIB_DIR})
  SET(CAFAnaCore_LIBS CAFAnaCoreExt)

  cmessage(STATUS "CAFAnaCore")
  cmessage(STATUS "     LIB_DIRS: ${CAFAnaCore_LIB_DIRS}")
  cmessage(STATUS "     INC_DIRS: ${CAFAnaCore_INC_DIR}")
  cmessage(STATUS "         LIBS: ${CAFAnaCore_LIBS}")

  if(NOT TARGET CAFAnaCore::Ext)
    add_library(CAFAnaCore::Ext INTERFACE IMPORTED)
    set_target_properties(CAFAnaCore::Ext PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CAFAnaCore_INC_DIR}"
        INTERFACE_COMPILE_OPTIONS "${CAFAnaCore_CompileOptions}"
        INTERFACE_LINK_DIRECTORIES "${CAFAnaCore_LIB_DIRS}"
        INTERFACE_LINK_LIBRARIES "${CAFAnaCore_LIBS}"
    )
  endif()
endif()