# - Package resource files as needed by the Vire toolkit
# Because there are many resources, use a script based GLOB to locate
# them together with a custom target to rebuild the resources
#

add_custom_target(vire_resources
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/_BuildResourceList.sh
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Bundling Vire Resources"
  )
set(Vire_RESOURCES)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/_resources.cmake")
  include(_resources.cmake)
endif()

foreach(_rfin ${Vire_RESOURCES})
  string(REGEX REPLACE "\\.in$" "" _rfout "${_rfin}")
  string(REGEX REPLACE "^${PROJECT_SOURCE_DIR}/resources" "${PROJECT_BUILD_DATAROOTDIR}/${VIRE_TAG}/resources" _rfout "${_rfout}")
  get_filename_component (_rfin_ext ${_rfin} EXT)
  #message( STATUS "[info] Populating resource file '${_rfout}'...")
  if (_rfin_ext STREQUAL ".in")
    #message( STATUS "[info] Configure resource file '${_rfin}' (@ONLY)")
    configure_file(${_rfin} ${_rfout} @ONLY)
  else()
    #message( STATUS "[info] Configure resource file '${_rfin}' (COPYONLY)")
    configure_file(${_rfin} ${_rfout} COPYONLY)
  endif()
endforeach()
