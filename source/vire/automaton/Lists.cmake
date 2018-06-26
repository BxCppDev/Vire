list(APPEND VireLibrary_HEADERS
  automaton/utils.h
  automaton/base_automaton.h
)

# list(APPEND VireLibrary_PRIVATE_HEADERS
# )

list(APPEND VireLibrary_SOURCES
  automaton/utils.cc
  automaton/base_automaton.cc
)

list(APPEND VireLibrary_TESTS
  automaton/testing/test-automaton-utils.cxx
  automaton/testing/test-automaton-base_automaton.cxx
)