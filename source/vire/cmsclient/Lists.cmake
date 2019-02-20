list(APPEND VireLibrary_HEADERS
  cmsclient/session_connection_parameters.h
  cmsclient/setup_infos.h
  cmsclient/setup_infos_tui.h
  cmsclient/client.h
  cmsclient/negotiation_service.h
  #cmsclient/negotiation.h
  #cmsclient/session_manager.h
  )

list(APPEND VireLibrary_SOURCES
  cmsclient/session_connection_parameters.cc
  cmsclient/setup_infos.cc
  cmsclient/setup_infos_tui.cc
  cmsclient/client.cc
  cmsclient/negotiation_service.cc
  #cmsclient/negotiation.cc
  #cmsclient/session_manager.cc
  )

list(APPEND VireLibrary_TESTS
  cmsclient/testing/test-cmsclient-setup_infos.cxx
  cmsclient/testing/test-cmsclient-setup_infos_tui.cxx
  cmsclient/testing/test-cmsclient-client.cxx
  )