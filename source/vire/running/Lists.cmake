list(APPEND VireLibrary_HEADERS
  running/utils.h
  running/scheduling.h
  running/run_control.h
  running/i_runnable.h
)

# list(APPEND VireLibrary_PRIVATE_HEADERS
# )

list(APPEND VireLibrary_SOURCES
  running/utils.cc
  running/scheduling.cc
  running/run_control.cc
)

list(APPEND VireLibrary_TESTS
  #running/testing/test-running-xxx.cxx
)