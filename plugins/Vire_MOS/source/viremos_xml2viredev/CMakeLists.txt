# Vire MOS XML to Vire device conversion

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
# include_directories(${CMAKE_CURRENT_BINARY_DIR}/../viremos)

set(VireMos_Xml2VireDev_SOURCES
  viremos_xml2viredev.cxx
  )

add_executable(viremos_xml2viredev
  ${VireMos_Xml2VireDev_SOURCES}
  )
# message(STATUS "****** Boost_LIBRARIES   = '${Boost_LIBRARIES}'")
# message(STATUS "****** Boost_LOG_LIBRARY = '${Boost_LOG_LIBRARY}'")
target_link_libraries(viremos_xml2viredev
  Vire_MOS
  Vire
  ${XERCESC_LIBRARIES}
  ${Boost_LOG_LIBRARY})

# - RPath it
set_target_properties(viremos_xml2viredev PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

if(UNIX AND NOT APPLE)
  set_target_properties(viremos_xml2viredev
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(APPLE)
  # Temporary setting - needs testing
  set_target_properties(viremos_xml2viredev
    PROPERTIES
      INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}"
      LINK_FLAGS "-Wl,-rpath,@loader_path/../${CMAKE_INSTALL_LIBDIR}"
    )
endif()

# - Install it
install(TARGETS viremos_xml2viredev
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

# - end