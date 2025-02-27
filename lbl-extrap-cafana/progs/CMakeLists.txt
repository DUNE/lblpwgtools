include_directories(${LBL_CAFANA_INCDIRS})

list(APPEND MACROS_TO_BUILD
		Test_Spectrum.C
)

# use external argparse
include(FetchContent)
FetchContent_Declare(
		argparse
		GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)


foreach (macro IN LISTS MACROS_TO_BUILD)
	set_source_files_properties(${macro} PROPERTIES LANGUAGE CXX)
	get_filename_component(exc ${macro} NAME_WE)
	add_executable(${exc} ${macro})
	set_target_properties(${exc} PROPERTIES LINKER_LANGUAGE CXX)
	target_link_libraries(${exc} lbl2025 argparse)
	link_all_libs(${exc})
	install(TARGETS ${exc})
endforeach ()