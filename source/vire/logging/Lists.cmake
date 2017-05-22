list(APPEND VireLibrary_HEADERS
  logging/simple_logging_service.h
  logging/simple_logging_service-reflect.h
  #logging/ui/log_commands.h
  #logging/ui/log_command_interface.h
  )

list(APPEND VireLibrary_SOURCES
  logging/simple_logging_service.cc
  #logging/ui/log_commands.cc
  #logging/ui/log_command_interface.cc
  )

list(APPEND VireLibrary_TESTS
  logging/testing/test-logging-simple_logging_service.cxx
  )
