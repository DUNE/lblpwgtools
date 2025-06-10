cmake_minimum_required(VERSION 3.22)
project(cafana-demo  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_link_options("-Wl,--no-undefined")

if (NOT DEFINED ENV{CAFANA})
    message(FATAL_ERROR "$CAFANA is not defined.  You won't be able to build these CAFAna analysis files without it!")
endif()

#------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "$ENV{CAFANA}/cmake")
include($ENV{CAFANA}/cmake/cmessage.cmake)
include($ENV{CAFANA}/cmake/FindExternals.cmake)

find_package(CAFAna REQUIRED)
if (NOT CAFAna_FOUND)
    message(FATAL_ERROR "Could not find CAFAna!")
endif()

find_package(DUNEPlotStyle)

set(LBL_CAFANA_INCDIRS
        # CAFAna itself
        $ENV{CAFANA}/include

        # CAFAna's dependencies
        $ENV{ROOT_INC}
        $ENV{BOOST_INC}
        $ENV{EIGEN_INC}
        $ENV{SUNDIALS_INC}
        $ENV{STAN_MATH_INC}
        $ENV{STAN_INC}
        $ENV{TBB_INC}
        $ENV{OSCLIB_INC}
        $ENV{SRPROXY_INC}
        $ENV{CAFANACORE_INC}
        $ENV{DUNEANAOBJ_INC}

        ${CMAKE_CURRENT_SOURCE_DIR}
)

# otherwise we will get errors about TBB
add_compile_options(-DTBB_INTERFACE_NEW)

include_directories(${LBL_CAFANA_INCDIRS})

find_library(CAFANACOREEXT CAFAnaCoreExt $ENV{CAFANACORE_LIB})
find_library(STANDARDRECORD duneanaobj_StandardRecord $ENV{DUNEANAOBJ_LIB})
find_library(STANDARDRECORDPROXY duneanaobj_StandardRecordProxy $ENV{DUNEANAOBJ_LIB})

#------------------------------------------------------------------------

# this is extremely lazy, but probably doesn't hurt anything.
# just link *all* the CAFAna dependencies and all the CAFAna libraries themselves
function(link_all_libs tgt)
    target_link_libraries(${tgt}
            ${STANDARDRECORDPROXY}
            CAFAnaCore::Ext
            OscLib::All
            ROOT::ROOT
            Stan::All
            Boost::filesystem
            CAFAna::All
    )
endfunction()

#------------------------------------------------------------------------

# if you want to organize your project into subdirectories,
# you could move the following into a CMakeLists.txt in a subdir
# and then use add_subdirectory() here to pass it to CMake

list(APPEND MACROS_TO_BUILD
	demo_cutsvars.C
)

foreach (macro IN LISTS MACROS_TO_BUILD)
	set_source_files_properties(${macro} PROPERTIES LANGUAGE CXX)
	get_filename_component(exc ${macro} NAME_WE)
	add_executable(${exc} ${macro})
	set_target_properties(${exc} PROPERTIES LINKER_LANGUAGE CXX)
	target_link_libraries(${exc} lbl2025 argparse)
	link_all_libs(${exc})
	install(TARGETS ${exc})
endforeach ()