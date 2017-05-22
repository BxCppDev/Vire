#.rst:
# FindProtobuf
# ------------
#
# Locate and configure the Google Protocol Buffers library, executable
# and components.
#
# Defines the following variables:
#
# ``PROTOBUF_FOUND``
#   Found the Google Protocol Buffers library
#   (libprotobuf & header files)
# ``PROTOBUF_INCLUDE_DIRS``
#   Include directories for Google Protocol Buffers
# ``PROTOBUF_LIBRARIES``
#   The protobuf libraries
# ``PROTOBUF_PROTOC_LIBRARIES``
#   The protoc libraries
# ``PROTOBUF_LITE_LIBRARIES``
#   The protobuf-lite libraries
# ``PROTOBUF_JAVA_JAR``
#   The protobuf jar file
# ``PROTOBUF_JAVA_UTIL_JAR``
#   The protobuf util jar file
# ``PROTOBUF_LITE_JAR``
#   The protobuf lite jar file
# ``PROTOBUF_VERSION_STRING``
#
# The following cache variables are also available to set or use:
#
# ``PROTOBUF_LIBRARY``
#   The protobuf library
# ``PROTOBUF_PROTOC_LIBRARY``
#   The protoc library
# ``PROTOBUF_LITE_LIBRARY``
#   The protobuf lite library
# ``PROTOBUF_INCLUDE_DIR``
#   The include directory for protocol buffers
# ``PROTOBUF_PROTOC_EXECUTABLE``
#   The protoc compiler
#
# Accepts the following variables as input:
#
#   PROTOBUF_ROOT - (as a CMake or environment variable)
#        The root directory of the protobuf install prefix
#
# Possible Components:
#
# ``JavaJar``
# ``JavaUtilJar``
#

### message( STATUS "=====> Entering FindProtobuf.cmake...")

if (PROTOBUF_INCLUDE_DIR)
  if (NOT PROTOBUF_ROOT)
    get_filename_component(PROTOBUF_ROOT "${PROTOBUF_INCLUDE_DIR}" PATH)
  endif()
  get_filename_component(PROTOBUF_ROOT_HINT "${PROTOBUF_INCLUDE_DIR}" PATH)
endif()
# mark_as_advanced(PROTOBUF_ROOT)

# Find the include directory
find_path(PROTOBUF_INCLUDE_DIR google/protobuf/message.h
  PATH_SUFFIXES include
  HINTS ${PROTOBUF_ROOT} ${PROTOBUF_ROOT_HINT}
  ENV PROTOBUF_ROOT
  DOC "The Google Protocol Buffers include directory"
  #NO_DEFAULT_PATH
  )
# mark_as_advanced(PROTOBUF_INCLUDE_DIR)
set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIR})

if(PROTOBUF_INCLUDE_DIR STREQUAL "")
  message(STATUS "[warning] FindProtobuf: Could not find google/protobuf/message.h in provided pathes, please set either PROTOBUF_ROOT or PROTOBUF_INCLUDE_DIR")
  set(PROTOBUF_LIBRARIES NOTFOUND)
else()

  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_PROTOBUF_ARCH_DIR 64/)
  endif()

  # Internal function: search for normal library
  function(_protobuf_find_libraries name filename)
    message( STATUS "[info] FindProtobuf: _protobuf_find_libraries: name='${name}' filename='${filename}'")
    find_library(${name}_LIBRARY
      NAMES ${filename}
      PATH_SUFFIXES lib lib${_PROTOBUF_ARCH_DIR}
      HINTS ${PROTOBUF_ROOT} ${PROTOBUF_ROOT_HINT}
      ENV PROTOBUF_ROOT
      NO_DEFAULT_PATH
      )
    # mark_as_advanced(${name}_LIBRARY)
    set(${name}_LIBRARIES     ${${name}_LIBRARY} PARENT_SCOPE)
    message( STATUS "[info] FindProtobuf: _protobuf_find_libraries: name='${name}_LIBRARIES'")
  endfunction()

  # The Protobuf library
  _protobuf_find_libraries(PROTOBUF protobuf)

  # The Protobuf lite library
  _protobuf_find_libraries(PROTOBUF_LITE protobuf-lite)

  # The Protobuf protoc Library
  _protobuf_find_libraries(PROTOBUF_PROTOC protoc)

  # Find the protoc Executable
  find_program(PROTOBUF_PROTOC_EXECUTABLE
    NAMES protoc
    PATH_SUFFIXES bin
    HINTS ${PROTOBUF_ROOT} ${PROTOBUF_ROOT_HINT}
    ENV PROTOBUF_ROOT
    DOC "The Google Protocol Buffers Compiler"
    NO_DEFAULT_PATH
    )
  # mark_as_advanced(PROTOBUF_PROTOC_EXECUTABLE)

  if(PROTOBUF_FOUND)
    set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIR})
  endif()
  execute_process(
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --version
    COMMAND sed -e "s/libprotoc //g"
    COMMAND tr -d "[[:space:]]"
    OUTPUT_VARIABLE _protobuf_version_string
    )
  # message( STATUS "_protobuf_version_string='${_protobuf_version_string}'")
  set(PROTOBUF_VERSION_STRING ${_protobuf_version_string})
  message( STATUS "[info] FindProtobuf: PROTOBUF_VERSION_STRING='${PROTOBUF_VERSION_STRING}'")
  # mark_as_advanced(PROTOBUF_VERSION_STRING)
endif()


message( STATUS "[info] FindProtobuf: Protobuf components='${Protobuf_FIND_COMPONENTS}'")

if( Protobuf_FIND_COMPONENTS )
  set(PROTOBUF_HAVE_JAVA_JAR 0)
  set(PROTOBUF_HAVE_JAVA_UTIL_JAR 0)
  set(PROTOBUF_HAVE_LITE_JAR 0)
  foreach( component ${Protobuf_FIND_COMPONENTS} )
    message( STATUS "[info] FindProtobuf: Protobuf component='${component}'")
    if (component STREQUAL "JavaJar")
      # Find the Java JAR file
      find_file(PROTOBUF_JAVA_JAR
	protobuf-java-${PROTOBUF_VERSION_STRING}.jar
	PATHS
	${PROTOBUF_ROOT}/lib/java
	${PROTOBUF_ROOT}/lib${_PROTOBUF_ARCH_DIR}/java
	DOC "The Google Protocol Buffers JAR file"
	NO_DEFAULT_PATH
	)
      if (PROTOBUF_JAVA_JAR)
	set(PROTOBUF_HAVE_JAVA_JAR 1)
      endif()
      mark_as_advanced(PROTOBUF_JAVA_JAR)
    elseif (component STREQUAL "JavaUtilJar")
      # Find the Java util JAR file
      find_file(PROTOBUF_JAVA_UTIL_JAR
	protobuf-java-util-${PROTOBUF_VERSION_STRING}.jar
	PATHS
	${PROTOBUF_ROOT}/lib/java
	${PROTOBUF_ROOT}/lib${_PROTOBUF_ARCH_DIR}/java
	DOC "The Google Protocol Buffers JAR util file"
	NO_DEFAULT_PATH
	)
      if (PROTOBUF_JAVA_UTIL_JAR)
	set(PROTOBUF_HAVE_JAVA_UTIL_JAR 1)
      endif()
      mark_as_advanced(PROTOBUF_JAVA_UTIL_JAR)
    # elseif (component STREQUAL "JavaLiteJar")
    #   # Find the Java lite JAR file
    #   find_file(PROTOBUF_JAVA_LITE_JAR
    # 	protobuf-lite-3.0.0-beta-3.jar
    # 	PATHS
    # 	${PROTOBUF_ROOT}/lib/java
    # 	${PROTOBUF_ROOT}/lib${_PROTOBUF_ARCH_DIR}/java
    # 	DOC "The Google Protocol Buffers JAR lite file"
    # 	NO_DEFAULT_PATH
    # 	)
    #   if (PROTOBUF_JAVA_LITE_JAR)
    # 	set(PROTOBUF_HAVE_JAVA_LITE_JAR 1)
    #   endif()
    #   mark_as_advanced(PROTOBUF_JAVA_LITE_JAR)
    else ()
      message(FATAL_ERROR "[fatal] FindProtobuf: Unknown Protobuf Component: ${component}")
    endif()

  endforeach( component )
endif( Protobuf_FIND_COMPONENTS )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Protobuf
  REQUIRED_VARS
  PROTOBUF_LIBRARY
  PROTOBUF_PROTOC_LIBRARY
  PROTOBUF_LITE_LIBRARY
  PROTOBUF_PROTOC_EXECUTABLE
  PROTOBUF_INCLUDE_DIR
  VERSION_VAR PROTOBUF_VERSION_STRING)

mark_as_advanced(
  PROTOBUF_INCLUDE_DIR
  PROTOBUF_LIBRARY
  PROTOBUF_PROTOC_LIBRARY
  PROTOBUF_LITE_LIBRARY
  PROTOBUF_PROTOC_EXECUTABLE
  PROTOBUF_ROOT
)

# - end
