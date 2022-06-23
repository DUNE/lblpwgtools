include_directories( $ENV{GSL_INC})

cet_find_library( GSL      NAMES gsl      PATHS ENV GSL_LIB NO_DEFAULT_PATH )
cet_find_library( GSLCBLAS NAMES gslcblas PATHS ENV GSL_LIB NO_DEFAULT_PATH )

art_make( BASENAME_ONLY
	         LIBRARY_NAME  OscLibFunc
          LIB_LIBRARIES ${ROOT_BASIC_LIB_LIST}
   			                  ${GSL}
			                     ${GSLCBLAS}
        )

install_headers()
install_source()
