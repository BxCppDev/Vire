list(APPEND VireLibrary_HEADERS
  message/body_layout.h
  message/protocol_utils.h
  message/message_identifier.h
  message/message_identifier-serial.ipp
  message/message_header.h
  message/message_header-serial.ipp
  message/message_body.h
  message/message_body-inl.h
  message/message_body-serial.ipp
  message/message.h
  message/message-serial.ipp
)

list(APPEND VireLibrary_SOURCES
  message/body_layout.cc
  message/protocol_utils.cc
  message/message_identifier.cc
  message/message_header.cc
  message/message_body.cc
  message/message.cc
)

list(APPEND VireLibrary_TESTS
  message/testing/test-message-message_header.cxx
  message/testing/test-message-message_body.cxx
  message/testing/test-message-message.cxx
)