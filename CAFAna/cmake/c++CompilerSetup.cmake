cmessage(STATUS "CMAKE CXX Standard: ${CMAKE_CXX_STANDARD}")

add_compile_options(-Wall -Wextra -Wno-unused-result -Wno-unknown-pragmas -Wno-deprecated-copy)
add_compile_options(-Werror -Wno-delete-non-virtual-dtor)
add_compile_options(-Wno-error=unused-variable)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-Wno-unused-parameter)
else()
	add_compile_options(-Wno-unused-but-set-variable)
endif()

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
	add_compile_options(-DDARWINBUILD -fvisibility=default)
	add_link_options(-undefined dynamic_lookup) #So that dylibs from subdirs can depend on each other at runtime
endif()

add_compile_options(-Wno-unused "-D__FILENAME__=\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"")

# We probably don't need these but they exist in the source
# add_compile_options(-DUSE_FQ_HARDCODED_SYST_PATHS)
# add_compile_options(-DUSE_SRT_CONTEXT)
