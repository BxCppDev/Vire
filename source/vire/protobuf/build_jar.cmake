message (STATUS "[info] Begin of 'source/vire/protobuf/build_jar.cmake'.")

message( STATUS "[info] Build JAR file from Java protobuf stubs...")
message( STATUS "[info]   PROTOBUF_JAVA_JAR='${PROTOBUF_JAVA_JAR}'")
message( STATUS "[info]   PROTOBUF_JAVA_UTIL_JAR='${PROTOBUF_JAVA_UTIL_JAR}'")
#message( STATUS "[info]   PROTOBUF_LITE_JAR='${PROTOBUF_LITE_JAR}'")

find_package(Java REQUIRED)
include(UseJava)
if(Java_FOUND)
  message(STATUS "[info] Java_VERSION_STRING   : '${Java_VERSION_STRING}'")
  message(STATUS "[info] Java_JAVAC_EXECUTABLE : '${Java_JAVAC_EXECUTABLE}'")
  message(STATUS "[info] Java_JAR_EXECUTABLE   : '${Java_JAR_EXECUTABLE}'")
else()
  message( FATAL "[fatal] Could not find Java!")
endif()

message(STATUS "[info] protobuf Jar      : '${PROTOBUF_JAVA_JAR}'")
message(STATUS "[info] protobuf util Jar : '${PROTOBUF_JAVA_UTIL_JAR}'")
#message(STATUS "protobuf lite Jar : '${PROTOBUF_LITE_JAR}'")

# - See: https://cmake.org/cmake/help/v3.5/module/UseJava.html
set(CMAKE_JAVA_INCLUDE_PATH ${PROTOBUF_JAVA_JAR})
add_jar(VireProtobuf
  SOURCES ${VireProtobufLibrary_java_SOURCES}
  OUTPUT_NAME VireProtobuf
  # OUTPUT_DIR ${PROJECT_BINARY_DIR}/lib/java
  )
get_target_property(_jarFile VireProtobuf JAR_FILE)
get_target_property(_classDir VireProtobuf CLASSDIR)
message(STATUS "[info] Jar file          : '${_jarFile}'")
message(STATUS "[info] Class compiled to : '${_classDir}'")

# set(CMAKE_JAVA_COMPILE_FLAGS
#   "-classpath" "XXX"
#   "-classpath" "${PROTOBUF_JAVA_JAR}"
# )

# - Install JAR file (preliminary)
install_jar(VireProtobuf ${CMAKE_INSTALL_LIBDIR}/java)
# install(FILES VireProtobuf.jar
#   DESTINATION ${CMAKE_INSTALL_LIBDIR/java}
#   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
#   )

message (STATUS "[info] End of 'source/vire/protobuf/build_jar.cmake'.")

# - end
