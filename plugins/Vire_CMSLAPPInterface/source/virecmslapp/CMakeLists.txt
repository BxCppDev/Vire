# - Top level CMake script for Vire CMSLAPP interface plugin library

message( STATUS "*****************************************")
message( STATUS "* Vire CMSLAPP interface plugin library *")
message( STATUS "*****************************************")

# #-----------------------------------------------------------------------
# # BxJsontools support
# #

# find_package(BxJsontools REQUIRED)
# if (BxJsontools_FOUND)
#   message( STATUS "Found the BxJsontools library:")
#   message( STATUS "  BxJsontools_VERSION='${BxJsontools_VERSION}'")
#   message( STATUS "  BxJsontools_INCLUDE_DIRS='${BxJsontools_INCLUDE_DIRS}'")
#   message( STATUS "  BxJsontools_LIBRARY='${BxJsontools_LIBRARY}'")
# endif()

message( STATUS "[Vire_CMSLAPPInterface] VIRE_BUILD_PREFIX = '${VIRE_BUILD_PREFIX}'")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(protobuf)
get_directory_property(_VireCmsLappProtobufLibrary_cpp_HEADERS
  DIRECTORY protobuf
  DEFINITION Vire_CMSLAPPInterface_ProtobufLibrary_cpp_HEADERS)
get_directory_property(_VireCmsLappProtobufLibrary_cpp_SOURCES
  DIRECTORY protobuf
  DEFINITION Vire_CMSLAPPInterface_ProtobufLibrary_cpp_SOURCES)
get_directory_property(_VireCmsLappProtobufLibrary_cpp_TARGETS
  DIRECTORY protobuf
  DEFINITION VireCmsLappProtobufLibrary_cpp_TARGETS)
if (VIRE_WITH_PROTOBUF_JAVA)
  get_directory_property(_VireCmsLappProtobufLibrary_java_TARGETS
    DIRECTORY protobuf
    DEFINITION VireCmsLappProtobufLibrary_java_TARGETS)
endif()
message(STATUS "[Vire_CMSLAPPInterface] _VireCmsLappProtobufLibrary_cpp_HEADERS='${_VireCmsLappProtobufLibrary_cpp_HEADERS}'")
message(STATUS "[Vire_CMSLAPPInterface] _VireCmsLappProtobufLibrary_cpp_SOURCES='${_VireCmsLappProtobufLibrary_cpp_SOURCES}'")

#-----------------------------------------------------------------------
# Declare headers
#
set(Vire_CMSLAPPInterface_Plugin_CORE_HEADERS
  vire/cmslapp/version.h.in
  vire/cmslapp/pubsub_support.h
)

set(Vire_CMSLAPPInterface_Plugin_HEADERS
  ${Vire_CMSLAPPInterface_Plugin_CORE_HEADERS}
  ${_VireCmsLappProtobufLibrary_cpp_HEADERS}
)

set(Vire_CMSLAPPInterface_Plugin_PRIVATE_HEADERS
)

#-----------------------------------------------------------------------
# Declare sources
#

# configure_file(vire/cmslapp/vire_xxx.cc.in
#   vire/cmslapp/vire_xxx.cc @ONLY)

set(Vire_CMSLAPPInterface_Plugin_SOURCES
  vire/cmslapp/version.cc
  vire/cmslapp/pubsub_support.cc
  ${_VireCmsLappProtobufLibrary_cpp_SOURCES}
)

# - Publish headers
foreach(_hdrin ${Vire_CMSLAPPInterface_Plugin_CORE_HEADERS})
  string(REGEX REPLACE "\\.in$" "" _hdrout "${_hdrin}")
  configure_file(${_hdrin} ${VIRE_BUILD_PREFIX}/include/${_hdrout} @ONLY)
endforeach()


add_library(Vire_CMSLAPP
  SHARED
  ${Vire_CMSLAPPInterface_Plugin_HEADERS}
  ${Vire_CMSLAPPInterface_Plugin_PRIVATE_HEADERS}
  ${Vire_CMSLAPPInterface_Plugin_SOURCES}
  ${_VireCmsLappProtobufLibrary_cpp_HEADERS}
  ${_VireCmsLappProtobufLibrary_cpp_SOURCES}
  )
# Tell CMake the source won't be available until build time.
set_source_files_properties(${_VireCmsLappProtobufLibrary_cpp_HEADERS} PROPERTIES GENERATED 1)
set_source_files_properties(${_VireCmsLappProtobufLibrary_cpp_SOURCES} PROPERTIES GENERATED 1)
set_target_properties(Vire_CMSLAPP PROPERTIES LINKER_LANGUAGE CXX)
foreach(_pb_target ${_VireCmsLappProtobufLibrary_cpp_TARGETS})
  message( STATUS "[info] ##### Adding new dependency from target='${_pb_target}'")
  add_dependencies(Vire ${_pb_target})
endforeach()


message (STATUS "[Vire_CMSLAPPInterface] Boost_LOG_LIBRARY       = ${Boost_LOG_LIBRARY}")
message (STATUS "[Vire_CMSLAPPInterface] Boost_LOG_SETUP_LIBRARY = ${Boost_LOG_SETUP_LIBRARY}")
message (STATUS "[Vire_CMSLAPPInterface] PROTOBUF_LIBRARIES      = ${PROTOBUF_LIBRARIES}")
target_link_libraries(Vire_CMSLAPP
  Vire
  ${Boost_LOG_LIBRARY}
  ${Boost_LOG_SETUP_LIBRARY}
  #${PROTOBUF_LIBRARIES}
  )


# - RPATH it
set_target_properties(Vire_CMSLAPP PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

if(UNIX AND NOT APPLE)
  set_target_properties(Vire_CMSLAPP
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(APPLE)
  # Temporary setting - needs testing
  set_target_properties(Vire_CMSLAPP
    PROPERTIES
      INSTALL_NAME_DIR "@rpath"
      #LINK_FLAGS "-Wl,-rpath,@loader_path"
    )
endif()

# export(TARGETS Vire_CMSLAPP ${Vire_ADDON_TARGETS} FILE ${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_CMAKEDIR}/Vire-${Vire_VERSION}/VireTargets.cmake)

# - Install it
# TODO : fixup rpaths as needed
install(TARGETS Vire_CMSLAPP
  EXPORT  VireTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

# Install it:
install(TARGETS Vire_CMSLAPP DESTINATION ${CMAKE_INSTALL_LIBDIR}/${VIRE_PLUGINLIBDIR})

# # - Install public headers (preliminary)
# install(DIRECTORY ${VIRE_BUILD_PREFIX}/include/vire/cmslapp
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#   )

# # - Install public headers (preliminary)
# install(DIRECTORY ${Vire_CMSLAPP}/include/vire
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#  )
# - List of test programs:
set(Vire_CMSLAPP_TESTS
  testing/test-cmslapp-version.cxx
  )

set(_virecmslapp_TEST_ENVIRONMENT "VIRE_RESOURCE_FILES_DIR=${VIRE_DATAROOTDIR}/resources")
if(VIRE_ENABLE_TESTING)
  foreach(_testsource ${Vire_CMSLAPP_TESTS})
    # message( STATUS "=========== TEST PLUGIN")
    get_filename_component(_testname "${_testsource}" NAME_WE)
    set(_testname "virecmslapp-${_testname}")
    add_executable(${_testname} ${_testsource} ${testing_SOURCES})
    target_link_libraries(${_testname} Vire_CMSLAPP Vire)
    # - On Apple, ensure dynamic_lookup of undefined symbols
    if(APPLE)
      set_target_properties(${_testname} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif()
    add_test(NAME ${_testname} COMMAND ${_testname})
    set_property(TEST ${_testname}
      APPEND PROPERTY ENVIRONMENT ${_virecmslapp_TEST_ENVIRONMENT}
      )
    # - For now, dump them into the testing output directory
    set_target_properties(${_testname}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vire_tests/plugins
      ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vire_tests/plugins
      )
  endforeach()
endif()

# - end