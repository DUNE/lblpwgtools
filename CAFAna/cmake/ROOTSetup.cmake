# Copyright 2018 L. Pickering, P Stowell, R. Terri, C. Wilkinson, C. Wret

################################################################################
#    This file is part of NUISANCE.
#
#    NUISANCE is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    NUISANCE is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with NUISANCE.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

if (NOT DEFINED ENV{ROOTSYS})
  cmessage (FATAL_ERROR "$ROOTSYS is not defined, please set up ROOT first.")
else()
  cmessage(STATUS "Using ROOT installed at $ENV{ROOTSYS}")
  set(CMAKE_ROOTSYS $ENV{ROOTSYS})
endif()

# Get cflags from ROOT
execute_process (COMMAND root-config
  --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS_RAW OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REPLACE " " ";" ROOT_CXX_FLAGS "${ROOT_CXX_FLAGS_RAW}")

#Check what ROOT thinks the standard is, set that project-wide
# and then remove it from ROOT_CXX_FLAGS
list (FIND ROOT_CXX_FLAGS "-std=c++11" CPP11_INDEX)
list (FIND ROOT_CXX_FLAGS "-std=c++14" CPP14_INDEX)
list (FIND ROOT_CXX_FLAGS "-std=c++17" CPP17_INDEX)
list (FIND ROOT_CXX_FLAGS "-std=c++20" CPP20_INDEX)

if (${CPP11_INDEX} GREATER -1)
  SET(CMAKE_CXX_STANDARD 11)
elseif (${CPP14_INDEX} GREATER -1)
  SET(CMAKE_CXX_STANDARD 14)
elseif (${CPP17_INDEX} GREATER -1)
  SET(CMAKE_CXX_STANDARD 17)
elseif (${CPP20_INDEX} GREATER -1)
  SET(CMAKE_CXX_STANDARD 20)
endif()
list(REMOVE_ITEM ROOT_CXX_FLAGS "-std=c++11")
list(REMOVE_ITEM ROOT_CXX_FLAGS "-std=c++14")
list(REMOVE_ITEM ROOT_CXX_FLAGS "-std=c++17")
list(REMOVE_ITEM ROOT_CXX_FLAGS "-std=c++20")

LIST(APPEND EXTRA_CXX_FLAGS ${ROOT_CXX_FLAGS})

# Get libdir from ROOT
execute_process (COMMAND root-config
  --libdir OUTPUT_VARIABLE ROOT_LIBDIR OUTPUT_STRIP_TRAILING_WHITESPACE)
# Get version from ROOT
execute_process (COMMAND root-config
  --version OUTPUT_VARIABLE ROOT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
# Get features from ROOT
execute_process (COMMAND root-config
  --features OUTPUT_VARIABLE ROOT_FEATURES OUTPUT_STRIP_TRAILING_WHITESPACE)

LIST(APPEND EXTRA_LINK_DIRS ${ROOT_LIBDIR})

LIST(APPEND ROOT_LIBS
  -Wl,--no-as-needed
  Core
  RIO
  Net
  Hist
  Graf
  Graf3d
  Gpad
  Tree
  Rint
  Postscript
  Matrix
  Physics
  MathCore)

if("${ROOT_FEATURES}" MATCHES "minuit2")
  cmessage(STATUS "ROOT built with MINUIT2 support")
  LIST(APPEND ROOT_LIBS Minuit2)
else()
  cmessage(FATAL_ERROR "ROOT built without MINUIT2 support but minimizer functionality requested. Either configure with -DUSE_MINIMIZER=FALSE or use a version of ROOT with MINUIT2 support.")
endif()

cmessage ( STATUS "[ROOT]: root-config --version: ${ROOT_VERSION} ")
cmessage ( STATUS "[ROOT]: root-config --cflags : ${ROOT_CXX_FLAGS} ")
cmessage ( STATUS "[ROOT]: root-config --libdir : ${ROOT_LIBDIR} ")
cmessage ( STATUS "[ROOT]: Libs                 : ${ROOT_LIBS} ")
