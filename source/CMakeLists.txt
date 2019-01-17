# - CMake script for Vire Library and Apps
#
#-----------------------------------------------------------------------
# Copyright 2015-2017, François Mauger <mauger@lpccaen.in2p3.fr>
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
message (STATUS "[info] Begin of 'source/CMakeLists.txt'.")

#-----------------------------------------------------------------------
# Core configuration
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${VIRE_BUILD_PREFIX}/include)
include_directories(${VIRE_BUILD_PREFIX}/include/vire)
#
#-----------------------------------------------------------------------
# Libraries and apps
#

#-----------------------------------------------------------------------
# Main Vire library + protobuf support:
message( STATUS "[info] Adding subdirectory 'vire'...")
add_subdirectory(vire)

#-----------------------------------------------------------------------
# Apps
message( STATUS "[info] Adding subdirectory 'virequery'...")
add_subdirectory(virequery)
add_subdirectory(vireinspector)
add_subdirectory(virecmsserver)
add_subdirectory(virecmsclient)

#-----------------------------------------------------------------------
# Java
if (VIRE_WITH_JAVA)
  message( STATUS "[info] Adding subdirectory 'java'...")
  add_subdirectory(java)
endif()

#-----------------------------------------------------------------------
# Manual
#
if(Pandoc_FOUND)
  set(VIRE_MANPAGE_IN  ${CMAKE_CURRENT_SOURCE_DIR}/Vire.1.md)
  set(VIRE_MANPAGE_OUT ${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/${VIRE_TAG}/man/man1/Vire.1)

  add_custom_command(OUTPUT ${VIRE_MANPAGE_OUT}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/${VIRE_TAG}/man/man1
    COMMAND ${Pandoc_EXECUTABLE} -s -w man ${VIRE_MANPAGE_IN} -o ${VIRE_MANPAGE_OUT}
    COMMENT "Generating Vire.1 man page"
    DEPENDS ${VIRE_MANPAGE_IN}
    )
  add_custom_target(vire_man ALL DEPENDS ${VIRE_MANPAGE_OUT})
endif()

message (STATUS "[info] End of 'source/CMakeLists.txt'.")

#- end
