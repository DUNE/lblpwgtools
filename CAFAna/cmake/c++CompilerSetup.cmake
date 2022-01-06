include(${CMAKE_SOURCE_DIR}/cmake/gperftools.cmake)

cmessage(STATUS "CMAKE CXX Standard: ${CMAKE_CXX_STANDARD}")

add_compile_options(-Werror -Wall)
add_compile_options(-Wno-error=unused-variable -Wno-error=unused-but-set-variable)