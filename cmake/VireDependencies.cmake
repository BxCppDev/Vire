# -----------------------------------------------------
message( STATUS "[info] Define Vire dependencies")

#-------------------------------------------------------
# Bayeux
#
# Bayeux is the main external dependency, and we know it will additionally
# search for and provide compatible versions of:
#
# - Boost
# - GSL
# - ROOT
set(VIRE_BAYEUX_MIN_VERSION "3.2.0")
message( STATUS "[info] Searching Bayeux ${VIRE_BAYEUX_MIN_VERSION}...")
find_package(Bayeux ${VIRE_BAYEUX_MIN_VERSION} REQUIRED NO_MODULE)
if (Bayeux_FOUND)
  message( STATUS "[info] Bayeux ${Bayeux_VERSION} found.")
  message( STATUS "[info] BAYEUX_BOOST_VERSION    = '${BAYEUX_BOOST_VERSION}'")
  # Trick because BayeuxConfig 3.2 does not provide the BAYEUX_BOOST_COMPONENTS variable:
  set(Vire_BAYEUX_VERSION ${Bayeux_VERSION})
  # # Trick because BayeuxConfig 3.2 does not provide the BAYEUX_BOOST_COMPONENTS variable:
  # set(BAYEUX_BOOST_COMPONENTS
  #   "filesystem;system;serialization;iostreams;program_options;regex;thread"
  #   )
  message( STATUS "[info] BAYEUX_BOOST_COMPONENTS = '${BAYEUX_BOOST_COMPONENTS}'")
  message( STATUS "[info] Bayeux_CMAKE_CONFIG_DIR = '${Bayeux_CMAKE_CONFIG_DIR}'")
endif()

# -------------------------------------------------------
# - Boost:
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

#-----------------------------------------------------------------------
# Java
#
if (VIRE_WITH_JAVA OR VIRE_WITH_PROTOBUF_JAVA)
  message( STATUS "[info] Searching Java...")
  find_package(Java)
  if (Java_FOUND)
    message( STATUS "[info] Java ${Java_VERSION_STRING} found.")
    message(STATUS "[info] Java_VERSION_STRING   : '${Java_VERSION_STRING}'")
    message(STATUS "[info] Java_JAVAC_EXECUTABLE : '${Java_JAVAC_EXECUTABLE}'")
    message(STATUS "[info] Java_JAR_EXECUTABLE   : '${Java_JAR_EXECUTABLE}'")
  else()
    message( FATAL "[fatal] Could not find Java!")
  endif()
else()
  message( STATUS "[info] No Java support.")
endif()

#-----------------------------------------------------------------------
# BxProtobuftools support
#
message (STATUS "[info] Searching for BxProtobuftools...")
find_package(BxProtobuftools 0.3 REQUIRED CONFIG)
if (BxProtobuftools_FOUND)
  message( STATUS "[info] Found the BxProtobuftools library:")
  message( STATUS "[info]   BxProtobuftools_VERSION      = '${BxProtobuftools_VERSION}'")
  message( STATUS "[info]   BxProtobuftools_DEFINITIONS  = '${BxProtobuftools_DEFINITIONS}'")
  message( STATUS "[info]   BxProtobuftools_INCLUDE_DIR  = '${BxProtobuftools_INCLUDE_DIR}'")
  message( STATUS "[info]   BxProtobuftools_INCLUDE_DIRS = '${BxProtobuftools_INCLUDE_DIRS}'")
  message( STATUS "[info]   BxProtobuftools_LIBRARY      = '${BxProtobuftools_LIBRARY}'")
  message( STATUS "[info]   BxProtobuftools_LIBRARIES    = '${BxProtobuftools_LIBRARIES}'")
endif()

#-----------------------------------------------------------------------
# BxJsontools support
#
message (STATUS "[info] Searching for BxJsontools...")
find_package(BxJsontools 0.3 REQUIRED CONFIG COMPONENTS manager)
if (BxJsontools_FOUND)
  message( STATUS "[info] Found the BxJsontools library:")
  message( STATUS "[info]   BxJsontools_VERSION           = '${BxJsontools_VERSION}'")
  message( STATUS "[info]   BxJsontools_INCLUDE_DIRS      = '${BxJsontools_INCLUDE_DIRS}'")
  message( STATUS "[info]   BxJsontools_LIBRARIES         = '${BxJsontools_LIBRARIES}'")
endif()

#-----------------------------------------------------------------------
# BxRabbitMQ support
#
message (STATUS "[info] Searching for BxRabbitMQ...")
find_package(BxRabbitMQ 0.4.3 REQUIRED CONFIG COMPONENTS manager)
if (BxRabbitMQ_FOUND)
  message( STATUS "[info] Found the BxRabbitMQ library:")
  message( STATUS "[info]   BxRabbitMQ_VERSION           = '${BxRabbitMQ_VERSION}'")
  message( STATUS "[info]   BxRabbitMQ_INCLUDE_DIRS      = '${BxRabbitMQ_INCLUDE_DIRS}'")
  message( STATUS "[info]   BxRabbitMQ_LIBRARIES         = '${BxRabbitMQ_LIBRARIES}'")
  message( STATUS "[info]   BxRabbitMQ_MANAGER_LIBRARIES = '${BxRabbitMQ_MANAGER_LIBRARIES}'")
endif()

#-----------------------------------------------------------------------
# Readline support
#
find_package(Readline MODULE)
message (STATUS "[info] Readline_INCLUDE_DIR = '${Readline_INCLUDE_DIR}' ")
message (STATUS "[info] Readline_LIBRARIES   = '${Readline_LIBRARIES}' ")

message( STATUS "[info] Vire dependencies done.")

#-----------------------------------------------------------------------
# Qt5 support
#
if (VIRE_WITH_QT_GUI)
  set(VireLibrary_HEADERS_QT_TO_BE_MOCCED)
  find_package(Qt5Core    ${VIRE_QT5_VERSION} REQUIRED)
  find_package(Qt5Gui     ${VIRE_QT5_VERSION} REQUIRED)
  find_package(Qt5Widgets ${VIRE_QT5_VERSION} REQUIRED)
  find_package(Qt5Svg     ${VIRE_QT5_VERSION} REQUIRED)
  message (STATUS "[info] Include dirs         = ${Qt5Core_INCLUDE_DIRS}")
  message (STATUS "[info] Include private dirs = ${Qt5Core_PRIVATE_INCLUDE_DIRS}")
  message (STATUS "[info] Include dirs         = ${Qt5Gui_INCLUDE_DIRS}")
  message (STATUS "[info] Include private dirs = ${Qt5Gui_PRIVATE_INCLUDE_DIRS}")
  message (STATUS "[info] Include dirs         = ${Qt5Widgets_INCLUDE_DIRS}")
  message (STATUS "[info] Include private dirs = ${Qt5Widgets_PRIVATE_INCLUDE_DIRS}")
  include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Core_PRIVATE_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Widgets_PRIVATE_INCLUDE_DIRS}
    )
endif()

#-----------------------------------------------------------------------
# Readline support
#
find_package(Readline MODULE)

#-----------------------------------------------------------------------
message( STATUS "[info] End of Vire dependencies")

# end
