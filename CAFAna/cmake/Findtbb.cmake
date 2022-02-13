include(FindPackageHandleStandardArgs)

if(DEFINED ENV{TBB_INC})
  SET(TBB_INC $ENV{TBB_INC})
endif()

if(DEFINED ENV{TBB_VERSION})
  SET(TBB_VERSION $ENV{TBB_VERSION})
endif()

if(DEFINED ENV{TBB_LIB})
  SET(TBB_LIB $ENV{TBB_LIB})
endif()

find_path(tbb_INC_DIR
  NAMES tbb/tbb.h
  PATHS ${TBB_INC}
  NO_DEFAULT_PATH)

find_path(tbb_LIB_DIR
  NAMES libtbb.so libtbb.dylib
  PATHS ${TBB_LIB}
  NO_DEFAULT_PATH)

find_package_handle_standard_args(tbb
	REQUIRED_VARS tbb_INC_DIR tbb_LIB_DIR TBB_VERSION
)

if(tbb_FOUND)

  set(TBB_COMPILE_OPTIONS)
  if(TBB_VERSION VERSION_GREATER 2020.0)
      set(TBB_COMPILE_OPTIONS "-DTBB_INTERFACE_NEW")
  endif()

  SET(tbb_LIB_DIRS ${tbb_LIB_DIR})
  LIST(APPEND tbb_LIBS tbb tbbmalloc tbbmalloc_proxy)

  cmessage(STATUS "TBB")
  cmessage(STATUS "     LIB_DIRS: ${tbb_LIB_DIRS}")
  cmessage(STATUS "  TBB_OPTIONS: ${TBB_COMPILE_OPTIONS}")
  cmessage(STATUS "     INC_DIRS: ${tbb_INC_DIR}")
  cmessage(STATUS "         LIBS: ${tbb_LIBS}")

  if(NOT TARGET tbb::All)
    add_library(tbb::All INTERFACE IMPORTED)
    set_target_properties(tbb::All PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${tbb_INC_DIR}"
        INTERFACE_COMPILE_OPTIONS "${TBB_COMPILE_OPTIONS}"
        INTERFACE_LINK_DIRECTORIES "${tbb_LIB_DIRS}"
        INTERFACE_LINK_LIBRARIES "${tbb_LIBS}"
    )
    LIST(APPEND CMAKE_INSTALL_RPATH "${tbb_LIB_DIR}")
  endif()
endif()