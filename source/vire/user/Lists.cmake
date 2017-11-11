list(APPEND VireLibrary_HEADERS
  user/user.h
  user/user-reflect.h
  user/user-serial.ipp
  user/enumerated_user_selector.h
  user/group.h
  user/group-reflect.h
  user/group-serial.ipp
  user/manager.h
  user/manager-reflect.h
  user/i_password_generator.h
  user/simple_password_generator.h
  )

list(APPEND VireLibrary_SOURCES
  user/user.cc
  user/enumerated_user_selector.cc
  user/group.cc
  user/manager.cc
  user/i_password_generator.cc
  user/simple_password_generator.cc
  )

list(APPEND VireLibrary_TESTS
  user/testing/test-user-user.cxx
  user/testing/test-user-group.cxx
  user/testing/test-user-manager.cxx
  user/testing/test-user-simple_password_generator.cxx
  )
