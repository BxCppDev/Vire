# Vire Plugin

#-----------------------------------------------------------------------
# Core configuration
#
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS -std=c++11)
endif()

# ----------------
# Dependencies

# #-----------------------------------------------------------------------
# # BxProtobuftools support
# #
# message (STATUS "[info] Searching for BxProtobuftools...")
# find_package(BxProtobuftools 0.3 REQUIRED CONFIG)
# if (BxProtobuftools_FOUND)
#   message( STATUS "[info] Found the BxProtobuftools library:")
#   message( STATUS "[info]   BxProtobuftools_VERSION      = '${BxProtobuftools_VERSION}'")
#   message( STATUS "[info]   BxProtobuftools_DEFINITIONS  = '${BxProtobuftools_DEFINITIONS}'")
#   message( STATUS "[info]   BxProtobuftools_INCLUDE_DIR  = '${BxProtobuftools_INCLUDE_DIR}'")
#   message( STATUS "[info]   BxProtobuftools_INCLUDE_DIRS = '${BxProtobuftools_INCLUDE_DIRS}'")
#   message( STATUS "[info]   BxProtobuftools_LIBRARY      = '${BxProtobuftools_LIBRARY}'")
#   message( STATUS "[info]   BxProtobuftools_LIBRARIES    = '${BxProtobuftools_LIBRARIES}'")
# endif()

#-----------------------------------------------------------------------
# BxRabbitMQ support
#
message (STATUS "[info] Searching for BxRabbitMQ...")
find_package(BxRabbitMQ 0.4.1 REQUIRED CONFIG COMPONENTS manager)
if (BxRabbitMQ_FOUND)
  message( STATUS "[info] Found the BxRabbitMQ library:")
  message( STATUS "[info]   BxRabbitMQ_VERSION           = '${BxRabbitMQ_VERSION}'")
  message( STATUS "[info]   BxRabbitMQ_INCLUDE_DIRS      = '${BxRabbitMQ_INCLUDE_DIRS}'")
  message( STATUS "[info]   BxRabbitMQ_LIBRARIES         = '${BxRabbitMQ_LIBRARIES}'")
  message( STATUS "[info]   BxRabbitMQ_MANAGER_LIBRARIES = '${BxRabbitMQ_MANAGER_LIBRARIES}'")
endif()
set(_Vire_RabbitMQ_BxRabbitMQ_LIBRARIES
  ${BxRabbitMQ_LIBRARIES}
  ${BxRabbitMQ_MANAGER_LIBRARIES}
  )
message (STATUS "[Vire_RabbitMQ] _Vire_RabbitMQ_BxRabbitMQ_LIBRARIES='${_Vire_RabbitMQ_BxRabbitMQ_LIBRARIES}'")

message (STATUS "[Vire_RabbitMQ] BxJsontools_INCLUDE_DIRS     = '${BxJsontools_INCLUDE_DIRS}'" )
message (STATUS "[Vire_RabbitMQ] BxProtobuftools_INCLUDE_DIRS = '${BxProtobuftools_INCLUDE_DIRS}'" )
message (STATUS "[Vire_RabbitMQ] BxRabbitMQ_INCLUDE_DIRS      = '${BxRabbitMQ_INCLUDE_DIRS}'" )
include_directories(${BxJsontools_INCLUDE_DIRS})
include_directories(${BxRabbitMQ_INCLUDE_DIRS})
include_directories(${BxProtobuftools_INCLUDE_DIRS})
message (STATUS "[Vire_RabbitMQ] VIRE_BUILD_PREFIX='${VIRE_BUILD_PREFIX}'")
include_directories(${VIRE_BUILD_PREFIX}/include)

#-----------------------------------------------------------------------
# Plugins:
# - Vire RabbitMQ interface plugin library
add_subdirectory(virerabbitmq)

#-----------------------------------------------------------------------
# Applications:

# - end