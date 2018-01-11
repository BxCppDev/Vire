# - CMake build script for Vire Inspector application

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
configure_file(vire/inspector/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/vire/inspector/config.h @ONLY)

set(VireInspectorApp_HEADERS
  ${CMAKE_CURRENT_BINARY_DIR}/vire/inspector/config.h
  vire/inspector/ihs_handler.h
  vire/inspector/shell_print_device_command.h
  vire/inspector/shell_find_devices_command.h
  vire/inspector/inspector_shell.h
  vire/inspector/inspector_shell_command_interface.h
  vireinspector.h
)

set(VireInspectorApp_PRIVATE_HEADERS
)

set(VireInspectorApp_SOURCES
  vireinspectormain.cxx
  vireinspector.cc
  vire/inspector/ihs_handler.cc
  vire/inspector/shell_print_device_command.cc
  vire/inspector/shell_find_devices_command.cc
  vire/inspector/inspector_shell.cc
  vire/inspector/inspector_shell_command_interface.cc
)

add_executable(vireinspector
  ${VireInspectorApp_HEADERS}
  ${VireInspectorApp_PRIVATE_HEADERS}
  ${VireInspectorApp_SOURCES}
  )
target_link_libraries(vireinspector
  Vire
  ${Bayeux_LIBRARIES}
  ${Boost_LIBRARIES}
  )
# - RPath it
set_target_properties(vireinspector PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

if(UNIX AND NOT APPLE)
  set_target_properties(vireinspector
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(APPLE)
  # Temporary setting - needs testing
  set_target_properties(vireinspector
    PROPERTIES
      INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}"
      LINK_FLAGS "-Wl,-rpath,@loader_path/../${CMAKE_INSTALL_LIBDIR}"
    )
endif()

# - Install
# TODO : fixup rpaths as needed
install(TARGETS vireinspector
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

#-----------------------------------------------------------------------
