# - Top level CMake script for Vire RabbitMQ interface plugin library

message( STATUS "********************************")
message( STATUS "* Vire RabbitMQ plugin library *")
message( STATUS "********************************")

message( STATUS "[Vire_RabbitMQ] VIRE_BUILD_PREFIX = '${VIRE_BUILD_PREFIX}'")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${VIRE_BUILD_PREFIX}/include)
include_directories(${VIRE_BUILD_PREFIX}/include/vire)


#-----------------------------------------------------------------------
# Declare headers
#
set(Vire_RabbitMQ_Plugin_HEADERS
  vire/rabbitmq/version.h.in
  vire/rabbitmq/permissions_utils.h
  vire/rabbitmq/user.h
  vire/rabbitmq/transport_driver.h
  vire/rabbitmq/manager_service.h
)

set(Vire_RabbitMQ_Plugin_PRIVATE_HEADERS
)

#-----------------------------------------------------------------------
# Declare sources
#

# configure_file(vire/rabbitmq/vire_xxx.cc.in
#   vire/rabbitmq/vire_xxx.cc @ONLY)

set(Vire_RabbitMQ_Plugin_SOURCES
  vire/rabbitmq/version.cc
  vire/rabbitmq/user.cc
  vire/rabbitmq/permissions_utils.cc
  vire/rabbitmq/transport_driver.cc
  vire/rabbitmq/manager_service.cc
)

# - Publish headers
foreach(_hdrin ${Vire_RabbitMQ_Plugin_HEADERS})
  string(REGEX REPLACE "\\.in$" "" _hdrout "${_hdrin}")
  configure_file(${_hdrin} ${VIRE_BUILD_PREFIX}/include/${_hdrout} @ONLY)
endforeach()


add_library(Vire_RabbitMQ
  SHARED
  ${Vire_RabbitMQ_Plugin_HEADERS}
  ${Vire_RabbitMQ_Plugin_PRIVATE_HEADERS}
  ${Vire_RabbitMQ_Plugin_SOURCES}
  )

target_link_libraries(Vire_RabbitMQ
  Vire
  ${_Vire_RabbitMQ_BxRabbitMQ_LIBRARIES}
  )

# - RPATH it
set_target_properties(Vire_RabbitMQ PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

if(UNIX AND NOT APPLE)
  set_target_properties(Vire_RabbitMQ
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(APPLE)
  # Temporary setting - needs testing
  set_target_properties(Vire_RabbitMQ
    PROPERTIES
      INSTALL_NAME_DIR "@rpath"
      #LINK_FLAGS "-Wl,-rpath,@loader_path"
    )
endif()

# export(TARGETS Vire_RabbitMQ ${Vire_ADDON_TARGETS} FILE ${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_CMAKEDIR}/Vire-${Vire_VERSION}/VireTargets.cmake)

# - Install it
# TODO : fixup rpaths as needed
install(TARGETS Vire_RabbitMQ
  EXPORT  VireTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

# Install it:
install(TARGETS Vire_RabbitMQ DESTINATION ${CMAKE_INSTALL_LIBDIR}/${VIRE_PLUGINLIBDIR})

# # - Install public headers (preliminary)
# install(DIRECTORY ${VIRE_BUILD_PREFIX}/include/vire/rabbitmq
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#   )

# # - Install public headers (preliminary)
# install(DIRECTORY ${Vire_RabbitMQ}/include/vire
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#  )
# - List of test programs:
set(Vire_RabbitMQ_TESTS
  testing/test_version.cxx
  testing/test_manager_service.cxx
  )

set(_virerabbitmq_TEST_ENVIRONMENT "VIRE_RESOURCE_FILES_DIR=${VIRE_DATAROOTDIR}/resources")
if(VIRE_ENABLE_TESTING)
  foreach(_testsource ${Vire_RabbitMQ_TESTS})
    get_filename_component(_testname "${_testsource}" NAME_WE)
    set(_testname "virerabbitmq-${_testname}")
    add_executable(${_testname} ${_testsource} ${testing_SOURCES})
    target_link_libraries(${_testname} Vire_RabbitMQ Vire)
    # - On Apple, ensure dynamic_lookup of undefined symbols
    if(APPLE)
      set_target_properties(${_testname} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif()
    add_test(NAME ${_testname} COMMAND ${_testname})
    set_property(TEST ${_testname}
      APPEND PROPERTY ENVIRONMENT ${_virerabbitmq_TEST_ENVIRONMENT}
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