list(APPEND VireLibrary_HEADERS
  auth/utils.h
  auth/crypto.h
  auth/credentials.h
  auth/manager.h
  auth/manager-reflect.h
  auth/password_entry.h
  )

list(APPEND VireLibrary_SOURCES
  auth/utils.cc
  auth/crypto.cc
  auth/credentials.cc
  auth/manager.cc
  auth/password_entry.cc
  )

list(APPEND VireLibrary_TESTS
  auth/testing/test-auth-crypto.cxx
  auth/testing/test-auth-credentials.cxx
  auth/testing/test-auth-manager.cxx
  auth/testing/test-auth-password_entry.cxx
  )
