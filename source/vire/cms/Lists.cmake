list(APPEND VireLibrary_HEADERS
  # general types:
  cms/utils.h
  cms/status.h
  cms/status-reflect.h
  cms/image.h
  cms/resource_status_record.h
  cms/resource_status_record-serial.ipp
  # cms/method_argument.h
  # cms/method_argument-serial.ipp
  # cms/image.h
  # #cms/experiment_image_registry.h
  # # error types:
  # cms/invalid_resource_error.h
  # cms/invalid_resource_error-serial.ipp
  # cms/no_pubsub_resource_error.h
  # cms/no_pubsub_resource_error-serial.ipp
  # cms/unknown_resources_error.h
  # cms/unknown_resources_error-serial.ipp
  # cms/invalid_status_error.h
  # cms/invalid_status_error-serial.ipp
  # cms/invalid_user_error.h
  # cms/invalid_user_error-serial.ipp
  # cms/invalid_credentials_error.h
  # cms/invalid_credentials_error-serial.ipp
  # cms/resource_exec_error.h
  # cms/resource_exec_error-serial.ipp
  # cms/argument_error.h
  # cms/argument_error-serial.ipp
  # request/response types:
  # cms/resource_exec.h
  # cms/resource_exec-serial.ipp
  # cms/resource_exec_success.h
  # cms/resource_exec_success-serial.ipp
  # cms/resource_exec_failure.h
  # cms/resource_exec_failure-serial.ipp
  # cms/resource_fetch_status.h
  # cms/resource_fetch_status-serial.ipp
  # cms/resource_fetch_status_success.h
  # cms/resource_fetch_status_success-serial.ipp
  # cms/resource_fetch_status_failure.h
  # cms/resource_fetch_status_failure-serial.ipp
  # cms/resource_pubsub.h
  # cms/resource_pubsub-serial.ipp
  # cms/resource_pubsub_success.h
  # cms/resource_pubsub_success-serial.ipp
  # cms/resource_pubsub_failure.h
  # cms/resource_pubsub_failure-serial.ipp
  )

list(APPEND VireLibrary_PRIVATE_HEADERS
)

list(APPEND VireLibrary_SOURCES
  # general types:
  cms/utils.cc
  cms/status.cc
  cms/image.cc
  cms/resource_status_record.cc
  # cms/method_argument.cc
  #cms/experiment_image_registry.cc
  # error types:
  # cms/invalid_resource_error.cc
  # cms/no_pubsub_resource_error.cc
  # cms/unknown_resources_error.cc
  # cms/invalid_status_error.cc
  # cms/invalid_user_error.cc
  # cms/invalid_credentials_error.cc
  # cms/resource_exec_error.cc
  # cms/argument_error.cc
  # request/response types:
  # cms/resource_base_request.cc
  # cms/resource_base_response.cc
  # cms/resource_fetch_status_request.cc
  # cms/resource_fetch_status_success_response.cc
  # cms/resource_fetch_status_failure_response.cc
  # cms/resource_exec_request.cc
  # cms/resource_exec_success_response.cc
  # cms/resource_exec_failure_response.cc
  # # cms/resource_pubsub_request.cc
  # cms/resource_status_change_event.cc
  # cms/device_status_change_event.cc
)

list(APPEND VireLibrary_TESTS
  cms/testing/test-cms-utils.cxx
  cms/testing/test-cms-image.cxx
  cms/testing/test-cms-resource_status_record.cxx
#   cms/testing/test-cms-invalid_resource_error.cxx
#   cms/testing/test-cms-invalid_status_error.cxx
#   cms/testing/test-cms-resource_exec_error.cxx
#   cms/testing/test-cms-invalid_user_error.cxx
#   cms/testing/test-cms-invalid_credentials_error.cxx
#   cms/testing/test-cms-argument_error.cxx
#   cms/testing/test-cms-method_argument.cxx
#   cms/testing/test-cms-resource_base_request.cxx
#   cms/testing/test-cms-resource_base_response.cxx
#   cms/testing/test-cms-resource_fetch_status_request.cxx
#   cms/testing/test-cms-resource_fetch_status_success_response.cxx
#   cms/testing/test-cms-resource_fetch_status_failure_response.cxx
#   cms/testing/test-cms-resource_exec_request.cxx
#   cms/testing/test-cms-resource_exec_success_response.cxx
#   cms/testing/test-cms-resource_exec_failure_response.cxx
#   cms/testing/test-cms-resource_status_change_event.cxx
#   cms/testing/test-cms-device_status_change_event.cxx
)