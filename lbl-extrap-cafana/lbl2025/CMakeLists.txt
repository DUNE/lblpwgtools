message(STATUS " subdir lbl2025: $PROJECT_NAME = ${PROJECT_NAME}")

include_directories(${LBL_CAFANA_INCDIRS})

set(headers
	Axes.h
	Cuts.h
	Loaders.h
	Vars.h
)

set(sources
	Axes.cxx
	Cuts.cxx
	Loaders.cxx
	Vars.cxx
)

add_library(lbl2025 SHARED ${sources})
set_target_properties(lbl2025 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(lbl2025 PUBLIC ${LBL_CAFANA_INCDIRS})
set_target_properties(lbl2025 PROPERTIES PUBLIC_HEADER "${headers}")
#target_link_libraries(lbl2025 CAFAna::All)
link_all_libs(lbl2025)

install(TARGETS lbl2025)