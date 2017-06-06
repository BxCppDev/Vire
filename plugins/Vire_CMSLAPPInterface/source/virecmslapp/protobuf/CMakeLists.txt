# - Protocol Buffer Support for Vire CMS/LAPP plugin

message( STATUS "***********************************************************")
message( STATUS "* Google Protocol Buffer Support for Vire CMS/LAPP plugin *")
message( STATUS "***********************************************************")

set(_gen_pb_code 0)
set(_gen_pb_java)
set(Vire_CMSLAPPInterface_ProtobufLibrary_cpp_HEADERS)
set(Vire_CMSLAPPInterface_ProtobufLibrary_cpp_SOURCES)
set(VIRE_CMSLAPPINTERFACE_WITH_PROTOBUF_JAVA VIRE_WITH_PROTOBUF_JAVA)
if (VIRE_CMSLAPPINTERFACE_WITH_PROTOBUF_JAVA)
  message( STATUS "Generating C++ & Java source stubs from protobuf files...")
  set(_gen_pb_java "--java")
  set(Vire_CMSLAPPInterface_ProtobufLibrary_java_SOURCES)
else()
  message( STATUS "Generating C++ source stubs from protobuf files...")
endif()

set(protobufList
  vire/cmslapp/ConnectionRequest.proto
  vire/cmslapp/ConnectionSuccess.proto
  vire/cmslapp/ConnectionFailure.proto
  )

message( STATUS "protobufList             = '${protobufList}'")
message( STATUS "PROJECT_SOURCE_DIR       = '${PROJECT_SOURCE_DIR}'")
message( STATUS "CMAKE_CURRENT_SOURCE_DIR = '${CMAKE_CURRENT_SOURCE_DIR}'")
message( STATUS "VIRE_BUILD_PREFIX    = '${VIRE_BUILD_PREFIX}'")

execute_process(COMMAND ${VIRE_SOURCE_DIR}/utilities/Protobuf/scripts/gen_pb_source.sh
  --debug
  --protoc ${PROTOBUF_PROTOC_EXECUTABLE}
  --input "${protobufList}"
  --out-dir ${VIRE_BUILD_PREFIX}/include/protobuf
  --proto-inc-dir ${VIRE_SOURCE_DIR}/source/vire/protobuf
  --proto-inc-dir ${CMAKE_CURRENT_SOURCE_DIR}
  --proto-src-dir ${CMAKE_CURRENT_SOURCE_DIR}
  --cpp
  ${_gen_pb_java}
  --list
  RESULT_VARIABLE _gen_pb_error_code
  OUTPUT_VARIABLE _gen_pb_out
  ERROR_VARIABLE  _gen_pb_err
  )
message( STATUS "Done.")
message( STATUS "_gen_pb_code='${_gen_pb_code}'")
message( STATUS "_gen_pb_err='\n${_gen_pb_err}'")
message( STATUS "_gen_pb_out='\n${_gen_pb_out}'")

string(REGEX REPLACE "\n" ";" _proto_stubs "${_gen_pb_out}")
message( STATUS "Stubs = '${_proto_stubs}'")

if(${_gen_pb_error_code})
  message( FATAL "Google Protocol Buffer source code generation failed!")
else()
  message( STATUS "Google Protocol Buffer source code generation was successful!")
endif()

foreach(proto_stub ${_proto_stubs})
  message( STATUS "Proto stub = '${proto_stub}'")
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

message( STATUS "C++ headers='${Vire_CMSLAPPInterface_ProtobufLibrary_cpp_HEADERS}'")
message( STATUS "C++ sources='${Vire_CMSLAPPInterface_ProtobufLibrary_cpp_SOURCES}'")

# - end