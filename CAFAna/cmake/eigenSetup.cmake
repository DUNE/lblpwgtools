if(DEFINED USE_EIGEN AND USE_EIGEN)

  if(DEFINED ENV{EIGEN_INC} AND EXISTS $ENV{EIGEN_INC})
	set(EIGEN_INC $ENV{EIGEN_INC})
  endif()

  if(DEFINED {EIGEN_INC} AND EXISTS ${EIGEN_INC})
  	include_directories(${EIGEN_INC})
  else()
      
  	include(ExternalProject)

  	ExternalProject_Add(eigen
    		PREFIX "${CMAKE_BINARY_DIR}/Ext"
   		GIT_REPOSITORY https://github.com/eigenteam/eigen-git-mirror.git
    		GIT_TAG 3.3.5
		UPDATE_DISCONNECTED 1
    		CONFIGURE_COMMAND ""
    		UPDATE_COMMAND ""
    		BUILD_COMMAND ""
    		INSTALL_COMMAND "")

  	include_directories(${CMAKE_BINARY_DIR}/Ext/src/eigen/)
  endif()

  LIST(APPEND EXTRA_CXX_FLAGS -DUSE_EIGEN)

endif()
