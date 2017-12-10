
message (STATUS "[info] Begin of 'source/java/CMakeLists.txt'.")

if (VIRE_WITH_JAVA)
  message( STATUS "[info] Build JAR file from Java sources...")
endif()

configure_file(vire/version.java.in
  ${CMAKE_CURRENT_BINARY_DIR}/vire/version.java
  @ONLY)

set(VireLibrary_java_SOURCES
  vire/vire.java
  ${CMAKE_CURRENT_BINARY_DIR}/vire/version.java
  vire/hello_vire.java
  vire/protobufable_interface.java
  vire/tree_dumpable_interface.java
  vire/utility/base_identifier.java
  vire/utility/instance_identifier.java
  vire/utility/model_identifier.java
  vire/utility/metadata_record.java
  vire/utility/error_code.java
  vire/utility/base_error.java
  vire/utility/base_payload.java
  vire/utility/base_event.java
  vire/utility/base_alarm.java
  vire/utility/base_request.java
  vire/utility/invalid_context_error.java
  vire/utility/invalid_setup_id_error.java
  vire/message/message_identifier.java
  vire/message/message_header.java
  vire/message/message_body.java
  vire/message/message.java
  vire/cms/resource_status_record.java
  vire/cms/method_argument.java
  #vire/cms/resource_exec_request.java
  #vire/cms/resource_exec_base_response.java
  )

set(VIRE_JAVA_JAR Vire.jar)
set(CMAKE_JAVA_INCLUDE_PATH ${VIRE_JAVA_JAR})
add_jar(VireJava
  SOURCES ${VireLibrary_java_SOURCES}
  OUTPUT_NAME VireJava
  # OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib/java
  MANIFEST
  vire/Manifest.txt
  )
get_target_property(_jarFile VireJava JAR_FILE)
get_target_property(_classDir VireJava CLASSDIR)
message(STATUS "[info] Jar file          : '${_jarFile}'")
message(STATUS "[info] Class compiled to : '${_classDir}'")
install_jar(VireJava ${CMAKE_INSTALL_LIBDIR}/java)

message (STATUS "[info] End of 'source/java/CMakeLists.txt'.")
