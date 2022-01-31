cmessage(STATUS "CMAKE CXX Standard: ${CMAKE_CXX_STANDARD}")

add_compile_options(-Wall -Wextra -Wno-unused-result -Wno-unknown-pragmas -Wno-deprecated-copy)
add_compile_options(-Werror -Wno-delete-non-virtual-dtor)
add_compile_options(-Wno-error=unused-variable -Wno-error=unused-but-set-variable)
add_compile_options(-Wno-unused "-D__FILENAME__=\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"")
#add_compile_options(-DUSE_TH2JAGGED)

# We probably don't need these but they exist in the source
# add_compile_options(-DUSE_FQ_HARDCODED_SYST_PATHS)
# add_compile_options(-DUSE_SRT_CONTEXT)
