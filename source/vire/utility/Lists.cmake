list(APPEND VireLibrary_HEADERS
  utility/rw_access.h
  utility/rw_access-reflect.h
  utility/base_payload.h
  utility/base_payload-serial.ipp
  utility/base_identifier.h
  utility/base_identifier-serial.ipp
  utility/instance_identifier.h
  utility/instance_identifier-serial.ipp
  utility/model_identifier.h
  utility/model_identifier-serial.ipp
  utility/base_error.h
  utility/base_error-serial.ipp
  utility/invalid_context_error.h
  utility/invalid_context_error-serial.ipp
  utility/invalid_setup_id_error.h
  utility/invalid_setup_id_error-serial.ipp
  utility/base_alarm.h
  utility/base_alarm-serial.ipp
  utility/base_alarm-serial.ipp
  utility/json_converters.h
  utility/payload_macros.h
  utility/metadata_record.h
  utility/metadata_record-serial.ipp
  utility/path.h
)

list(APPEND VireLibrary_SOURCES
  utility/rw_access.cc
  utility/base_identifier.cc
  utility/instance_identifier.cc
  utility/model_identifier.cc
  utility/base_error.cc
  utility/invalid_context_error.cc
  utility/invalid_setup_id_error.cc
  utility/base_payload.cc
  utility/base_alarm.cc
  utility/json_converters.cc
  utility/metadata_record.cc
  utility/path.cc
)

list(APPEND VireLibrary_TESTS
  utility/testing/test-utility-base_payload.cxx
  utility/testing/test-utility-base_alarm.cxx
  utility/testing/test-utility-base_error.cxx
  utility/testing/test-utility-invalid_context_error.cxx
  utility/testing/test-utility-invalid_setup_id_error.cxx
  utility/testing/test-utility-identifiers.cxx
  utility/testing/test-utility-path.cxx
)