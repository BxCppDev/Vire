# - CMake build script for Vire CMS Server application

#-----------------------------------------------------------------------
# Copyright 2015-2017 François Mauger <mauger@lpccaen.in2p3.fr>
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
find_package(Boost REQUIRED program_options filesystem system log)

message (STATUS "[info] virecmsserver: BxJsontools_INCLUDE_DIRS=${BxJsontools_INCLUDE_DIRS}")
include_directories(${Readline_INCLUDE_DIR})
include_directories(${BxJsontools_INCLUDE_DIRS})
include_directories(${BxRabbitMQ_INCLUDE_DIRS})
include_directories(${BxProtobuftools_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(BEFORE ${Bayeux_INCLUDE_DIRS})
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})

#-----------------------------------------------------------------------
# Build command line app
#
# - Generate application configuration file
configure_file(virecmsserver/virecmsserver_config.h.in virecmsserver/virecmsserver_config.h @ONLY)

set(VireCmsServerApp_HEADERS
  virecmsserver/ui/shell.h
  virecmsserver/ui/server_print_command.h
  virecmsserver/ui/server_stop_command.h
  virecmsserver/ui/server_uptime_command.h
  virecmsserver/ui/server_command_interface.h
  virecmsserver/virecmsserver_config.h
)

set(VireCmsServerApp_PRIVATE_HEADERS
)

set(VireCmsServerApp_SOURCES
  virecmsserver/ui/shell.cc
  virecmsserver/ui/server_print_command.cc
  virecmsserver/ui/server_stop_command.cc
  virecmsserver/ui/server_uptime_command.cc
  virecmsserver/ui/server_command_interface.cc
  virecmsservermain.cxx
)

add_executable(virecmsserver
  ${VireCmsServerApp_HEADERS}
  ${VireCmsServerApp_PRIVATE_HEADERS}
  ${VireCmsServerApp_SOURCES}
  )
target_link_libraries(virecmsserver
  Vire
  ${Bayeux_LIBRARIES}
  ${Boost_LIBRARIES}
  )
# - RPath it
set_target_properties(virecmsserver PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

if(UNIX AND NOT APPLE)
  set_target_properties(virecmsserver
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(APPLE)
  # Temporary setting - needs testing
  set_target_properties(virecmsserver
    PROPERTIES
      INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}"
      LINK_FLAGS "-Wl,-rpath,@loader_path/../${CMAKE_INSTALL_LIBDIR}"
    )
endif()

# - Install
# TODO : fixup rpaths as needed
install(TARGETS virecmsserver
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

#-----------------------------------------------------------------------
