add_custom_target(
    SRProxyGen
    DEPENDS ${CMAKE_SOURCE_DIR}/StandardRecord/StandardRecord.h
#    OUTPUT SRProxy.cxx SRProxy.h FwdDeclare.h
    COMMAND gen_srproxy -i StandardRecord/StandardRecord.h -o SRProxy -t caf::StandardRecord --include-path $ENV{CAFANA}/include/ --output-path StandardRecord/Proxy --epilog-fwd ${CMAKE_CURRENT_SOURCE_DIR}/EpilogFwd.h
)

set(Proxy_implementation_files
    SRProxy.cxx
    Instantiations.cxx
    )

set(Proxy_header_files
    SRProxy.h
    FwdDeclare.h
    )

add_library(StandardRecordProxy SHARED ${Proxy_implementation_files})

install(TARGETS StandardRecordProxy DESTINATION lib)

install(FILES ${Proxy_header_files} DESTINATION include/StandardRecord/Proxy)

add_dependencies(StandardRecordProxy SRProxyGen)
