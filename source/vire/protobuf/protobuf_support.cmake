# - Protocol Buffer Support for Vire

message( STATUS "**********************************")
message( STATUS "* Google Protocol Buffer Support *")
message( STATUS "**********************************")

set(_gen_pb_code 0)
set(_gen_pb_java)
set(VireProtobufLibrary_cpp_HEADERS)
set(VireProtobufLibrary_cpp_SOURCES)
if (VIRE_WITH_PROTOBUF_JAVA)
  message( STATUS "Generating C++ & Java source stub from protobuf files...")
  set(_gen_pb_java "--java")
  set(VireProtobufLibrary_java_SOURCES)
else()
  message( STATUS "Generating C++ source stub from protobuf files...")
endif()
execute_process(COMMAND ${PROJECT_SOURCE_DIR}/utilities/Protobuf/scripts/gen_pb_source.sh
  --protoc ${PROTOBUF_PROTOC_EXECUTABLE}
  #--out-dir ${CMAKE_CURRENT_BINARY_DIR}
  --out-dir ${VIRE_BUILDPRODUCT_DIR}/include/protobuf
  --proto-src-dir ${CMAKE_CURRENT_SOURCE_DIR}
  --cpp
  ${_gen_pb_java}
  --list
  RESULT_VARIABLE _gen_pb_error_code
  OUTPUT_VARIABLE _gen_pb_out
  ERROR_VARIABLE  _gen_pb_err
  )
message( STATUS "Done.")
# message( STATUS "_gen_pb_code='${_gen_pb_code}'")
# message( STATUS "_gen_pb_out='${_gen_pb_out}'")
# message( STATUS "_gen_pb_err='${_gen_pb_err}'")

string(REGEX REPLACE "\n" ";" _proto_stubs "${_gen_pb_out}")
message( STATUS "Stubs = '${_proto_stubs}'")
# set(_proto_stubs ${_gen_pb_out})

if(${_gen_pb_error_code})
  message( FATAL "Google Protocol Buffer source code generation failed!")
else()
  message( STATUS "Google Protocol Buffer source code generation was successful!")
endif()

foreach(proto_stub ${_proto_stubs})
  message( STATUS "Proto stub = '${proto_stub}'")
  list(APPEND VireProtobufLibrary_cpp_HEADERS
    ${VIRE_BUILDPRODUCT_DIR}/include/protobuf/cpp/${proto_stub}.pb.h
    )
  list(APPEND VireProtobufLibrary_cpp_SOURCES
    ${VIRE_BUILDPRODUCT_DIR}/include/protobuf/cpp/${proto_stub}.pb.cc
    )
  if (VIRE_WITH_PROTOBUF_JAVA)
    list(APPEND VireProtobufLibrary_java_SOURCES
      ${VIRE_BUILDPRODUCT_DIR}/include/protobuf/java/${proto_stub}OuterClass.java
      )
  endif()
endforeach()

# message( STATUS "C++ headers='${VireProtobufLibrary_cpp_HEADERS}'")
# message( STATUS "C++ sources='${VireProtobufLibrary_cpp_SOURCES}'")
# if (VIRE_WITH_PROTOBUF_JAVA)
#   message( STATUS "Java sources='${VireProtobufLibrary_java_SOURCES}'")
# endif()
# message (STATUS "PROTOBUF_INCLUDE_DIRS = '${PROTOBUF_INCLUDE_DIRS}' ")

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${VIRE_BUILDPRODUCT_DIR}/include/protobuf/cpp)

add_library(Vire_protobuf SHARED
  ${VireProtobufLibrary_cpp_HEADERS}
  ${VireProtobufLibrary_cpp_SOURCES}
)
target_compile_features(Vire_protobuf PUBLIC ${VIRE_CXX_COMPILE_FEATURES})
target_link_libraries(Vire_protobuf
  "${PROTOBUF_PROTOC_LIBRARIES};${PROTOBUF_LIBRARIES};pthread")

# - Rpath it
set_target_properties(Vire_protobuf PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)

if(UNIX AND NOT APPLE)
  set_target_properties(Vire_protobuf
    PROPERTIES INSTALL_RPATH "\$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
    )
elseif(APPLE)
  # Temporary setting - needs testing
  set_target_properties(Vire_protobuf
    PROPERTIES
      INSTALL_NAME_DIR "@rpath"
      #LINK_FLAGS "-Wl,-rpath,@loader_path"
    )
endif()

set_target_properties(Vire_protobuf PROPERTIES INTERFACE_LINK_LIBRARIES "")

list(APPEND Vire_ADDON_TARGETS Vire_protobuf)

# - Install it
# TODO : fixup rpaths as needed
install(TARGETS Vire_protobuf
  EXPORT  VireTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

# - Install public headers (preliminary)
message (STATUS "===> CMAKE_CURRENT_BINARY_DIR='${CMAKE_CURRENT_BINARY_DIR}'")
message (STATUS "===> CMAKE_INSTALL_INCLUDEDIR='${CMAKE_INSTALL_INCLUDEDIR}'")
#install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cpp/vire
install(DIRECTORY ${VIRE_BUILDPRODUCT_DIR}/include/protobuf/cpp/vire/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/protobuf/vire
  FILES_MATCHING PATTERN "*.pb.h"
  # FILES_MATCHING REGEX "*.pb.h"
  # REGEX "*.java" EXCLUDE
  # REGEX "*.pb.cc" EXCLUDE
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  )

if (VIRE_WITH_PROTOBUF_JAVA)
  include(build_jar.cmake)
endif()

set(VireProtobufLibrary_TESTS
  protobuf/testing/test-protobuf-protobuf.cxx
  # protobuf/testing/test-protobuf-ModelIdentifier.cxx
)
#message (STATUS "000 ---> VireProtobufLibrary_TESTS='${VireProtobufLibrary_TESTS}'")

# - end
