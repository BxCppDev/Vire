list(APPEND VireLibrary_HEADERS
  com/utils.h
  com/i_encoding_driver.h
  com/boost_serialization_encoding_driver.h
  com/json_encoding_driver.h
  com/protobuf_encoding_driver.h
  com/mailbox.h
  com/actor.h
  com/domain.h
  com/domain_builder.h
  # com/i_transport_driver.h
  #com/rabbitmq_transport_driver.h
  com/manager.h
  #com/manager-reflect.h
  )

list(APPEND VireLibrary_SOURCES
  com/utils.cc
  com/i_encoding_driver.cc
  com/boost_serialization_encoding_driver.cc
  com/json_encoding_driver.cc
  com/protobuf_encoding_driver.cc
  com/mailbox.cc
  com/actor.cc
  com/domain.cc
  com/domain_builder.cc
  # com/i_transport_driver.cc
  # #com/rabbitmq_transport_driver.cc
  com/manager.cc
  )

list(APPEND VireLibrary_TESTS
  com/testing/test-com-boost_serialization_encoding_driver.cxx
  com/testing/test-com-json_encoding_driver.cxx
  com/testing/test-com-protobuf_encoding_driver.cxx
  com/testing/test-com-mailbox.cxx
  com/testing/test-com-domain.cxx
  com/testing/test-com-actor.cxx
  com/testing/test-com-domain_builder.cxx
  com/testing/test-com-manager.cxx
  )
