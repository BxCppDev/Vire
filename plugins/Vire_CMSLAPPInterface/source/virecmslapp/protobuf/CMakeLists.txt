# - Protocol Buffer Support for Vire CMS/LAPP plugin

message( STATUS "[info] ***********************************************************")
message( STATUS "[info] * Google Protocol Buffer Support for Vire CMS/LAPP plugin *")
message( STATUS "[info] ***********************************************************")

set(_gen_pb_code 0)
set(_gen_pb_java)
set(Vire_CMSLAPPInterface_ProtobufLibrary_cpp_HEADERS)
set(Vire_CMSLAPPInterface_ProtobufLibrary_cpp_SOURCES)
set(Vire_CMSLAPPInterface_ProtobufLibrary_cpp_TARGETS)
set(Vire_CMSLAPPInterface_ProtobufLibrary_java_TARGETS)
set(VIRE_CMSLAPPINTERFACE_WITH_PROTOBUF_JAVA VIRE_WITH_PROTOBUF_JAVA)
if (VIRE_CMSLAPPINTERFACE_WITH_PROTOBUF_JAVA)
  message( STATUS "[info] Generating C++ & Java source stubs from protobuf files...")
  set(_gen_pb_java "--java")
  set(Vire_CMSLAPPInterface_ProtobufLibrary_java_SOURCES)
else()
  message( STATUS "[info] Generating C++ source stubs from protobuf files...")
endif()

set(protobufList
  vire/cmslapp/ErrorCode.proto
  )

message( STATUS "[info] protobufList             = '${protobufList}'")
message( STATUS "[info] PROJECT_SOURCE_DIR       = '${PROJECT_SOURCE_DIR}'")
message( STATUS "[info] CMAKE_CURRENT_SOURCE_DIR = '${CMAKE_CURRENT_SOURCE_DIR}'")
message( STATUS "[info] [info] VIRE_BUILD_PREFIX        = '${VIRE_BUILD_PREFIX}'")
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
    COMMAND ${VIRE_SOURCE_DIR}/utilities/Protobuf/scripts/gen_pb_source.sh
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
  list(APPEND _VireProtobufLibrary_cpp_TARGETS ${pb_cpp_generate_target_name})

  if (VIRE_WITH_PROTOBUF_JAVA)
    add_custom_command(
      OUTPUT ${_pb_java_output}
      COMMAND ${VIRE_SOURCE_DIR}/utilities/Protobuf/scripts/gen_pb_source.sh
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
    list(APPEND _VireProtobufLibrary_java_TARGETS ${pb_java_generate_target_name})
  endif()
endforeach()
message( STATUS "[info] Stubs = '${_proto_stubs}'")

# execute_process(COMMAND ${VIRE_SOURCE_DIR}/utilities/Protobuf/scripts/gen_pb_source.sh
#   --debug
#   --protoc ${PROTOBUF_PROTOC_EXECUTABLE}
#   --input "${protobufList}"
#   --out-dir ${VIRE_BUILD_PREFIX}/include/protobuf
#   --proto-inc-dir ${VIRE_SOURCE_DIR}/source/vire/protobuf
#   --proto-inc-dir ${CMAKE_CURRENT_SOURCE_DIR}
#   --proto-src-dir ${CMAKE_CURRENT_SOURCE_DIR}
#   --cpp
#   ${_gen_pb_java}
#   --list
#   RESULT_VARIABLE _gen_pb_error_code
#   OUTPUT_VARIABLE _gen_pb_out
#   ERROR_VARIABLE  _gen_pb_err
#   )
# message( STATUS "Done.")
# message( STATUS "_gen_pb_code='${_gen_pb_code}'")
# message( STATUS "_gen_pb_err='\n${_gen_pb_err}'")
# message( STATUS "_gen_pb_out='\n${_gen_pb_out}'")

# string(REGEX REPLACE "\n" ";" _proto_stubs "${_gen_pb_out}")
# message( STATUS "Stubs = '${_proto_stubs}'")

# if(${_gen_pb_error_code})
#   message( FATAL "Google Protocol Buffer source code generation failed!")
# else()
#   message( STATUS "Google Protocol Buffer source code generation was successful!")
# endif()

foreach(proto_stub ${_proto_stubs})
  message( STATUS "[info] Proto stub = '${proto_stub}'")
  list(APPEND Vire_CMSLAPPInterface_ProtobufLibrary_cpp_HEADERS
    ${VIRE_BUILD_PREFIX}/include/protobuf/cpp/${proto_stub}.pb.h
    )
  list(APPEND Vire_CMSLAPPInterface_ProtobufLibrary_cpp_SOURCES
    ${VIRE_BUILD_PREFIX}/include/protobuf/cpp/${proto_stub}.pb.cc
    )
  if (VIRE_WITH_PROTOBUF_JAVA)
    list(APPEND Vire_CMSLAPPInterface_ProtobufLibrary_java_SOURCES
      ${VIRE_BUILD_PREFIX}/include/protobuf/java/${proto_stub}OuterClass.java
      )
  endif()
endforeach()

message( STATUS "[info] C++ headers='${Vire_CMSLAPPInterface_ProtobufLibrary_cpp_HEADERS}'")
message( STATUS "[info] C++ sources='${Vire_CMSLAPPInterface_ProtobufLibrary_cpp_SOURCES}'")

message (STATUS "[info] End of Google Protocol Buffer Support for Vire CMS/LAPP plugin.")

# - end