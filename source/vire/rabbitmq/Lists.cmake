list(APPEND VireLibrary_HEADERS
  rabbitmq/permissions_utils.h
  rabbitmq/user.h
  rabbitmq/manager_service.h
  rabbitmq/transport_manager.h
  rabbitmq/service_client_plug.h
  rabbitmq/event_emitter_plug.h
  )

list(APPEND VireLibrary_SOURCES
  rabbitmq/permissions_utils.cc
  rabbitmq/user.cc
  rabbitmq/manager_service.cc
  rabbitmq/transport_manager.cc
  rabbitmq/service_client_plug.cc
  rabbitmq/event_emitter_plug.cc
  )

list(APPEND VireLibrary_TESTS
  rabbitmq/testing/test-rabbitmq-manager_service.cxx
  rabbitmq/testing/test-rabbitmq-transport_manager.cxx
  )
