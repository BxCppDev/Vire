# - CMake build script for Vire CMS/LAPP Plugin

#-----------------------------------------------------------------------
# Copyright 2016 François Mauger <mauger@lpccaen.in2p3.fr>
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

message (STATUS "=====================================================")
message (STATUS "=  C M S / L A P P  I N T E R F A C E  P L U G I N  =")
message (STATUS "=====================================================")

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
set(Vire_CMSLAPPInterface_VERSION_MAJOR 0)
set(Vire_CMSLAPPInterface_VERSION_MINOR 1)
set(Vire_CMSLAPPInterface_VERSION_PATCH 0)
set(Vire_CMSLAPPInterface_VERSION "${Vire_CMSLAPPInterface_VERSION_MAJOR}.${Vire_CMSLAPPInterface_VERSION_MINOR}.${Vire_CMSLAPPInterface_VERSION_PATCH}")

project(Vire_CMSLAPPInterface VERSION ${Vire_CMSLAPPInterface_VERSION})

# - SVN Versioning, if available
set(Vire_CMSLAPPInterface_VERSION_REVISION 0)

# find_package(Vire REQUIRED)
message( STATUS "VIRE_SOURCE_DIR   = '${VIRE_SOURCE_DIR}'")
message( STATUS "VIRE_BUILD_PREFIX = '${VIRE_BUILD_PREFIX}'")

option(VIRE_CMSLAPPINTERFACE_WITH_PROTOBUF_JAVA "Generate Protobuf Java stub files" OFF)

# ----------------
# Source
add_subdirectory(source)

# - end