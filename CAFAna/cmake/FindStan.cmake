include(CMakeFindDependencyMacro)

#This isn't Eigen3 because the UPS product for Eigen doesn't provide a useable Eigen3Config.cmake
#so we sub our own
find_dependency(Eigen3Local REQUIRED)
find_dependency(Sundials REQUIRED)
find_dependency(tbb REQUIRED)

include(FindPackageHandleStandardArgs)

if(DEFINED ENV{STAN_INC})
  SET(STAN_INC $ENV{STAN_INC})
endif()

if(DEFINED ENV{STAN_MATH_INC})
  SET(STAN_MATH_INC $ENV{STAN_MATH_INC})
endif()

find_path(Stan_INC_DIR
  NAMES stan/version.hpp
  PATHS ${STAN_INC})

find_path(Stan_Math_INC_DIR
  NAMES stan/math.hpp
  PATHS ${STAN_MATH_INC})

find_package_handle_standard_args(Stan
	REQUIRED_VARS Stan_INC_DIR Stan_Math_INC_DIR
)

if(Stan_FOUND)

  cmessage(STATUS "Stan-Math")
  cmessage(STATUS "     INC_DIR: ${Stan_Math_INC_DIR}")

  if(NOT TARGET Stan::Math)
    add_library(Stan::Math INTERFACE IMPORTED)
    set_target_properties(Stan::Math PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Stan_Math_INC_DIR}"
    )
  endif()
  LIST(APPEND STAN_DEPENDENCIES Stan::Math Sundials::All Eigen3::Eigen tbb::All)

  cmessage(STATUS "Stan")
  cmessage(STATUS "     INC_DIR: ${Stan_INC_DIR}")
  cmessage(STATUS "Dependencies: ${STAN_DEPENDENCIES}")

  if(NOT TARGET Stan::All)
    add_library(Stan::All INTERFACE IMPORTED)
    set_target_properties(Stan::All PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Stan_INC_DIR}"
        INTERFACE_LINK_LIBRARIES "${STAN_DEPENDENCIES}"
    )
  endif()
endif()