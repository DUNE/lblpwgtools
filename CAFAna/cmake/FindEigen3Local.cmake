include(FindPackageHandleStandardArgs)

if(DEFINED ENV{EIGEN_INC})
  SET(EIGEN_INC $ENV{EIGEN_INC})
endif()

find_path(Eigen_INC_DIR
  NAMES Eigen/Dense
  PATHS ${EIGEN_INC}
  NO_DEFAULT_PATH)

find_package_handle_standard_args(Eigen3Local
	REQUIRED_VARS Eigen_INC_DIR
)

if(Eigen3Local_FOUND)
  cmessage(STATUS "Eigen3")
  cmessage(STATUS "     INC_DIRS: ${Eigen_INC_DIR}")

  if(NOT TARGET Eigen3::Eigen)
    add_library(Eigen3::Eigen INTERFACE IMPORTED)
    set_target_properties(Eigen3::Eigen PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Eigen_INC_DIR}"
    )
  endif()
endif()