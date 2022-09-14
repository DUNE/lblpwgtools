#If we can find it, link it in
find_library(GPERFTOOLS_LIB profiler)
  
if(NOT "${GPERFTOOLS_LIB}x" STREQUAL "GPERFTOOLS_LIB-NOTFOUNDx")
  get_filename_component(GPERFTOOLS_LIB_DIR ${GPERFTOOLS_LIB} DIRECTORY)

  cmessage(STATUS "perftools")
  cmessage(STATUS "    LINK_DIRECTORIES: ${GPERFTOOLS_LIB_DIR}")
  cmessage(STATUS "      LINK_LIBRARIES: -Wl,--no-as-needed;-lprofiler;-Wl,--as-needed")


  if(NOT TARGET perftools::All)
    add_library(perftools::All INTERFACE IMPORTED)
    set_target_properties(perftools::All PROPERTIES
        INTERFACE_LINK_DIRECTORIES ${GPERFTOOLS_LIB_DIR}
        INTERFACE_LINK_LIBRARIES "-Wl,--no-as-needed;-lprofiler;-Wl,--as-needed"
    )
  endif()
endif()

