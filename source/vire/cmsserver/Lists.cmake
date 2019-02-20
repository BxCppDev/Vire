 set(VIRE_CMSSERVER_HEADERS_QT_TO_BE_MOCCED
   cmsserver/sc_driver.h
   cmsserver/ui/sc_driver_panel.h
   cmsserver/ui/sc_manager_panel.h
   cmsserver/server.h
   cmsserver/ui/server_panel.h
   )
 
 QT5_WRAP_CPP(VIRE_CMSSERVER_HEADERS_QT_MOC
   ${VIRE_CMSSERVER_HEADERS_QT_TO_BE_MOCCED}
   )

 list(APPEND VireLibrary_HEADERS
  cmsserver/utils.h
  cmsserver/running.h
  cmsserver/resource_cardinality.h
  cmsserver/resource_pool.h
  cmsserver/uc_utils.h
  cmsserver/uc_parametrised_resource_specifications.h
  cmsserver/uc_resource_mount_link.h
  cmsserver/uc_resource_mount_link-serial.ipp
  cmsserver/uc_functional_resource_specifications.h
  cmsserver/uc_functional_resource_specifications-serial.ipp
  cmsserver/uc_model_description.h
  cmsserver/uc_model_description-serial.ipp
  cmsserver/uc_model_db.h
  cmsserver/uc_model_db-serial.ipp
  cmsserver/uc_error.h
  cmsserver/uc_time_constraints.h
  cmsserver/uc_resource_constraints.h
  cmsserver/uc_composition_description.h
  cmsserver/uc_composition_description-serial.ipp
  cmsserver/uc_mounting_table.h
  cmsserver/uc_scope_resources.h
  cmsserver/uc_scope_resources-serial.ipp
  cmsserver/base_use_case.h
  cmsserver/uc_factory.h
  #cmsserver/temperature_monitoring_use_case.h
  #cmsserver/lock_use_case.h
  cmsserver/sc_driver.h
  cmsserver/ui/sc_driver_panel.h
  cmsserver/sc_manager.h
  cmsserver/ui/sc_manager_panel.h
  # cmsserver/session_reservation.h
  # cmsserver/session_reservation-serial.ipp
  # cmsserver/session_manager.h
  # cmsserver/sequence_use_case.h
  # cmsserver/parallel_use_case.h
  #cmsserver/shell_use_case.h
  #cmsserver/session_info.h
  #cmsserver/session.h
  #cmsserver/client_connection.h
  #cmsserver/session_possibility.h
  # cmsserver/agenda.h
  cmsserver/gate.h
  cmsserver/server.h
  cmsserver/ui/server_panel.h
)

list(APPEND VireLibrary_PRIVATE_HEADERS
)

list(APPEND VireLibrary_SOURCES
  cmsserver/utils.cc
  cmsserver/running.cc
  cmsserver/resource_cardinality.cc
  cmsserver/resource_pool.cc
  cmsserver/uc_utils.cc
  cmsserver/uc_resource_mount_link.cc
  cmsserver/uc_parametrised_resource_specifications.cc
  cmsserver/uc_functional_resource_specifications.cc
  cmsserver/uc_model_description.cc
  cmsserver/uc_model_db.cc
  cmsserver/uc_error.cc
  cmsserver/uc_composition_description.cc
  cmsserver/uc_mounting_table.cc
  cmsserver/uc_scope_resources.cc
  cmsserver/uc_time_constraints.cc
  cmsserver/uc_resource_constraints.cc
  cmsserver/base_use_case.cc
  cmsserver/uc_factory.cc
  #cmsserver/temperature_monitoring_use_case.cc
  #cmsserver/lock_use_case.cc
  cmsserver/sc_driver.cc
  cmsserver/sc_manager.cc
  cmsserver/ui/sc_driver_panel.cc
  cmsserver/ui/sc_manager_panel.cc
  # cmsserver/session_reservation.cc
  # cmsserver/sequence_use_case.cc
  # cmsserver/parallel_use_case.cc
  #cmsserver/shell_use_case.cc
  #cmsserver/session_info.cc
  #cmsserver/session.cc
  #cmsserver/client_connection.cc
  #cmsserver/session_possibility.cc
  # cmsserver/agenda.cc
  cmsserver/gate.cc
  # cmsserver/session_manager.cc
  cmsserver/server.cc
  cmsserver/ui/server_panel.cc
  ${VIRE_CMSSERVER_HEADERS_QT_MOC}
)

list(APPEND VireLibrary_TESTS
  cmsserver/testing/test-cmsserver-resource_cardinality.cxx
  cmsserver/testing/test-cmsserver-resource_pool.cxx
  cmsserver/testing/test-cmsserver-prs.cxx
  cmsserver/testing/test-cmsserver-uc_model_description.cxx
  cmsserver/testing/test-cmsserver-uc_model_db.cxx
  cmsserver/testing/test-cmsserver-uc_composition_description.cxx
  cmsserver/testing/test-cmsserver-uc_mounting_table.cxx
  cmsserver/testing/test-cmsserver-uc_time_constraints.cxx
  cmsserver/testing/test-cmsserver-uc_factory.cxx
  #cmsserver/testing/test-cmsserver-base_use_case.cxx
  #cmsserver/testing/test-cmsserver-lock_use_case.cxx
  cmsserver/testing/test-cmsserver-sc_driver.cxx
  #cmsserver/testing/test-cmsserver-use_case.cxx
  # cmsserver/testing/test-cmsserver-session_reservation.cxx
  # cmsserver/testing/test-cmsserver-session_possibility.cxx
  # cmsserver/testing/test-cmsserver-agenda.cxx
  cmsserver/testing/test-cmsserver-gate.cxx
  cmsserver/testing/test-cmsserver-server.cxx
  # cmsserver/testing/test-cmsserver-session.cxx
)