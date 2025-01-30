
art_make( BASENAME_ONLY
	  LIBRARY_NAME      Utilities
	  LIB_LIBRARIES     UtilitiesFunc
	                    RawData
			    CMap_service
			    GeometryObjects
			    Geometry_service
	  SERVICE_LIBRARIES Utilities
	                    UtilitiesFunc
			    CMap_service
			    ${ART_FRAMEWORK_CORE}
			    ${ART_FRAMEWORK_PRINCIPAL}
			    ${ART_FRAMEWORK_SERVICES_REGISTRY}
			    ${ART_FRAMEWORK_SERVICES_BASIC}
			    ${ART_FRAMEWORK_SERVICES_OPTIONAL}
			    ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
			    art_Persistency_Common  
			    art_Utilities canvas
			    ${MF_MESSAGELOGGER}
			    ${MF_UTILITIES}
			    ${FHICLCPP}
			    cetlib cetlib_except
	  MODULE_LIBRARIES  Utilities
	                    UtilitiesFunc
			    CMap_service
			    NovaTimingUtilities
			    Geometry_service
			    LiveGeometry_service
			    RecoBase
			    Metadata
			    ${ART_FRAMEWORK_CORE}
			    ${ART_FRAMEWORK_PRINCIPAL}
			    ${ART_FRAMEWORK_SERVICES_REGISTRY}
			    ${ART_FRAMEWORK_SERVICES_BASIC}
			    ${ART_FRAMEWORK_SERVICES_OPTIONAL}
			    ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
			    art_Persistency_Common  
			    art_Utilities canvas
			    ${MF_MESSAGELOGGER}
			    ${MF_UTILITIES}
			    ${FHICLCPP}
			    cetlib cetlib_except
	  EXCLUDE           rootlogon.C RemoveBeamSpills_module.cc RemoveBeamSpillsPrecise_module.cc
)


if( DEFINED ENV{IFDH_ART_FQ_DIR} )

 include_directories( $ENV{IFBEAM_FQ_DIR}/include )
 include_directories( $ENV{IFDH_ART_FQ_DIR}/inc   )
 include_directories( $ENV{IFDHC_FQ_DIR}/inc      )
 include_directories( $ENV{LIBWDA_FQ_DIR}/include )

 cet_find_library( IFBEAMSERV NAMES IFBeam_service PATHS ENV IFDH_ART_LIB NO_DEFAULT_PATH )

 simple_plugin(RemoveBeamSpills "module"
               Utilities
               UtilitiesFunc
	             ${IFBEAMSERV}
               ifbeam
	             NovaTimingUtilities
               ${ART_FRAMEWORK_CORE}
	             ${ART_FRAMEWORK_PRINCIPAL}
               ${ART_FRAMEWORK_SERVICES_REGISTRY}
               ${ART_FRAMEWORK_SERVICES_BASIC}
               ${ART_FRAMEWORK_SERVICES_OPTIONAL}
               ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
               art_Persistency_Common  
               art_Utilities canvas
               ${MF_MESSAGELOGGER}
               ${MF_UTILITIES}
               ${FHICLCPP}
               cetlib cetlib_except
               BASENAME_ONLY
             )               

 simple_plugin(RemoveBeamSpillsPrecise "module"
               Utilities
	             UtilitiesFunc
	             ${IFBEAMSERV}
               ifbeam
	             NovaTimingUtilities
               ${ART_FRAMEWORK_CORE}
	             ${ART_FRAMEWORK_PRINCIPAL}
               ${ART_FRAMEWORK_SERVICES_REGISTRY}
               ${ART_FRAMEWORK_SERVICES_BASIC}
               ${ART_FRAMEWORK_SERVICES_OPTIONAL}
               ${ART_FRAMEWORK_SERVICES_OPTIONAL_TFILESERVICE_SERVICE}
               art_Persistency_Common  
               art_Utilities canvas
               ${MF_MESSAGELOGGER}
               ${MF_UTILITIES}
               ${FHICLCPP}
               cetlib cetlib_except
               BASENAME_ONLY
             )               

endif()

add_subdirectory(batch)
add_subdirectory(eventCount)
add_subdirectory(func)
add_subdirectory(strip)

install_headers()
install_fhicl()
install_source()
install_scripts()
