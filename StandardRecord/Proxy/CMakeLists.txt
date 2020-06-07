add_custom_command(
    DEPENDS ../StandardRecord.h Prolog.h EpilogFwd.h WgtMapExtra.h WgtMapElemExtra.h CVMapExtra.h
    OUTPUT SRProxy.cxx SRProxy.h FwdDeclare.h
    COMMAND gen_srproxy -i StandardRecord/StandardRecord.h -o SRProxy -t caf::StandardRecord --include-path ${CMAKE_CURRENT_SOURCE_DIR}/../.. --output-path StandardRecord/Proxy --prolog ${CMAKE_CURRENT_SOURCE_DIR}/Prolog.h --epilog-fwd ${CMAKE_CURRENT_SOURCE_DIR}/EpilogFwd.h --extra WgtMap ${CMAKE_CURRENT_SOURCE_DIR}/WgtMapExtra.h --extra WgtMapElem ${CMAKE_CURRENT_SOURCE_DIR}/WgtMapElemExtra.h --extra CVMap ${CMAKE_CURRENT_SOURCE_DIR}/CVMapExtra.h
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
