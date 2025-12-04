# this might be nested inside my fnal-personal project
if (NOT DEFINED CMAKE_MINIMUM_REQUIRED_VERSION)
	cmake_minimum_required(VERSION 3.22)
endif()

if (NOT DEFINED PROJECT_NAME)
	project(lbl-cafana-revival  LANGUAGES CXX)
endif()

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
if(DUNEPlotStyle_FOUND)
	add_compile_options(-DLBL2025_USE_DUNESTYLE)
endif()

set(LBL_CAFANA_USE_STAN True)
if ("$ENV{CAFANA_USE_STAN}" STREQUAL "0")
	message(WARNING "CAFAna was built with Stan support disabled.  Propagating that to LBL...")
	set(LBL_CAFANA_USE_STAN False)
elseif (DEFINED ENV{CAFANA_USE_STAN})
	message(WARNING "\$CAFANA_USE_STAN has value '$ENV{CAFANA_USE_STAN}', which I interpret to mean I *should* depend on Stan.  Here goes...")
endif()

set(LBL_CAFANA_INCDIRS
	# CAFAna itself
	$ENV{CAFANA}/include

	# CAFAna's dependencies
	$ENV{ROOT_INC}
	$ENV{BOOST_INC}
	$ENV{EIGEN_INC}
	$ENV{OSCLIB_INC}
	$ENV{SRPROXY_INC}
	$ENV{CAFANACORE_INC}
	$ENV{DUNEANAOBJ_INC}

	${CMAKE_CURRENT_SOURCE_DIR}
)
if(LBL_CAFANA_USE_STAN)
	set(LBL_CAFANA_INCDIRS ${LBL_CAFANA_INCDIRS}
		$ENV{SUNDIALS_INC}
		$ENV{STAN_MATH_INC}
		$ENV{STAN_INC}
		$ENV{TBB_INC}
	)
endif()


# otherwise we will get errors about TBB
add_compile_options(-DTBB_INTERFACE_NEW)
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed" )

if(NOT LBL_CAFANA_USE_STAN)
	remove_definitions(-DCAFANA_USE_STAN)
endif()

include_directories(${LBL_CAFANA_INCDIRS})

#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#	message(STATUS " with incdir: '${dir}'")
#endforeach()

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
			Boost::filesystem
			CAFAna::All
			-Wl,--no-as-needed
	)
	if(LBL_CAFANA_USE_STAN)
		target_link_libraries(${tgt} Stan::All)
	endif()
endfunction()


add_subdirectory(lbl2025)
add_subdirectory(progs)