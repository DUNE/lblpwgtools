
SET(ENV{BOOST_LIB} "$ENV{BOOST_FQ_DIR}/lib")
cet_find_library( BOOST_FILESYSTEM NAMES boost_filesystem PATHS ENV BOOST_LIB NO_DEFAULT_PATH)
cet_find_library( BOOST_IOSTREAMS  NAMES boost_iostreams  PATHS ENV BOOST_LIB NO_DEFAULT_PATH)

art_make_exec(NAME      eventCount
              SOURCE    eventCount.cc Arguments.cc FileInfo.cc
              LIBRARIES ${BOOST_FILESYSTEM}
                        ${BOOST_IOSTREAMS}
                        ${ROOT_BASIC_LIB_LIST}
)

install_source()
install_headers()