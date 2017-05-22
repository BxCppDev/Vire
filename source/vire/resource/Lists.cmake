list(APPEND VireLibrary_HEADERS
  resource/utils.h
  resource/resource.h
  resource/base_resource_instance.h
  resource/dummy_resource_instance.h
  resource/datapoint_resource_instance.h
  resource/method_resource_instance.h
  resource/resource-reflect.h
  resource/role.h
  resource/base_resource_builder.h
  resource/base_role_builder.h
  resource/devices_to_resources_builder.h
  resource/base_resource_selector.h
  resource/enumerated_resource_selector.h
  resource/ranged_resource_selector.h
  resource/regex_resource_selector.h
  resource/by_access_resource_selector.h
  resource/by_tag_resource_selector.h
  resource/general_expression_resource_selector.h
  resource/manager.h
)

list(APPEND VireLibrary_PRIVATE_HEADERS
  resource/detail/gers_tools.h
)

list(APPEND VireLibrary_SOURCES
  resource/utils.cc
  resource/resource.cc
  resource/base_resource_instance.cc
  resource/dummy_resource_instance.cc
  resource/datapoint_resource_instance.cc
  resource/method_resource_instance.cc
  resource/role.cc
  resource/base_resource_builder.cc
  resource/base_role_builder.cc
  resource/devices_to_resources_builder.cc
  resource/base_resource_selector.cc
  resource/enumerated_resource_selector.cc
  resource/ranged_resource_selector.cc
  resource/regex_resource_selector.cc
  resource/by_access_resource_selector.cc
  resource/by_tag_resource_selector.cc
  resource/general_expression_resource_selector.cc
  resource/detail/gers_tools.cc
  resource/manager.cc
)

list(APPEND VireLibrary_TESTS
  resource/testing/test-resource-resource.cxx
  resource/testing/test-resource-role.cxx
  resource/testing/test-resource-enumerated_resource_selector.cxx
  resource/testing/test-resource-regex_resource_selector.cxx
  resource/testing/test-resource-general_expression_resource_selector.cxx
  resource/testing/test-resource-by_access_resource_selector.cxx
  resource/testing/test-resource-by_tag_resource_selector.cxx
  resource/testing/test-resource-manager.cxx
)
