# - CMake build script for Vire MOS Plugin

message (STATUS "========================")
message (STATUS "=  M O S  P L U G I N  =")
message (STATUS "========================")

#-----------------------------------------------------------------------
# Copyright 2015 François Mauger <mauger@lpccaen.in2p3.fr>
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

# -----------------------------------------------------------------------
# Enforce an out-of-source build.
#
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(STATUS "This project requires an out-of-source build.")
  message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
  message(STATUS "  CMakeCache.txt")
  message(STATUS "  CMakeFiles")
  message(STATUS "Once these files are removed, create a separate directory")
  message(STATUS "and run CMake from there, pointing it to:")
  message(STATUS "  ${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "in-source build detected")
endif()
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

# - Versioning
set(Vire_MOS_VERSION_MAJOR 0)
set(Vire_MOS_VERSION_MINOR 1)
set(Vire_MOS_VERSION_PATCH 0)
set(Vire_MOS_VERSION "${Vire_MOS_VERSION_MAJOR}.${Vire_MOS_VERSION_MINOR}.${Vire_MOS_VERSION_PATCH}")
project(Vire_MOS_Plugin VERSION ${Vire_MOS_VERSION})

# - SVN Versioning, if available
set(Vire_MOS_VERSION_REVISION 0)

# find_package(Vire REQUIRED)

# ----------------
# Source
add_subdirectory(source)

# - end