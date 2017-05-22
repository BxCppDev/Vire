# Vire MOS Plugin material

#-----------------------------------------------------------------------
# Core configuration
#
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS -std=c++11)
endif()

# ----------------
# Dependencies
find_package(Boost ${VIRE_BOOST_MIN_VERSION}
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
set(_Vire_MOS_Boost_LIBRARIES
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
message (STATUS "[Vire_MOS] _Vire_MOS_Boost_LIBRARIES='${_Vire_MOS_Boost_LIBRARIES}'")
message (STATUS "[Vire_MOS] Boost_LOG_LIBRARY='${Boost_LOG_LIBRARY}'")
message (STATUS "[Vire_MOS] Boost_LOG_SETUP_LIBRARY='${Boost_LOG_SETUP_LIBRARY}'")
find_package(XercesC 3.1.1 REQUIRED)
message( STATUS "[Vire_MOS] XercesC_INCLUDE_DIRS=${XercesC_INCLUDE_DIRS}")
message( STATUS "[Vire_MOS] XercesC_LIBRARIES=${XercesC_LIBRARIES}")
find_package(CAMP 0.7.1 REQUIRED)
find_package(CLHEP 2.1.3.1 REQUIRED)
# find_package(Bayeux 3.0.0 REQUIRED)

# list(INSERT CMAKE_INSTALL_CMAKEDIR 0
#   "${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_CMAKEDIR}/${VIRE_TAG}")
#set(Vire_DIR "${VIRE_BUILD_PREFIX}/${CMAKE_INSTALL_CMAKEDIR}/${VIRE_TAG}")
#message( STATUS "[Vire_MOS] Vire_DIR = '${Vire_DIR}'")
#find_package(Vire 0.1.0 REQUIRED)
# message( STATUS "[Vire_MOS] Vire_INCLUDE_DIRS=${Vire_INCLUDE_DIRS}")
# message( STATUS "[Vire_MOS] Vire_LIBRARIES=${Vire_LIBRARIES}")
# message( STATUS "[Vire_MOS] Vire_DEFINITIONS=${Vire_DEFINITIONS}")

include_directories(${XercesC_INCLUDE_DIRS})
include_directories(${VIRE_BUILD_PREFIX}/include)
include_directories(${VIRE_BUILD_PREFIX}/include/vire)

#-----------------------------------------------------------------------
# - Vire MOS plugin library
add_subdirectory(viremos)

#-----------------------------------------------------------------------
# - Vire MOS XML to Vire device convertor
add_subdirectory(viremos_xml2viredev)

# - end