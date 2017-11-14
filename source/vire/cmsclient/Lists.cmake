list(APPEND VireLibrary_HEADERS
  cmsclient/session_connection_parameters.h
  cmsclient/server_infos.h
  cmsclient/client.h
  #cmsclient/negotiation.h
  #cmsclient/session_manager.h
  )

list(APPEND VireLibrary_SOURCES
  cmsclient/session_connection_parameters.cc
  cmsclient/server_infos.cc
  cmsclient/client.cc
  #cmsclient/negotiation.cc
  #cmsclient/session_manager.cc
  )

list(APPEND VireLibrary_TESTS
  cmsclient/testing/test-cmsclient-server_infos.cxx
  #cmsclient/testing/test-cmsclient-client.cxx
  )