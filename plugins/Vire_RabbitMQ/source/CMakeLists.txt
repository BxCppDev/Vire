# Vire Plugin

#-----------------------------------------------------------------------
# Core configuration
#
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS -std=c++11)
endif()

# ----------------
# Dependencies
find_package(BxRabbitMQ 0.4 REQUIRED)
set(_Vire_RabbitMQ_BxRabbitMQ_LIBRARIES
  ${BxRabbitMQ_LIBRARIES}
  ${BxRabbitMQ_MANAGER_LIBRARIES}
  )
message (STATUS "[Vire_RabbitMQ] _Vire_RabbitMQ_BxRabbitMQ_LIBRARIES='${_Vire_RabbitMQ_BxRabbitMQ_LIBRARIES}'")

include_directories(${BxRabbitMQ_INCLUDE_DIRS})
include_directories(${VIRE_BUILD_PREFIX}/include)

#-----------------------------------------------------------------------
# Plugins:
# - Vire RabbitMQ interface plugin library
add_subdirectory(virerabbitmq)

#-----------------------------------------------------------------------
# Applications:

# - end