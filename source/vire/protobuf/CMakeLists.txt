# - Protocol Buffer Support for Vire

message (STATUS "[info] Begin of 'source/vire/protobuf/CMakeLists.txt'.")

message( STATUS "[info] *******************************************")
message( STATUS "[info] * Google Protocol Buffer Support for Vire *")
message( STATUS "[info] *******************************************")

set(_gen_pb_code 0)
set(_gen_pb_java)
set(VireProtobufLibrary_cpp_HEADERS)
set(VireProtobufLibrary_cpp_SOURCES)
set(VireProtobufLibrary_cpp_TARGETS)
set(VireProtobufLibrary_java_TARGETS)
if (VIRE_WITH_PROTOBUF_JAVA)
  message( STATUS "[info] Generating C++ & Java source stubs from protobuf files...")
  set(_gen_pb_java "--java")
  set(VireProtobufLibrary_java_SOURCES)
else()
  message( STATUS "[info] Generating C++ source stubs from protobuf files...")
endif()

set(protobufList

  # vire/utility:
  vire/utility/BasePayload.proto
  vire/utility/BaseIdentifier.proto
  vire/utility/InstanceIdentifier.proto
  vire/utility/ModelIdentifier.proto
  vire/utility/BaseAlarm.proto
  vire/utility/BaseError.proto
  vire/utility/InvalidContextError.proto
  vire/utility/InvalidSetupIdError.proto
  vire/utility/MetadataRecord.proto
  vire/utility/Properties.proto

  # vire/message:
  vire/message/MessageIdentifier.proto
  vire/message/MessageHeader.proto
  vire/message/MessageBody.proto
  vire/message/Message.proto

  # vire/cms:
  vire/cms/ArgumentError.proto
  vire/cms/InvalidCredentialsError.proto
  vire/cms/InvalidResourceError.proto
  vire/cms/InvalidStatusError.proto
  vire/cms/InvalidUserError.proto
  vire/cms/MethodArgument.proto
  vire/cms/NoPubsubResourceError.proto
  vire/cms/ResourceExecError.proto
  vire/cms/UnknownResourcesError.proto
  vire/cms/TimeoutError.proto

  vire/cms/ResourceExec.proto
  vire/cms/ResourceExecFailure.proto
  vire/cms/ResourceExecSuccess.proto

  vire/cms/ResourceFetchStatus.proto
  vire/cms/ResourceFetchStatusFailure.proto
  vire/cms/ResourceFetchStatusSuccess.proto

  vire/cms/ResourcePubsub.proto
  vire/cms/ResourcePubsubFailure.proto
  vire/cms/ResourcePubsubSuccess.proto

  vire/cms/ResourceStatusRecord.proto

  vire/cms/ResourceStatusChange.proto
  vire/cms/DeviceStatusChange.proto

  vire/cms/ConnectionRequest.proto
  vire/cms/ConnectionSuccess.proto
  vire/cms/ConnectionFailure.proto
  vire/cms/DisconnectionRequest.proto
  vire/cms/DisconnectionSuccess.proto
  vire/cms/DisconnectionFailure.proto

  )

message( STATUS "[debug] ###############################################################################")
message( STATUS "[info] protobufList = '${protobufList}'")


# https://cmake.org/cmake/help/v3.0/command/add_custom_command.html?highlight=add_#command:add_custom_command
set(_proto_stubs)
set(_pb_out_dir ${VIRE_BUILD_PREFIX}/include/protobuf)
foreach(protobuf_file ${protobufList})
  message( STATUS "[info] Protobuf file='${protobuf_file}' (relative path)")
  get_filename_component(_pb_dir     ${protobuf_file} DIRECTORY)
  get_filename_component(_pb_name_we ${protobuf_file} NAME_WE)
  set(_pb_stub "${_pb_dir}/${_pb_name_we}")
  message( STATUS "[info] ##### _pb_stub='${_pb_stub}'")
  # list(APPEND VireProtobufLibrary_cpp_HEADERS
  #   ${VIRE_BUILD_PREFIX}/include/protobuf/cpp/${proto_stub}.pb.h
  #   )
  # list(APPEND VireProtobufLibrary_cpp_SOURCES
  #   ${VIRE_BUILD_PREFIX}/include/protobuf/cpp/${proto_stub}.pb.cc
  #   )
  # if (VIRE_WITH_PROTOBUF_JAVA)
  #   list(APPEND VireProtobufLibrary_java_SOURCES
  #     ${VIRE_BUILD_PREFIX}/include/protobuf/java/${proto_stub}OuterClass.java
  #     )
  # endif()
  set(_this_protobufList ${protobuf_file})
  message( STATUS "[info] ##### _this_protobufList='${_this_protobufList}'")
  set(_pb_cpp_output
    "${_pb_out_dir}/cpp/${_pb_stub}.pb.h"
    "${_pb_out_dir}/cpp/${_pb_stub}.pb.cc"
    )
  message( STATUS "[info] ##### _pb_cpp_output='${_pb_cpp_output}'")
  if (VIRE_WITH_PROTOBUF_JAVA)
    set(_pb_java_output
      "${_pb_out_dir}/java/${_pb_stub}Stub.java"
      )
    message( STATUS "[info] ##### _pb_java_output='${_pb_java_output}'")
  endif()
  list(APPEND _proto_stubs "${_pb_stub}")

  add_custom_command(
    OUTPUT ${_pb_cpp_output}
    COMMAND ${PROJECT_SOURCE_DIR}/utilities/Protobuf/scripts/gen_pb_source.sh
    --debug
    --protoc ${PROTOBUF_PROTOC_EXECUTABLE}
    --input "${_this_protobufList}"
    --out-dir ${_pb_out_dir}
    --proto-inc-dir .
    --proto-inc-dir ${CMAKE_CURRENT_SOURCE_DIR}
    --proto-src-dir ${CMAKE_CURRENT_SOURCE_DIR}
    --cpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${protobuf_file}
    )
  set(_tmp_pb_cpp_generate_target_name "pb_generate_cpp__${_pb_stub}")
  string(REPLACE "/" "__" pb_cpp_generate_target_name ${_tmp_pb_cpp_generate_target_name})
  message( STATUS "[info] ##### Adding new custom target='${pb_cpp_generate_target_name}'")
  add_custom_target(${pb_cpp_generate_target_name} DEPENDS ${_pb_cpp_output})
  list(APPEND VireProtobufLibrary_cpp_TARGETS ${pb_cpp_generate_target_name})

  if (VIRE_WITH_PROTOBUF_JAVA)
    add_custom_command(
      OUTPUT ${_pb_java_output}
      COMMAND ${PROJECT_SOURCE_DIR}/utilities/Protobuf/scripts/gen_pb_source.sh
      --debug
      --protoc ${PROTOBUF_PROTOC_EXECUTABLE}
      --input "${_this_protobufList}"
      --out-dir ${_pb_out_dir}
      --proto-inc-dir .
      --proto-inc-dir ${CMAKE_CURRENT_SOURCE_DIR}
      --proto-src-dir ${CMAKE_CURRENT_SOURCE_DIR}
      --java
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${protobuf_file}
      )
    set(_tmp_pb_java_generate_target_name "pb_generate_java_${_pb_stub}")
    string(REPLACE "/" "__" pb_java_generate_target_name ${_tmp_pb_java_generate_target_name})
    add_custom_target(${pb_java_generate_target_name} DEPENDS ${_pb_java_output})
    list(APPEND VireProtobufLibrary_java_TARGETS ${pb_java_generate_target_name})
  endif()
endforeach()
message( STATUS "[info] Stubs = '${_proto_stubs}'")

message( STATUS "[debug] ###############################################################################")

# execute_process(COMMAND ${PROJECT_SOURCE_DIR}/utilities/Protobuf/scripts/gen_pb_source.sh
#   --debug
#   --protoc ${PROTOBUF_PROTOC_EXECUTABLE}
#   --input "${protobufList}"
#   --out-dir ${VIRE_BUILD_PREFIX}/include/protobuf
#   --proto-inc-dir .
#   --proto-inc-dir ${CMAKE_CURRENT_SOURCE_DIR}
#   --proto-src-dir ${CMAKE_CURRENT_SOURCE_DIR}
#   --cpp
#   ${_gen_pb_java}
#   --list
#   RESULT_VARIABLE _gen_pb_error_code
#   OUTPUT_VARIABLE _gen_pb_out
#   ERROR_VARIABLE  _gen_pb_err
#   )
# message( STATUS "[info] Done.")
# message( STATUS "[info] _gen_pb_code='${_gen_pb_error_code}'")
# if (NOT _gen_pb_error_code EQUAL 0)
#   message( STATUS "[error] _gen_pb_err='\n${_gen_pb_err}'")
#   message( FATAL_ERROR "[fatal] Failed to build Protobuf stubs!")
# endif()
# message( STATUS "[info] _gen_pb_out='\n${_gen_pb_out}'")
# string(REGEX REPLACE "\n" ";" _proto_stubs "${_gen_pb_out}")

message( STATUS "[info] Stubs = '${_proto_stubs}'")

# if(${_gen_pb_error_code})
#   message( STATUS "[error] _gen_pb_err='\n${_gen_pb_err}'")
#   message( FATAL  "[fatal] Google Protocol Buffer source code generation failed!")
# else()
#   message( STATUS "[info] Google Protocol Buffer source code generation was successful!")
# endif()

foreach(proto_stub ${_proto_stubs})
  message( STATUS "[info] Proto stub='${proto_stub}'")
  list(APPEND VireProtobufLibrary_cpp_HEADERS
    ${_pb_out_dir}/cpp/${proto_stub}.pb.h
    )
  list(APPEND VireProtobufLibrary_cpp_SOURCES
    ${_pb_out_dir}/cpp/${proto_stub}.pb.cc
    )
  if (VIRE_WITH_PROTOBUF_JAVA)
    list(APPEND VireProtobufLibrary_java_SOURCES
      ${_pb_out_dir}/java/${proto_stub}OuterClass.java
      )
  endif()
endforeach()

message( STATUS "[info] C++ headers='${VireProtobufLibrary_cpp_HEADERS}'")
message( STATUS "[info] C++ sources='${VireProtobufLibrary_cpp_SOURCES}'")

# - Install public headers (preliminary)
message (STATUS "[info] CMAKE_CURRENT_BINARY_DIR='${CMAKE_CURRENT_BINARY_DIR}'")
message (STATUS "[info] CMAKE_INSTALL_INCLUDEDIR='${CMAKE_INSTALL_INCLUDEDIR}'")
# install(DIRECTORY ${VIRE_BUILD_PREFIX}/include/protobuf/cpp/vire/
#   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/protobuf/vire
#   FILES_MATCHING PATTERN "*.pb.h"
#   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
#   )

if (VIRE_WITH_PROTOBUF_JAVA)
  include(build_jar.cmake)
endif()

set(VireProtobufLibrary_TESTS
  # protobuf/testing/test-protobuf-protobuf.cxx
  # protobuf/testing/test-protobuf-ModelIdentifier.cxx
)

message (STATUS "[info] End of 'source/vire/protobuf/CMakeLists.txt'.")

# - end
