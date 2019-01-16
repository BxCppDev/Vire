 set(VIRE_CMS_HEADERS_QT_TO_BE_MOCCED
   cms/image_status.h
   cms/ui/image_status_panel.h
   cms/base_image.h 
   cms/ui/image_panel.h
   cms/monitoring_record.h 
   cms/ui/image_registry_panel.h
   )
 
 QT5_WRAP_CPP(VIRE_CMS_HEADERS_QT_MOC
   ${VIRE_CMS_HEADERS_QT_TO_BE_MOCCED}
   )
  
 list(APPEND VireLibrary_HEADERS
  cms/utils.h
  cms/ui/utils.h
  cms/application.h
  cms/status.h
  cms/status-reflect.h
  cms/monitoring.h
  cms/image_status.h
  cms/ui/image_status_panel.h
  cms/monitoring_record.h
  cms/base_image.h
  cms/resource_image.h
  cms/device_image.h
  cms/resource_status_record.h
  cms/resource_status_record-serial.ipp
  cms/method_argument.h
  cms/method_argument-serial.ipp
  cms/image_registry.h
  cms/ui/image_registry_panel.h

  cms/invalid_resource_error.h
  cms/invalid_resource_error-serial.ipp
  cms/no_pubsub_resource_error.h
  cms/no_pubsub_resource_error-serial.ipp
  cms/unknown_resources_error.h
  cms/unknown_resources_error-serial.ipp
  cms/invalid_status_error.h
  cms/invalid_status_error-serial.ipp
  cms/invalid_user_error.h
  cms/invalid_user_error-serial.ipp
  cms/invalid_credentials_error.h
  cms/invalid_credentials_error-serial.ipp
  cms/resource_exec_error.h
  cms/resource_exec_error-serial.ipp
  cms/argument_error.h
  cms/argument_error-serial.ipp
  cms/timeout_error.h
  cms/timeout_error-serial.ipp

  cms/resource_exec.h
  cms/resource_exec-serial.ipp
  cms/resource_exec_success.h
  cms/resource_exec_success-serial.ipp
  cms/resource_exec_failure.h
  cms/resource_exec_failure-serial.ipp

  cms/resource_fetch_status.h
  cms/resource_fetch_status-serial.ipp
  cms/resource_fetch_status_success.h
  cms/resource_fetch_status_success-serial.ipp
  cms/resource_fetch_status_failure.h
  cms/resource_fetch_status_failure-serial.ipp

  cms/resource_pubsub.h
  cms/resource_pubsub-serial.ipp
  cms/resource_pubsub_success.h
  cms/resource_pubsub_success-serial.ipp
  cms/resource_pubsub_failure.h
  cms/resource_pubsub_failure-serial.ipp

  cms/resource_status_change.h
  cms/resource_status_change-serial.ipp
  cms/device_status_change.h
  cms/device_status_change-serial.ipp

  cms/connection_request.h
  cms/connection_request-serial.ipp
  cms/connection_success.h
  cms/connection_success-serial.ipp
  cms/connection_failure.h
  cms/connection_failure-serial.ipp
  cms/disconnection_request.h
  cms/disconnection_request-serial.ipp
  cms/disconnection_success.h
  cms/disconnection_success-serial.ipp
  cms/disconnection_failure.h
  cms/disconnection_failure-serial.ipp

  cms/base_rpc_worker.h

  )

list(APPEND VireLibrary_PRIVATE_HEADERS
)

list(APPEND VireLibrary_SOURCES
  ${VIRE_CMS_HEADERS_QT_MOC}
  cms/utils.cc
  cms/ui/utils.cc
  cms/application.cc
  cms/status.cc
  cms/monitoring.cc
  cms/image_status.cc
  cms/ui/image_status_panel.cc
  cms/monitoring_record.cc
  cms/resource_status_record.cc
  cms/method_argument.cc
  cms/base_image.cc
  cms/ui/image_panel.cc
  cms/resource_image.cc
  cms/device_image.cc
  cms/image_registry.cc
  cms/ui/image_registry_panel.cc

  cms/invalid_resource_error.cc
  cms/no_pubsub_resource_error.cc
  cms/unknown_resources_error.cc
  cms/invalid_status_error.cc
  cms/invalid_user_error.cc
  cms/invalid_credentials_error.cc
  cms/resource_exec_error.cc
  cms/argument_error.cc
  cms/timeout_error.cc

  cms/resource_fetch_status.cc
  cms/resource_fetch_status_success.cc
  cms/resource_fetch_status_failure.cc

  cms/resource_exec.cc
  cms/resource_exec_success.cc
  cms/resource_exec_failure.cc

  cms/resource_pubsub.cc
  cms/resource_pubsub_success.cc
  cms/resource_pubsub_failure.cc

  cms/resource_status_change.cc
  cms/device_status_change.cc

  cms/connection_request.cc
  cms/connection_success.cc
  cms/connection_failure.cc
  cms/disconnection_request.cc
  cms/disconnection_success.cc
  cms/disconnection_failure.cc

  cms/base_rpc_worker.cc
  )

list(APPEND VireLibrary_TESTS
  cms/testing/test-cms-utils.cxx
  cms/testing/test-cms-image_status.cxx
  cms/testing/test-cms-monitoring.cxx
  cms/testing/test-cms-image.cxx
  cms/testing/test-cms-image_registry.cxx
  cms/testing/test-cms-resource_status_record.cxx

  cms/testing/test-cms-invalid_resource_error.cxx
  cms/testing/test-cms-invalid_status_error.cxx
  cms/testing/test-cms-invalid_user_error.cxx
  cms/testing/test-cms-invalid_credentials_error.cxx
  cms/testing/test-cms-argument_error.cxx
  cms/testing/test-cms-method_argument.cxx
  cms/testing/test-cms-resource_exec_error.cxx
  cms/testing/test-cms-timeout_error.cxx

  cms/testing/test-cms-resource_fetch_status.cxx
  cms/testing/test-cms-resource_fetch_status_success.cxx
  cms/testing/test-cms-resource_fetch_status_failure.cxx

  cms/testing/test-cms-resource_exec.cxx
  cms/testing/test-cms-resource_exec_success.cxx
  cms/testing/test-cms-resource_exec_failure.cxx

  cms/testing/test-cms-resource_pubsub.cxx
  cms/testing/test-cms-resource_pubsub_success.cxx
  cms/testing/test-cms-resource_pubsub_failure.cxx

  cms/testing/test-cms-resource_status_change.cxx
  cms/testing/test-cms-device_status_change.cxx

  cms/testing/test-cms-connection_request.cxx
  cms/testing/test-cms-connection_failure.cxx
  cms/testing/test-cms-disconnection_request.cxx
  cms/testing/test-cms-disconnection_success.cxx
  cms/testing/test-cms-base_rpc_worker.cxx

)