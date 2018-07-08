# - CMake build script for Vire library

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
message(STATUS "[info] Begin of 'source/vire/CMakeLists.txt'")

#-----------------------------------------------------------------------
# Configure Vire library
#

message(STATUS "[debug] VIRE_LIBDIR_TO_RESOURCEDIR = '${VIRE_LIBDIR_TO_RESOURCEDIR}'")
message(STATUS "[debug] VIRE_LIBDIR_TO_PLUGINDIR   = '${VIRE_LIBDIR_TO_PLUGINDIR}'")
# set(VIRE_DATAROOTDIR ${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/${VIRE_TAG})
# set(VIRE_DATAROOTDIR ${PROJECT_BUILD_DATAROOTDIR}/${VIRE_TAG})
# message(STATUS "[debug] VIRE_DATAROOTDIR           = '${VIRE_DATAROOTDIR}'")

# -- RPATH settings --
# From: https://cmake.org/Wiki/CMake_RPATH_handling
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
# the RPATH to be used when installing, but only if it's not a system directory
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_FULL_LIBDIR}" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
endif()

set(VireLibrary_DEFINITIONS
  -std=c++11
  )

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS ${VireLibrary_DEFINITIONS})
endif()

set(VireLibrary_HEADERS
  vire.h
  vire_config.h.in
  resource_files.h
  plugin.h
  version.h.in
  exitcodes.h
  base_object_protobuf.h
  detail/vire_library.h
  )

set(VireLibrary_PRIVATE_HEADERS
  )

#-----------------------------------------------------------------------
# Declare sources
#
configure_file(vire.cc.in           vire.cc           @ONLY)
configure_file(resource_files.cc.in resource_files.cc @ONLY)
configure_file(plugin.cc.in         plugin.cc         @ONLY)

string(RANDOM LENGTH 18 MANGLE_BINRELOC_MAIN)
set(MANGLE_BINRELOC "VIREMB${MANGLE_BINRELOC_MAIN}")
configure_file(vire_binreloc.h.in vire_binreloc.h @ONLY)

set(VireLibrary_SOURCES
  version.cc
  ${CMAKE_CURRENT_BINARY_DIR}/vire.cc
  ${CMAKE_CURRENT_BINARY_DIR}/resource_files.cc
  ${CMAKE_CURRENT_BINARY_DIR}/plugin.cc
  ${CMAKE_CURRENT_BINARY_DIR}/vire_binreloc.h
  vire_binreloc.c
  detail/vire_library.cc
  )

set(VireLibrary_TESTS)

list(APPEND VireLibrary_TESTS
  testing/test-vire.cxx
  )

# -----------------------------------------------------------------------

message (STATUS "[info] Adding subdirectory 'protobuf'...")
add_subdirectory(protobuf)
get_directory_property(VireProtobufLibrary_TESTS
  DIRECTORY protobuf
  DEFINITION VireProtobufLibrary_TESTS
  )
list(APPEND VireLibrary_TESTS
  ${VireProtobufLibrary_TESTS}
  )

# Time utilities:
include(time/Lists.cmake)

# General utilities:
include(utility/Lists.cmake)

# Time utilities:
include(ui/Lists.cmake)

# Logging service:
include(logging/Lists.cmake)

# Running utilities:
include(running/Lists.cmake)

# User/group service
include(user/Lists.cmake)

# Devices description service
include(device/Lists.cmake)

# Resources description service
include(resource/Lists.cmake)

# Users authentication service
include(auth/Lists.cmake)

# Message formatting
include(message/Lists.cmake)

# CMS payload objects:
include(cms/Lists.cmake)

# Communication service
include(com/Lists.cmake)

# RabbitMQ communication
include(rabbitmq/Lists.cmake)

# Automaton:
include(automaton/Lists.cmake)

# Simulation:
include(simulation/Lists.cmake)

# CMS server kernel and utilities:
include(cmsserver/Lists.cmake)

# CMS client kernel and utilities:
include(cmsclient/Lists.cmake)

# # CMS subcontractor:
# include(cmssubcontractor/Lists.cmake)

list(APPEND VireLibrary_HEADERS
  )

list(APPEND VireLibrary_SOURCES
  the_introspectable.cc
  the_serializable.cc
  )

# - Publish headers
foreach(_hdrin ${VireLibrary_HEADERS})
  string(REGEX REPLACE "\\.in$" "" _hdrout "${_hdrin}")
  configure_file(${_hdrin} ${VIRE_BUILD_PREFIX}/include/vire/${_hdrout} @ONLY)
endforeach()

if (VIRE_WITH_QT_GUI)
  set(VIRE_WITH_QT_GUI 1)
  # - QT moc headers
  # set(VIRE_HEADERS_QT_TO_BE_MOCCED
  #   cmsclient/gui/main_window.h
  #   )
  QT5_WRAP_CPP(VireLibrary_HEADERS_QT_MOC
    ${VireLibrary_HEADERS_QT_TO_BE_MOCCED}
    # OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED ### This option does not work
    )
  list(APPEND VireLibrary_SOURCES
    ${VireLibrary_HEADERS_QT_MOC}
    )
endif()

#-----------------------------------------------------------------------
# Build/Link Library
#

include_directories(${Readline_INCLUDE_DIR})
include_directories(${BxJsontools_INCLUDE_DIRS})
include_directories(${BxRabbitMQ_INCLUDE_DIRS})
include_directories(${BxProtobuftools_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${VIRE_BUILD_PREFIX}/include/protobuf/cpp)
include_directories(${VIRE_BUILD_PREFIX}/include)

get_directory_property(_VireProtobufLibrary_cpp_HEADERS
  DIRECTORY protobuf
  DEFINITION VireProtobufLibrary_cpp_HEADERS)
get_directory_property(_VireProtobufLibrary_cpp_SOURCES
  DIRECTORY protobuf
  DEFINITION VireProtobufLibrary_cpp_SOURCES)
get_directory_property(_VireProtobufLibrary_cpp_TARGETS
  DIRECTORY protobuf
  DEFINITION VireProtobufLibrary_cpp_TARGETS)
if (VIRE_WITH_PROTOBUF_JAVA)
  get_directory_property(_VireProtobufLibrary_java_TARGETS
    DIRECTORY protobuf
    DEFINITION VireProtobufLibrary_java_TARGETS)
endif()
message(STATUS "[debug] VireLibrary_HEADERS='${VireLibrary_HEADERS}'")
message(STATUS "[debug] VireLibrary_PRIVATE_HEADERS='${VireLibrary_PRIVATE_HEADERS}'")
message(STATUS "[debug] VireLibrary_SOURCES='${VireLibrary_SOURCES}'")
message(STATUS "[debug] _VireProtobufLibrary_cpp_HEADERS='${_VireProtobufLibrary_cpp_HEADERS}'")
message(STATUS "[debug] _VireProtobufLibrary_cpp_SOURCES='${_VireProtobufLibrary_cpp_SOURCES}'")
message(STATUS "[debug] _VireProtobufLibrary_cpp_TARGETS='${_VireProtobufLibrary_cpp_TARGETS}'")
message(STATUS "[debug] _VireProtobufLibrary_java_TARGETS='${_VireProtobufLibrary_java_TARGETS}'")

#-----------------------------------------------------------------------
# Build/Link the library
#
add_library(Vire SHARED
  ${VireLibrary_HEADERS}
  ${VireLibrary_PRIVATE_HEADERS}
  ${VireLibrary_SOURCES}
  ${_VireProtobufLibrary_cpp_HEADERS}
  ${_VireProtobufLibrary_cpp_SOURCES}
  )
# Doc: https://cmake.org/Wiki/CMake_FAQ#How_can_I_add_a_dependency_to_a_source_file_which_is_generated_in_a_subdirectory.3F
# Tell CMake the source won't be available until build time.
set_source_files_properties(${_VireProtobufLibrary_cpp_HEADERS} PROPERTIES GENERATED 1)
set_source_files_properties(${_VireProtobufLibrary_cpp_SOURCES} PROPERTIES GENERATED 1)
set_target_properties(Vire PROPERTIES LINKER_LANGUAGE CXX)
foreach(_pb_target ${_VireProtobufLibrary_cpp_TARGETS})
  message( STATUS "[info] ##### Adding new dependency from target='${_pb_target}'")
  add_dependencies(Vire ${_pb_target})
endforeach()

target_compile_definitions(Vire PRIVATE ENABLE_BINRELOC)
message( STATUS "[debug] VIRE_CXX_COMPILE_FEATURES = '${VIRE_CXX_COMPILE_FEATURES}'")
target_compile_features(Vire PUBLIC ${VIRE_CXX_COMPILE_FEATURES})
# set_property(TARGET Vire APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include/vire>  # <prefix>/include/vire
#   )
message( STATUS "[debug] VIRE_BUILD_PREFIX        = '${VIRE_BUILD_PREFIX}'")
message( STATUS "[debug] CMAKE_CURRENT_BINARY_DIR = '${CMAKE_CURRENT_BINARY_DIR}'")
message( STATUS "[debug] CMAKE_INSTALL_INCLUDEDIR = '${CMAKE_INSTALL_INCLUDEDIR}'")
# - Use BinReloc
set_target_properties(Vire PROPERTIES COMPILE_DEFINITIONS ENABLE_BINRELOC)
# target_compile_definitions(Vire PRIVATE ENABLE_BINRELOC)

target_include_directories(Vire
  PUBLIC
  $<BUILD_INTERFACE:${VIRE_BUILD_PREFIX}/include>
  $<BUILD_INTERFACE:${VIRE_BUILD_PREFIX}/include/vire>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vire>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/protobuf/cpp>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/protobuf/cpp/vire>
  )

message( STATUS "[debug] Vire_Boost_LIBRARIES    = '${Vire_Boost_LIBRARIES}'")
message( STATUS "[debug] BxJsontools_LIBRARY     = '${BxJsontools_LIBRARY}'")
message( STATUS "[debug] BxProtobuftools_LIBRARY = '${BxProtobuftools_LIBRARY}'")
message( STATUS "[debug] PROTOBUF_LIBRARIES      = '${PROTOBUF_LIBRARIES}'")
message( STATUS "[debug] Readline_LIBRARIES      = '${Readline_LIBRARIES}'")
message( STATUS "[debug] BxRabbitMQ_LIBRARIES    = '${BxRabbitMQ_LIBRARIES}'")
message( STATUS "[debug] BxRabbitMQ_MANAGER_LIBRARIES = '${BxRabbitMQ_MANAGER_LIBRARIES}'")
target_link_libraries(Vire PUBLIC
  Bayeux::Bayeux
  ${Vire_Boost_LIBRARIES}
  ${BxJsontools_LIBRARY}
  ${BxProtobuftools_LIBRARY}
  ${PROTOBUF_LIBRARIES}
  ${BxRabbitMQ_LIBRARIES}
  ${BxRabbitMQ_MANAGER_LIBRARIES}
  ${Readline_LIBRARIES}
  pthread
  b64
  crypt)
if(VIRE_WITH_QT_GUI)
  target_link_libraries(Vire PUBLIC Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Svg)
endif()
message (STATUS "[info] CMAKE_INSTALL_FULL_LIBDIR = '${CMAKE_INSTALL_FULL_LIBDIR}'")

# if(UNIX AND NOT APPLE)
#   set_target_properties(Vire
#     PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
#     # PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}"
#     )
# elseif(APPLE)
#   # Temporary setting - needs testing
#   set_target_properties(Vire
#     PROPERTIES
#       INSTALL_NAME_DIR "@rpath"
#       #LINK_FLAGS "-Wl,-rpath,@loader_path"
#     )
# endif()

# - Install it
# TODO : fixup rpaths as needed
install(TARGETS Vire
  EXPORT  VireTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

# - Install public headers (preliminary)
install(DIRECTORY ${VIRE_BUILD_PREFIX}/include/vire
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
  FILES_MATCHING PATTERN "*.h"
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  )

# - Install Protobuf public headers (preliminary)
install(DIRECTORY ${VIRE_BUILD_PREFIX}/include/protobuf
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
  FILES_MATCHING PATTERN "*.pb.h"
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  )

#-----------------------------------------------------------------------
# Build Tests
#
# - NB assumes each test .cxx file generates a single executable
# - We also output the executables to a dedicated directory
if(VIRE_ENABLE_TESTING)
  foreach(_testsource ${VireLibrary_TESTS})
    set(_vire_TEST_ENVIRONMENT "VIRE_RESOURCE_FILES_DIR=${PROJECT_BUILD_DATAROOTDIR}/${VIRE_TAG}/resources")
    get_filename_component(_testname "${_testsource}" NAME_WE)
    string(REPLACE "-" ";" _testname_splitted ${_testname})
    list(LENGTH _testname_splitted _testname_splitted_length)
    set(_topic1 "")
    set(_topic2 "")
    set(_topic3 "")
    if (_testname_splitted_length GREATER 2)
      list(GET _testname_splitted 1 _topic1)
    endif()
    if (_testname_splitted_length GREATER 3)
      list(GET _testname_splitted 2 _topic2)
    endif()
    if (_testname_splitted_length GREATER 4)
      list(GET _testname_splitted 3 _topic3)
    endif()
    if (_testname_splitted_length EQUAL 2)
      list(APPEND _vire_TEST_ENVIRONMENT "VIRE_TESTING_DIR=${CMAKE_CURRENT_SOURCE_DIR}/testing")
    elseif (_testname_splitted_length EQUAL 3)
      list(APPEND _vire_TEST_ENVIRONMENT "VIRE_TESTING_ONE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/${_topic1}/testing")
    elseif (_testname_splitted_length EQUAL 4)
      list(APPEND _vire_TEST_ENVIRONMENT "VIRE_TESTING_TWO_DIR=${CMAKE_CURRENT_SOURCE_DIR}/${_topic1}/${_topic2}/testing")
    elseif (_testname_splitted_length EQUAL 5)
      list(APPEND _vire_TEST_ENVIRONMENT "VIRE_TESTING_THREE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/${_topic1}/${_topic2}/${_topic3}/testing")
    endif()
    list(APPEND _vire_TEST_ENVIRONMENT "VIRE_BASE_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
    #message( STATUS "[info] _vire_TEST_ENVIRONMENT='${_vire_TEST_ENVIRONMENT}'")
    set(_testname "vire-${_testname}")
    #message( STATUS "[info] _testname='${_testname}'")
    add_executable(${_testname} ${_testsource})
    target_link_libraries(${_testname}
      Vire
      )
    set(_test_with_protobuf 0)
    string(SUBSTRING ${_testname} 0 19 _testname_prefix)
    #message( STATUS "[info] _testname_prefix='${_testname_prefix}'")
    if (_testname_prefix STREQUAL "vire-test-protobuf-")
      message( STATUS "[info] Test '${_testname}' with Protobuf support.")
      set(_test_with_protobuf 1)
    endif()
    if(_test_with_protobuf)
      message( STATUS "[info] Special Protobuf support for test '${_testname}'...")
      include_directories(${PROTOBUF_INCLUDE_DIRS})
      include_directories(${VIRE_BUILD_PREFIX}/include/protobuf/cpp)
    endif()
    # - On Apple, ensure dynamic_lookup of undefined symbols
    if(APPLE)
      set_target_properties(${_testname} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif()
    add_test(NAME ${_testname} COMMAND ${_testname})
    set_property(TEST ${_testname}
      APPEND PROPERTY ENVIRONMENT ${_vire_TEST_ENVIRONMENT}
      )
    # - For now, dump them into the testing output directory
    set_target_properties(${_testname}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vire_tests
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vire_tests
      )
  endforeach()
endif()

message(STATUS "[info] End of 'source/vire/CMakeLists.txt'.")

# - end
