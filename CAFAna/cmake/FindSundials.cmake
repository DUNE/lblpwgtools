include(FindPackageHandleStandardArgs)

if(DEFINED ENV{SUNDIALS_INC})
  SET(SUNDIALS_INC $ENV{SUNDIALS_INC})
endif()

if(DEFINED ENV{SUNDIALS_LIB})
  SET(SUNDIALS_LIB $ENV{SUNDIALS_LIB})
endif()

find_path(Sundials_INC_DIR
  NAMES sundials/sundials_version.h
  PATHS ${SUNDIALS_INC}
  NO_DEFAULT_PATH)

find_path(Sundials_LIB_DIR
  NAMES libsundials_cvodes.so
  PATHS ${SUNDIALS_LIB}
  NO_DEFAULT_PATH)

find_package_handle_standard_args(Sundials
	REQUIRED_VARS Sundials_INC_DIR Sundials_LIB_DIR
)

if(Sundials_FOUND)

  SET(Sundials_LIB_DIRS ${Sundials_LIB_DIR})
  LIST(APPEND Sundials_LIBS 
    sundials_cvodes
    sundials_idas
    sundials_kinsol
    sundials_nvecserial)

  cmessage(STATUS "Sundials")
  cmessage(STATUS "     LIB_DIRS: ${Sundials_LIB_DIRS}")
  cmessage(STATUS "     INC_DIRS: ${Sundials_INC_DIR}")
  cmessage(STATUS "         LIBS: ${Sundials_LIBS}")

  if(NOT TARGET Sundials::All)
    add_library(Sundials::All INTERFACE IMPORTED)
    set_target_properties(Sundials::All PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Sundials_INC_DIR}"
        INTERFACE_LINK_DIRECTORIES "${Sundials_LIB_DIRS}"
        INTERFACE_LINK_LIBRARIES "${Sundials_LIBS}"
    )
  endif()
endif()