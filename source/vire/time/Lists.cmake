list(APPEND VireLibrary_HEADERS
  time/utils.h
  time/rt_utils.h
  time/time_interval_cut.h
  # time/time_interval_cut-reflect.h
  time/time_availability.h
  # time/time_availability-reflect.h
  time/time_service.h
  time/duration_interval.h
)

list(APPEND VireLibrary_SOURCES
  time/utils.cc
  time/rt_utils.cc
  time/time_interval_cut.cc
  time/time_availability.cc
  time/time_service.cc
  time/duration_interval.cc
)

list(APPEND VireLibrary_TESTS
  time/testing/test-time-utils.cxx
  time/testing/test-time-time_interval_cut.cxx
  time/testing/test-time-time_availability.cxx
  time/testing/test-time-time_service.cxx
)
