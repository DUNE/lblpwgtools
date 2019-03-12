if(DEFINED USE_EIGEN AND USE_EIGEN)
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

  LIST(APPEND EXTRA_CXX_FLAGS -DUSE_EIGEN)

  include_directories(${CMAKE_BINARY_DIR}/Ext/src/eigen/)
endif()
