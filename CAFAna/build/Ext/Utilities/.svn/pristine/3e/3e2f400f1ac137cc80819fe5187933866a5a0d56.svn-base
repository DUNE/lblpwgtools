include_directories( $ENV{SQLITE_FQ_DIR}/include/ )
include_directories( $ENV{ART_DIR}/source )

art_make_exec(NAME stripParameterSets
              SOURCE stripParameterSets.cc                 
		            LIBRARIES ${SQLITE3}
              ${ART_PERSISTENCY_ROOTDB}
			           ${ROOT_BASIC_LIB_LIST}
             )

install_source()
