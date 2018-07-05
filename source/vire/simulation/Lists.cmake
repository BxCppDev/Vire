list(APPEND VireLibrary_HEADERS
  simulation/utils.h
  simulation/base_object.h
  simulation/i_measurable_temperature.h
  simulation/i_measurable_pressure.h
  simulation/i_measurable_humidity.h
  simulation/simple_device.h
  simulation/thermometer.h
  simulation/switchable_device.h
  simulation/room.h
  simulation/door.h
)

# list(APPEND VireLibrary_PRIVATE_HEADERS
# )

list(APPEND VireLibrary_SOURCES
  simulation/utils.cc
  simulation/base_object.cc
  simulation/simple_device.cc
  simulation/switchable_device.cc
  simulation/thermometer.cc
  simulation/room.cc
  simulation/door.cc
)

list(APPEND VireLibrary_TESTS
  simulation/testing/test-simulation-base_object.cxx
  simulation/testing/test-simulation-simple_device.cxx
  simulation/testing/test-simulation-switchable_device.cxx
  simulation/testing/test-simulation-door.cxx
)