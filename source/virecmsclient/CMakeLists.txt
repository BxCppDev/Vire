# - CMake build script for Vire CMS Client application

#-----------------------------------------------------------------------
# Copyright 2017 François Mauger <mauger@lpccaen.in2p3.fr>
#
# This file is part of Vire.
#
# Vire is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Vire is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Vire. If not, see <http://www.gnu.org/licenses/>.
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Configure application
#find_package(Boost REQUIRED program_options filesystem system log)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
#include_directories(${Readline_INCLUDE_DIR})
include_directories(${BxJsontools_INCLUDE_DIRS})
#include_directories(${BxRabbitMQ_INCLUDE_DIRS})
include_directories(${BxProtobuftools_INCLUDE_DIRS})
#include_directories(${PROTOBUF_INCLUDE_DIRS})
#include_directories(${VIRE_BUILD_PREFIX}/include/protobuf/cpp)
#include_directories(${VIRE_BUILD_PREFIX}/include)

#-----------------------------------------------------------------------
# Build command line app
#
# - Generate application configuration file
# configure_file(virecmsclient_config.h.in virecmsclient_config.h @ONLY)

set(VireCmsClientApp_HEADERS
  virecmsclient/core.h
  virecmsclient/parameters.h
  virecmsclient/command_line.h
)

set(VireCmsClientApp_PRIVATE_HEADERS
)

set(VireCmsClientApp_SOURCES
  virecmsclient/core.cc
  virecmsclient/parameters.cc
  virecmsclient/command_line.cc
  virecmsclientmain.cxx
)

add_executable(virecmsclient
  ${VireCmsClientApp_HEADERS}
  ${VireCmsClientApp_PRIVATE_HEADERS}
  ${VireCmsClientApp_SOURCES}
  )
target_link_libraries(virecmsclient
  Vire
  )
# - RPath it
set_target_properties(virecmsclient PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

if(UNIX AND NOT APPLE)
  set_target_properties(virecmsclient
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(APPLE)
  # Temporary setting - needs testing
  set_target_properties(virecmsclient
    PROPERTIES
      INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}"
      LINK_FLAGS "-Wl,-rpath,@loader_path/../${CMAKE_INSTALL_LIBDIR}"
    )
endif()

# - Install
# TODO : fixup rpaths as needed
install(TARGETS virecmsclient
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

#-----------------------------------------------------------------------
