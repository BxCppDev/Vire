# - CMake script for Vire's Documentation
#

#-----------------------------------------------------------------------
# Copyright 2012,2013 Ben Morgan <bmorgan.warwick@gmail.com>
# Copyright 2012,2013 University of Warwick
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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Vire. If not, see <http://www.gnu.org/licenses/>.
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Core Doxygen Configuration
#
set(VIRE_DOXYFILE_IN  vire_doxygen.conf.in)
set(VIRE_DOXYFILE_OUT ${PROJECT_BINARY_DIR}/documentation/vire_doxygen.conf)
set(VIRE_DOXYGEN_OUTPUT_DIR  ${VIRE_BUILD_DATAROOTDIR}/${VIRE_DOCUMENTATION_DIR}/API)
set(VIRE_DOXYGEN_OUTPUT_FILE ${VIRE_DOXYGEN_OUTPUT_DIR}/html/index.html)
set(VIRE_DOXYGEN_OUTPUT_IMAGE_DIR ${VIRE_DOXYGEN_OUTPUT_DIR}/html/images)

option(VIRE_WITH_LATEX_DOCS "Build LaTeX Manual for Vire" OFF)
mark_as_advanced(VIRE_WITH_LATEX_DOCS)
if(VIRE_WITH_LATEX_DOCS)
  set(VIRE_GENERATE_LATEX "YES")
else()
  set(VIRE_GENERATE_LATEX "NO")
endif()

#-----------------------------------------------------------------------
# Individual Documents
#
set(Cadfael_VERSION "1.1.0")
configure_file(InstallationGuide.md.in InstallationGuide.md @ONLY)

set(VIRE_MARKDOWN_DOCS
  Mainpage.md
  #${PROJECT_SOURCE_DIR}/source/Vire.1.md
  #${CMAKE_CURRENT_BINARY_DIR}/InstallationGuide.md
  )

set(VIRE_ABSOLUTE_MARKDOWN_DOCS)
foreach(_mdDoc ${VIRE_MARKDOWN_DOCS})
  get_filename_component(_absMdDoc ${_mdDoc} ABSOLUTE)
  set(VIRE_ABSOLUTE_MARKDOWN_DOCS "${VIRE_ABSOLUTE_MARKDOWN_DOCS} ${_absMdDoc}")
endforeach()

# - Image paths
set(VIRE_DOXYGEN_IMAGE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/images")

#-----------------------------------------------------------------------
# Build doxygen docs
#
configure_file(${VIRE_DOXYFILE_IN} ${VIRE_DOXYFILE_OUT} @ONLY)

add_custom_command(
  OUTPUT  ${VIRE_DOXYGEN_OUTPUT_FILE}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${VIRE_DOXYGEN_OUTPUT_DIR}
  COMMAND ${DOXYGEN_EXECUTABLE} ${VIRE_DOXYFILE_OUT}
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  DEPENDS Vire ${VIRE_DOXYFILE_OUT} ${VIRE_MARKDOWN_DOCS}
  COMMENT "Doxygenating Vire"
  )

add_custom_target(vire_docs ALL DEPENDS ${VIRE_DOXYGEN_OUTPUT_FILE})
