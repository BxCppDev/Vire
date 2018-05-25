list(APPEND VireLibrary_HEADERS
  cmsserver/utils.h
  cmsserver/resource_cardinality.h
  cmsserver/resource_pool.h
  cmsserver/parametrized_resource_specifications.h
  cmsserver/use_case_info.h
  cmsserver/use_case_info-serial.ipp
  #cmsserver/use_case_db.h
  cmsserver/session_reservation.h
  cmsserver/session_reservation-serial.ipp
  # cmsserver/session_manager.h
  cmsserver/base_use_case.h
  #cmsserver/shell_use_case.h
  cmsserver/session_info.h
  cmsserver/session.h
  cmsserver/client_connection.h
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
  cmsserver/parametrized_resource_specifications.cc
  cmsserver/use_case_info.cc
  #cmsserver/use_case_db.cc
  cmsserver/session_reservation.cc
  cmsserver/base_use_case.cc
  #cmsserver/shell_use_case.cc
  cmsserver/session_info.cc
  cmsserver/session.cc
  cmsserver/client_connection.cc
  cmsserver/session_possibility.cc
  # cmsserver/agenda.cc
  cmsserver/gate.cc
  # cmsserver/session_manager.cc
  cmsserver/server.cc
)

list(APPEND VireLibrary_TESTS
  cmsserver/testing/test-cmsserver-resource_cardinality.cxx
  cmsserver/testing/test-cmsserver-resource_pool.cxx
  cmsserver/testing/test-cmsserver-use_case_info.cxx
  cmsserver/testing/test-cmsserver-use_case.cxx
  cmsserver/testing/test-cmsserver-session_reservation.cxx
  # cmsserver/testing/test-cmsserver-base_use_case.cxx
  # cmsserver/testing/test-cmsserver-session_possibility.cxx
  # cmsserver/testing/test-cmsserver-agenda.cxx
  # cmsserver/testing/test-cmsserver-server.cxx
  cmsserver/testing/test-cmsserver-session.cxx
  cmsserver/testing/test-cmsserver-prs.cxx
)