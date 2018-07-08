# - CMake script for Vire Plugins
#
#-----------------------------------------------------------------------
# Copyright 2015-2016, François Mauger <mauger@lpccaen.in2p3.fr>
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

message (STATUS "===================")
message (STATUS "=  P L U G I N S  =")
message (STATUS "===================")

#-----------------------------------------------------------------------
# - List of built plugins:
set(VIRE_PLUGINS
  Vire_MOS              # The Vire to Multipurpose OPCUA Server (MOS) plugin and utilities
  Vire_CMSLAPPInterface # The Vire CMS/LAPP interface plugin
)

#-----------------------------------------------------------------------
# Plugins are compiled code, so need to be installed under libdir...
#
message(STATUS "[devel] VIRE_LIBDIR_TO_PLUGINDIR = '${VIRE_LIBDIR_TO_PLUGINDIR}'")
set(VIRE_PLUGINLIBDIR ${VIRE_LIBDIR_TO_PLUGINDIR})
message(STATUS "[devel] VIRE_PLUGINLIBDIR = '${VIRE_PLUGINLIBDIR}'")

# - Local setting of output path
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${VIRE_LIBDIR_TO_PLUGINDIR}")

# - To allow plugins to be developed independently, point
# them to the current Vire
set(Vire_DIR "${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_CMAKEDIR}/${VIRE_TAG}")

# - List of plugins:
set(_vire_PLUGINS ${VIRE_PLUGINS})

# - Process plugins here:
foreach(_vireplugin ${_vire_PLUGINS})
  if(EXISTS ${PROJECT_SOURCE_DIR}/plugins/${_vireplugin})
    message( STATUS "Processing Vire plugin '${_vireplugin}'...")
    add_subdirectory(${PROJECT_SOURCE_DIR}/plugins/${_vireplugin})
    # Publish resource files for this plugin:
    if(EXISTS ${PROJECT_SOURCE_DIR}/plugins/${_vireplugin}/resources/_resources.cmake)
      include(${PROJECT_SOURCE_DIR}/plugins/${_vireplugin}/resources/_resources.cmake)
      foreach(_rfin ${_vire_${_vireplugin}_RESOURCES_FILES})
	string(REGEX REPLACE "\\.in$" "" _rfout "${_rfin}")
	string(REGEX REPLACE
	  "^${PROJECT_SOURCE_DIR}/plugins/${_vireplugin}/resources/"
	  "${PROJECT_BUILD_DATAROOTDIR}/${VIRE_TAG}/resources/plugins/${_vireplugin}/"
	  _rfout
	  "${_rfout}")
	get_filename_component (_rfin_ext ${_rfin} EXT)
	message( STATUS "[debug] Vire plugin '${_vireplugin}': _rfin  = '${_rfin}'")
	message( STATUS "[debug] Vire plugin '${_vireplugin}': _rfout = '${_rfout}'")
	if (_rfin_ext STREQUAL ".in")
	  configure_file(${_rfin} ${_rfout} @ONLY)
	else()
	  configure_file(${_rfin} ${_rfout} COPYONLY)
	endif()
      endforeach()
    endif()
  else()
    message( FATAL "Vire plugin '${_vireplugin}' does not exist!")
  endif()
endforeach()

message (STATUS "===================================")
message (STATUS "=  E N D    O F    P L U G I N S  =")
message (STATUS "===================================")

# - end