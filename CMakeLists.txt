# - Top level CMake script for Vire

#-----------------------------------------------------------------------
# Copyright (C) 2012,2013 Ben Morgan <bmorgan.warwick@gmail.com>
# Copyright (C) 2012,2013 University of Warwick
# Copyright (C) 2015-2017 François Mauger <mauger@lpccaen.in2p3.fr>
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
# along with Vire.  If not, see <http://www.gnu.org/licenses/>.
#-----------------------------------------------------------------------
message (STATUS "[info] Begin of 'CMakeLists.txt'.")

#-----------------------------------------------------------------------
# Enforce an out-of-source build.
#
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(STATUS "Vire requires an out-of-source build.")
  message(STATUS "Please remove these files from ${CMAKE_BINARY_DIR} first:")
  message(STATUS "  CMakeCache.txt")
  message(STATUS "  CMakeFiles")
  message(STATUS "Once these files are removed, create a separate directory")
  message(STATUS "and run CMake from there, pointing it to:")
  message(STATUS "  ${CMAKE_SOURCE_DIR}")
  message(FATAL_ERROR "in-source build detected")
endif()

#-----------------------------------------------------------------------
# CMake/project requirements and configuration
#
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(Vire)

# - Load custom modules
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

# - Version number management utilities:
include(BxVersionManager)

# - Versioning only giving major and minor. Patch number is
#   automatically deduced from an external file.
bx_version_set(Vire 1 0)
message(STATUS "[info] Project version = '${PROJECT_VERSION}'")

#-----------------------------------------------------------------------
# Common LPC utilities
#
include(LPCCMakeSettings)

# - SVN revision number for dev builds
include(LPCSVNUtilities)
set(Vire_VERSION_REVISION 0)

Subversion_DIRECTORY_IS_WC(${PROJECT_SOURCE_DIR} Vire)
if(Vire_DIRECTORY_IS_WC)
  Subversion_WC_INFO(${PROJECT_SOURCE_DIR} Vire)
  set(Vire_VERSION_REVISION ${Vire_WC_REVISION})
endif()

#-----------------------------------------------------------------------
# Useful variables
set(VIRE_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(VIRE_BINARY_DIR "${PROJECT_BINARY_DIR}")

#-----------------------------------------------------------------------
# Configure testing if required
#
option(VIRE_ENABLE_TESTING "Build unit testing system for Vire" OFF)
if(VIRE_ENABLE_TESTING)
  enable_testing()
endif()

#-----------------------------------------------------------------------
# Configure/Build utilities as needed
#
option(VIRE_WITH_DEVELOPER_TOOLS "Build/install developer tools" ON)
mark_as_advanced(VIRE_WITH_DEVELOPER_TOOLS)

#-----------------------------------------------------------------------
# Optional build of documentation
#
option(VIRE_WITH_DOCS "Build documentation for Vire" ON)

#-----------------------------------------------------------------------
# Option for enabling Java component
#
option(VIRE_WITH_JAVA "Build Vire Java component" OFF)
mark_as_advanced(VIRE_WITH_JAVA)

#-----------------------------------------------------------------------
# Option for enabling Qt GUI components
#
option(VIRE_WITH_QT_GUI "Build Qt-based GUI components" ON)
mark_as_advanced(VIRE_WITH_QT_GUI)
if (VIRE_WITH_QT_GUI)
  # Minimal system Qt5 version from Ubuntu 14.04:
  set(VIRE_QT5_VERSION "5.2.1")
  # Qt5 version from cadfaelbrew (but conflict with system installation
  # on Ubuntu 14.04 when running examples):
  # set(VIRE_QT5_VERSION "5.6.0")
endif()

#-----------------------------------------------------------------------
# Configure Google Protocol Buffer support
#
option(VIRE_WITH_PROTOBUF_JAVA "Build Vire with Google Protocol Buffer Java support" OFF)

#-----------------------------------------------------------------------
# Vire plugins support
#
option(VIRE_WITH_PLUGINS "Build Vire plugins" ON)
mark_as_advanced(VIRE_WITH_PLUGINS)

#-----------------------------------------------------------------------
# Sandbox
#
option(VIRE_WITH_SANDBOX "Build Vire sandbox material" OFF)
mark_as_advanced(VIRE_WITH_SANDBOX)

#-----------------------------------------------------------------------
# Debug
message( STATUS "[info] Vire_VERSION              = '${Vire_VERSION}'")
message( STATUS "[info] VIRE_SOURCE_DIR           = '${VIRE_SOURCE_DIR}'")
message( STATUS "[info] VIRE_BINARY_DIR           = '${VIRE_BINARY_DIR}'")
message( STATUS "[info] VIRE_ENABLE_TESTING       = '${VIRE_ENABLE_TESTING}'")
message( STATUS "[info] VIRE_WITH_DEVELOPER_TOOLS = '${VIRE_WITH_DEVELOPER_TOOLS}'")
message( STATUS "[info] VIRE_WITH_DOCS            = '${VIRE_WITH_DOCS}'")
message( STATUS "[info] VIRE_WITH_JAVA            = '${VIRE_WITH_JAVA}'")
message( STATUS "[info] VIRE_WITH_QT_GUI          = '${VIRE_WITH_QT_GUI}'")
message( STATUS "[info] VIRE_WITH_PROTOBUF_JAVA   = '${VIRE_WITH_PROTOBUF_JAVA}'")
message( STATUS "[info] VIRE_WITH_PLUGINS         = '${VIRE_WITH_PLUGINS}'")
message( STATUS "[info] VIRE_WITH_SANDBOX         = '${VIRE_WITH_SANDBOX}'")

#-----------------------------------------------------------------------
# Bayeux is the main external dependency, and we know it will additionally
# search for and provide compatible versions of:
#
# - Boost
# - GSL
# - ROOT
set(VIRE_BAYEUX_MIN_VERSION "3.1.0")
message( STATUS "[info] Searching Bayeux ${VIRE_BAYEUX_MIN_VERSION}...")
find_package(Bayeux ${VIRE_BAYEUX_MIN_VERSION} REQUIRED NO_MODULE)
if (Bayeux_FOUND)
  message( STATUS "[info] Bayeux ${Bayeux_VERSION} found.")
  message( STATUS "[info] BAYEUX_BOOST_VERSION    = '${BAYEUX_BOOST_VERSION}'")
  # Trick because BayeuxConfig 3.1 does not provide the BAYEUX_BOOST_COMPONENTS variable:
  set(Vire_BAYEUX_VERSION ${Bayeux_VERSION})
  set(BAYEUX_BOOST_COMPONENTS
    "filesystem;system;serialization;iostreams;program_options;regex;thread"
    )
  message( STATUS "[info] BAYEUX_BOOST_COMPONENTS = '${BAYEUX_BOOST_COMPONENTS}'")
  message( STATUS "[info] Bayeux_CMAKE_CONFIG_DIR = '${Bayeux_CMAKE_CONFIG_DIR}'")
endif()

#-------------------------------------------------------------------------
# Boost is also important with specific libs
message( STATUS "[info] Searching special Boost material for Vire...")
set(VIRE_BOOST_MIN_VERSION ${BAYEUX_BOOST_VERSION})
set(Boost_NO_BOOST_CMAKE ON)
set(VIRE_BOOST_COMPONENTS
  ${BAYEUX_BOOST_COMPONENTS}
  )
list(APPEND VIRE_BOOST_COMPONENTS log)
message( STATUS "[info] VIRE_BOOST_COMPONENTS   = '${VIRE_BOOST_COMPONENTS}'")
message( STATUS "[info] Finding Boost with specific libraries...")
find_package(Boost ${VIRE_BOOST_MIN_VERSION} REQUIRED
  ${VIRE_BOOST_COMPONENTS}
  )
set(Vire_Boost_VERSION ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION})
set(Vire_Boost_LIBRARIES)
foreach(_vire_boost_lib ${VIRE_BOOST_COMPONENTS})
  list(APPEND Vire_Boost_LIBRARIES Boost::${_vire_boost_lib})
endforeach()
message(STATUS "[info] Vire_Boost_VERSION   = '${Vire_Boost_VERSION}'")
message(STATUS "[info] Vire_Boost_LIBRARIES = '${Vire_Boost_LIBRARIES}'")
# message(STATUS "[info] Boost_LIBRARIES               = '${Boost_LIBRARIES}'")
# message(STATUS "[info] Boost_FILESYSTEM_LIBRARY      = '${Boost_FILESYSTEM_LIBRARY}'")
# message(STATUS "[info] Boost_SYSTEM_LIBRARY          = '${Boost_SYSTEM_LIBRARY}'")
# message(STATUS "[info] Boost_SERIALIZATION_LIBRARY   = '${Boost_SERIALIZATION_LIBRARY}'")
# message(STATUS "[info] Boost_IOSTREAMS_LIBRARY       = '${Boost_IOSTREAMS_LIBRARY}'")
# message(STATUS "[info] Boost_PROGRAM_OPTIONS_LIBRARY = '${Boost_PROGRAM_OPTIONS_LIBRARY}'")
# message(STATUS "[info] Boost_DATE_TIME_LIBRARY       = '${Boost_DATE_TIME_LIBRARY}'")
# message(STATUS "[info] Boost_THREAD_LIBRARY          = '${Boost_THREAD_LIBRARY}'")
# message(STATUS "[info] Boost_LOG_LIBRARY             = '${Boost_LOG_LIBRARY}'")
# message(STATUS "[info] Boost_LOG_SETUP_LIBRARY       = '${Boost_LOG_SETUP_LIBRARY}'")

# # - Camp
# find_package(CAMP ${BAYEUX_CAMP_VERSION} REQUIRED)

# # - CLHEP
# find_package(CLHEP ${BAYEUX_CLHEP_VERSION} REQUIRED)

if (VIRE_WITH_JAVA OR VIRE_WITH_PROTOBUF_JAVA)
  message( STATUS "[info] Searching Java...")
  find_package(Java)
  if (Java_FOUND)
    message( STATUS "[info] Java ${Java_VERSION_STRING} found.")
  endif()
else()
  message( STATUS "[info] No Java support.")
endif()

#-----------------------------------------------------------------------
# Build the subdirectories as required
#
message( STATUS "[info] Adding subdirectory 'source'...")
add_subdirectory(source)

# if(VIRE_WITH_DOCS)
#   add_subdirectory(documentation)
# endif()

#-----------------------------------------------------------------------
# Resource installation
add_subdirectory(resources)


#-----------------------------------------------------------------------
# Prepare plugin installation
# - List of built plugins:
set(VIRE_PLUGINS
  Vire_MOS              # The Vire to Multipurpose OPCUA Server (MOS) plugin and utilities
  Vire_CMSLAPPInterface # The Vire CMS/LAPP interface plugin
  Vire_RabbitMQ         # The Vire RabbitMQ plugin
)

#-----------------------------------------------------------------------
# - CMake Support files
include(CMakePackageConfigHelpers)

# - Versioning file is the same for the build and install trees
write_basic_package_version_file(
  ${VIRE_BUILD_CMAKEDIR}/${VIRE_TAG}/VireConfigVersion.cmake
  COMPATIBILITY SameMajorVersion
  )

message(STATUS "[debug] CMAKE_INSTALL_CMAKEDIR = '${CMAKE_INSTALL_CMAKEDIR}'")
message(STATUS "[debug] PROJECT_TAG            = '${PROJECT_TAG}'")

message(STATUS "[debug] Fetch variables from subdirectories : ")
get_directory_property(BxJsontools_VERSION     DIRECTORY "source/vire" DEFINITION "BxJsontools_VERSION" )
get_directory_property(BxProtobuftools_VERSION DIRECTORY "source/vire" DEFINITION "BxProtobuftools_VERSION" )
get_directory_property(BxRabbitMQ_VERSION      DIRECTORY "source/vire" DEFINITION "BxRabbitMQ_VERSION" )
message(STATUS "[debug] * BxJsontools_VERSION     = '${BxJsontools_VERSION}'")
message(STATUS "[debug] * BxProtobuftools_VERSION = '${BxProtobuftools_VERSION}'")
message(STATUS "[debug] * BxRabbitMQ_VERSION      = '${BxRabbitMQ_VERSION}'")
# - Config file is also the same in build/install trees as we use same layout
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/VireConfig.cmake.in
  ${VIRE_BUILD_CMAKEDIR}/${VIRE_TAG}/VireConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/${PROJECT_TAG}
  PATH_VARS
    CMAKE_INSTALL_BINDIR
    CMAKE_INSTALL_LIBDIR
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_DATAROOTDIR
    )

# - Targets (build tree)
export(EXPORT VireTargets
  NAMESPACE Vire::
  FILE ${VIRE_BUILD_CMAKEDIR}/${VIRE_TAG}/VireTargets.cmake
  )

# - Targets (install tree)
install(EXPORT VireTargets
  NAMESPACE Vire::
  DESTINATION ${CMAKE_INSTALL_CMAKEDIR}/${VIRE_TAG}
  )

# - Installation of, well, install tree files
install(
  FILES
    ${PROJECT_BUILD_CMAKEDIR}/${VIRE_TAG}/VireConfigVersion.cmake
    ${PROJECT_BUILD_CMAKEDIR}/${VIRE_TAG}/VireConfig.cmake
  DESTINATION
    ${CMAKE_INSTALL_CMAKEDIR}/${VIRE_TAG}
  )

#-----------------------------------------------------------------------
# - Install published resources and documentation
message(STATUS "[devel] PROJECT_BUILD_DATAROOTDIR = '${PROJECT_BUILD_DATAROOTDIR}'")
install(DIRECTORY ${PROJECT_BUILD_DATAROOTDIR}/
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}
  )

#-----------------------------------------------------------------------
# - Plugins installation - Must come last because they must find Vire
if (VIRE_WITH_PLUGINS)
  add_subdirectory(plugins)
endif()

message (STATUS "[info] End of 'CMakeLists.txt'.")

# - end