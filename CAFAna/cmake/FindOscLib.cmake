include(FindPackageHandleStandardArgs)

if(DEFINED ENV{OSCLIB_INC})
  SET(OSCLIB_INC $ENV{OSCLIB_INC})
endif()

if(DEFINED ENV{OSCLIB_LIB})
  SET(OSCLIB_LIB $ENV{OSCLIB_LIB})
endif()

find_path(OscLib_INC_DIR
  NAMES OscLib/IOscCalc.h
  PATHS ${OSCLIB_INC}
  NO_DEFAULT_PATH)

find_path(OscLib_LIB_DIR
  NAMES libOscLib.so libOscLib.dylib
  PATHS ${OSCLIB_LIB}
  NO_DEFAULT_PATH)

find_package_handle_standard_args(OscLib
	REQUIRED_VARS OscLib_INC_DIR OscLib_LIB_DIR
)

if(OscLib_FOUND)

  SET(OscLib_LIB_DIRS ${OscLib_LIB_DIR})
  SET(OscLib_LIBS OscLib)

  cmessage(STATUS "OscLib")
  cmessage(STATUS "     LIB_DIRS: ${OscLib_LIB_DIRS}")
  cmessage(STATUS "     INC_DIRS: ${OscLib_INC_DIR}")
  cmessage(STATUS "         LIBS: ${OscLib_LIBS}")

  if(NOT TARGET OscLib::All)
    add_library(OscLib::All INTERFACE IMPORTED)
    set_target_properties(OscLib::All PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${OscLib_INC_DIR}"
        INTERFACE_LINK_DIRECTORIES "${OscLib_LIB_DIRS}"
        INTERFACE_LINK_LIBRARIES "${OscLib_LIBS}"
    )
    LIST(APPEND CMAKE_INSTALL_RPATH "${OscLib_LIB_DIR}")

  endif()
endif()