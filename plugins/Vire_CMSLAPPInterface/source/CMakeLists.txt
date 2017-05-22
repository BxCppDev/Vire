# Vire Plugin

#-----------------------------------------------------------------------
# Core configuration
#
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS -std=c++11)
endif()

# ----------------
# Dependencies
find_package(Boost 1.60.0
  REQUIRED
  filesystem
  system
  serialization
  iostreams
  program_options
  date_time
  thread
  regex
  log
  )
set(_Vire_CMSLAPP_Boost_LIBRARIES
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_SERIALIZATION_LIBRARY}
  ${Boost_IOSTREAMS_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  ${Boost_LOG_LIBRARY}
  ${Boost_LOG_SETUP_LIBRARY}
  )
message (STATUS "_Vire_CMSLAPP_Boost_LIBRARIES='${_Vire_CMSLAPP_Boost_LIBRARIES}'")
message (STATUS "Boost_LOG_LIBRARY='${Boost_LOG_LIBRARY}'")
message (STATUS "Boost_LOG_SETUP_LIBRARY='${Boost_LOG_SETUP_LIBRARY}'")
find_package(CAMP   0.7.1   REQUIRED)
find_package(CLHEP  2.1.3.1 REQUIRED)
# find_package(Bayeux 3.0.0   REQUIRED)
# message( STATUS "Bayeux_INCLUDE_DIRS=${Bayeux_INCLUDE_DIRS}")
# message( STATUS "Bayeux_LIBRARIES=${Bayeux_LIBRARIES}")

# list(INSERT CMAKE_INSTALL_CMAKEDIR 0
#   "${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_CMAKEDIR}/${VIRE_TAG}")
set(Vire_DIR "${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_CMAKEDIR}/${VIRE_TAG}")
find_package(Vire 0.1.0 REQUIRED)
message( STATUS "Vire_INCLUDE_DIRS=${Vire_INCLUDE_DIRS}")
message( STATUS "Vire_LIBRARIES=${Vire_LIBRARIES}")
message( STATUS "Vire_DEFINITIONS=${Vire_DEFINITIONS}")

include_directories(${Bayeux_INCLUDE_DIRS})
include_directories(${VIRE_BUILD_PREFIX}/include)
# include_directories(${Vire_INCLUDE_DIRS})

#-----------------------------------------------------------------------
# BxJsontools support
#
find_package(BxJsontools REQUIRED)
if (BxJsontools_FOUND)
  message( STATUS "Found the BxJsontools library:")
  message( STATUS "  BxJsontools_VERSION='${BxJsontools_VERSION}'")
  message( STATUS "  BxJsontools_INCLUDE_DIRS='${BxJsontools_INCLUDE_DIRS}'")
  message( STATUS "  BxJsontools_LIBRARY='${BxJsontools_LIBRARY}'")
endif()
#-----------------------------------------------------------------------
# Protobuf support
#
find_package(Protobuf 3.0 REQUIRED)
find_package(BxProtobuftools REQUIRED)
include_directories(${BxJsontools_INCLUDE_DIRS})
include_directories(${BxProtobuftools_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${VIRE_BUILD_PREFIX}/include/protobuf/cpp)

#-----------------------------------------------------------------------
# Plugins:
# - Vire CMS/LAPP interface plugin library
add_subdirectory(virecmslapp)

#-----------------------------------------------------------------------
# Applications:

# - end