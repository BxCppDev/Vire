list(APPEND VireLibrary_HEADERS
  cmsserver/utils.h
  cmsserver/resource_cardinality.h
  cmsserver/resource_pool.h
  cmsserver/process_info.h
  cmsserver/process_info-serial.ipp
  cmsserver/session_info.h
  cmsserver/session_info-serial.ipp
  cmsserver/session_reservation.h
  cmsserver/session_reservation-serial.ipp
  cmsserver/agenda.h
  # cmsserver/session.h
  # cmsserver/base_process.h
  # cmsserver/dummy_process.h
  # cmsserver/root_process.h
  # cmsserver/kernel.h
  #cmsserver/session_possibility.h
)

list(APPEND VireLibrary_PRIVATE_HEADERS
  # cmsserver/detail/process_macros.h
)

list(APPEND VireLibrary_SOURCES
  cmsserver/utils.cc
  cmsserver/resource_cardinality.cc
  cmsserver/resource_pool.cc
  cmsserver/process_info.cc
  cmsserver/session_info.cc
  cmsserver/session_reservation.cc
  cmsserver/agenda.cc
  # cmsserver/session.cc
  # cmsserver/base_process.cc
  # cmsserver/dummy_process.cc
  # cmsserver/root_process.cc
  # cmsserver/kernel.cc
  #cmsserver/session_possibility.cc
)

list(APPEND VireLibrary_TESTS
  cmsserver/testing/test-cmsserver-resource_cardinality.cxx
  cmsserver/testing/test-cmsserver-resource_pool.cxx
  cmsserver/testing/test-cmsserver-process_info.cxx
  cmsserver/testing/test-cmsserver-session_info.cxx
  cmsserver/testing/test-cmsserver-session_reservation.cxx
  cmsserver/testing/test-cmsserver-agenda.cxx
  # # cmsserver/testing/test-cmsserver-session.cxx
  # cmsserver/testing/test-cmsserver-kernel.cxx
  #cmsserver/testing/test-cmsserver-session_possibility.cxx
)