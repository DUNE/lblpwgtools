LIST(APPEND EXTRA_LIBS ${ROOT_LIBS})

include(${CMAKE_SOURCE_DIR}/cmake/gperftools.cmake)

set(CXX_WARNINGS -Wall -Wextra -Wno-unused-result -Wno-unknown-pragmas)

LIST(APPEND EXTRA_CXX_FLAGS ${CXX_WARNINGS} -Werror -Wno-delete-non-virtual-dtor -Wno-unused "-D__FILENAME__=\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"")

if(KNL)
  LIST(APPEND EXTRA_CXX_FLAGS -march=knl)
endif()

LIST(APPEND EXTRA_LIBS dl)
if(DEFINED USE_OPENMP AND USE_OPENMP)
  LIST(APPEND EXTRA_LIBS -lgomp)
endif()

list(REMOVE_DUPLICATES EXTRA_LINK_DIRS)
list(REMOVE_DUPLICATES EXTRA_LIBS)
list(REMOVE_DUPLICATES EXTRA_CXX_FLAGS)

BuildFlagString(CAFANA_LINK_DIRS "-L" ${EXTRA_LINK_DIRS})

BuildLibraryFlagString(STR_EXTRA_LIBS ${EXTRA_LIBS})
BuildFlagString(STR_EXTRA_SHAREDOBJS " " ${EXTRA_SHAREDOBJS})

#This ends up holding all of the libraries and search paths for extenal dependencies
CatStringsIfNotEmpty(CAFANA_DEPEND_LIBS
  ${STR_EXTRA_SHAREDOBJS}
  ${STR_EXTRA_LIBS})

BuildFlagString(STR_EXTRA_LINK_FLAGS " " ${EXTRA_LINK_FLAGS})

CatStringsIfNotEmpty(CMAKE_LINK_FLAGS
  ${CMAKE_LINK_FLAGS}
  ${STR_EXTRA_LINK_FLAGS})

get_directory_property(CAFANA_INCLUDE_DIRS INCLUDE_DIRECTORIES)

BuildFlagString(CAFANA_CXX_FLAGS " " ${EXTRA_CXX_FLAGS})
CatStringsIfNotEmpty(CAFANA_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CAFANA_CXX_FLAGS} )

if (VERBOSE)
  cmessage (STATUS "C++ Compiler      : ${CXX_COMPILER_NAME}")
  cmessage (STATUS "    Flags         : ${CAFANA_CXX_FLAGS}")
  cmessage (STATUS "    Release Flags : ${CMAKE_CXX_FLAGS_RELEASE}")
  cmessage (STATUS "    Debug Flags   : ${CMAKE_CXX_FLAGS_DEBUG}")
  cmessage (STATUS "    RelDeb Flags  : ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  cmessage (STATUS "    Include Dirs  : ${CAFANA_INCLUDE_DIRS}")
  cmessage (STATUS "    Linker Flags  : ${CMAKE_LINK_FLAGS}")
  cmessage (STATUS "    Link Dirs     : ${CAFANA_LINK_DIRS}")
  cmessage (STATUS "    Lib Flags     : ${CAFANA_DEPEND_LIBS}")
endif()

add_compile_options(${EXTRA_CXX_FLAGS})
