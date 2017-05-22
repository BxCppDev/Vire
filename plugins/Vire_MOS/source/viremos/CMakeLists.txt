# - Top level CMake script for Vire MOS plugin library

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#-----------------------------------------------------------------------
# Declare headers
#

set(VireMosPlugin_HEADERS
  vire/mos/vire_mos_config.h.in
  vire/mos/vire_mos.h
  vire/mos/version.h.in
  vire/mos/opcua_ast.h
  vire/mos/opcua_dom.h
  vire/mos/dom_utils.h
  vire/mos/dom_utils-inl.h
  vire/mos/opcua_ast-reflect.h
  vire/mos/export_utils.h
  vire/mos/opcua_export_vire.h
  vire/mos/opcua_xml.h
  )

set(VireMosPlugin_PRIVATE_HEADERS
)

#-----------------------------------------------------------------------
# Declare sources
#
configure_file(vire/mos/vire_mos.cc.in vire/mos/vire_mos.cc @ONLY)

set(VireMosPlugin_SOURCES
  ${CMAKE_CURRENT_BINARY_DIR}/vire/mos/vire_mos.cc
  vire/mos/version.cc
  vire/mos/opcua_ast.cc
  vire/mos/opcua_dom.cc
  vire/mos/dom_utils.cc
  vire/mos/opcua_ast-reflect.cc
  vire/mos/opcua_export_vire.cc
  vire/mos/export_utils.cc
  )

# - Publish headers
foreach(_hdrin ${VireMosPlugin_HEADERS})
  string(REGEX REPLACE "\\.in$" "" _hdrout "${_hdrin}")
  configure_file(${_hdrin} ${VIRE_BUILD_PREFIX}/include/${_hdrout} @ONLY)
endforeach()

add_library(Vire_MOS
  SHARED
  ${VireMosPlugin_HEADERS}
  ${VireMosPlugin_PRIVATE_HEADERS}
  ${VireMosPlugin_SOURCES}
  )

target_link_libraries(Vire_MOS
  Vire
  XercesC::XercesC
  )

# - RPATH it
set_target_properties(Vire_MOS PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

if(UNIX AND NOT APPLE)
  set_target_properties(Vire_MOS
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(APPLE)
  # Temporary setting - needs testing
  set_target_properties(Vire_MOS
    PROPERTIES
      INSTALL_NAME_DIR "@rpath"
      #LINK_FLAGS "-Wl,-rpath,@loader_path"
    )
endif()

# export(TARGETS Vire_MOS ${Vire_ADDON_TARGETS} FILE ${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_CMAKEDIR}/Vire-${Vire_VERSION}/VireTargets.cmake)

# - Install it
# TODO : fixup rpaths as needed
install(TARGETS Vire_MOS
  EXPORT  VireTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

# Install it:
install(TARGETS Vire_MOS DESTINATION ${CMAKE_INSTALL_LIBDIR}/${VIRE_PLUGINLIBDIR})

# # - Install public headers (preliminary)
# install(DIRECTORY ${VIRE_BUILD_PREFIX}/include/vire/mos
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#   )

# # - Install public headers (preliminary)
# install(DIRECTORY ${Vire_MOS}/include/vire
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#  )
# - List of test programs:
set(Vire_MOS_TESTS
  testing/test_opcua_ast.cxx
  )

set(_viremos_TEST_ENVIRONMENT "VIRE_RESOURCE_FILES_DIR=${VIRE_DATAROOTDIR}/${VIRE_TAG}/resources")
if(VIRE_ENABLE_TESTING)
  foreach(_testsource ${Vire_MOS_TESTS})
    get_filename_component(_testname "${_testsource}" NAME_WE)
    set(_testname "viremos-${_testname}")
    add_executable(${_testname} ${_testsource} ${testing_SOURCES})
    target_link_libraries(${_testname} Vire_MOS Vire)
    # - On Apple, ensure dynamic_lookup of undefined symbols
    if(APPLE)
      set_target_properties(${_testname} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif()
    add_test(NAME ${_testname} COMMAND ${_testname})
    set_property(TEST ${_testname}
      APPEND PROPERTY ENVIRONMENT ${_viremos_TEST_ENVIRONMENT}
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