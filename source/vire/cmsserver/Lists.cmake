list(APPEND VireLibrary_HEADERS
  cmsserver/utils.h
  cmsserver/resource_cardinality.h
  cmsserver/resource_pool.h
  cmsserver/session_info.h
  cmsserver/session_info-serial.ipp
  cmsserver/session_reservation.h
  cmsserver/session_reservation-serial.ipp
  # cmsserver/session_manager.h
  cmsserver/base_use_case.h
  cmsserver/shell_use_case.h
  cmsserver/session.h
  cmsserver/session_possibility.h
  # cmsserver/agenda.h
  cmsserver/gate.h
  cmsserver/server.h
)

list(APPEND VireLibrary_PRIVATE_HEADERS
)

list(APPEND VireLibrary_SOURCES
  cmsserver/utils.cc
  cmsserver/resource_cardinality.cc
  cmsserver/resource_pool.cc
  cmsserver/session_info.cc
  cmsserver/session_reservation.cc
  cmsserver/base_use_case.cc
  cmsserver/shell_use_case.cc
  cmsserver/session.cc
  cmsserver/session_possibility.cc
  # cmsserver/agenda.cc
  cmsserver/gate.cc
  # cmsserver/session_manager.cc
  cmsserver/server.cc
)

list(APPEND VireLibrary_TESTS
  cmsserver/testing/test-cmsserver-resource_cardinality.cxx
  cmsserver/testing/test-cmsserver-resource_pool.cxx
  cmsserver/testing/test-cmsserver-session_info.cxx
  cmsserver/testing/test-cmsserver-session_reservation.cxx
  # cmsserver/testing/test-cmsserver-session_possibility.cxx
  # cmsserver/testing/test-cmsserver-agenda.cxx
  # cmsserver/testing/test-cmsserver-server.cxx
  cmsserver/testing/test-cmsserver-session.cxx
)