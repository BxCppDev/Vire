list(APPEND VireLibrary_HEADERS
  ui/i_password_dialog.h
  ui/tui_utils.h
  ui/tui_password_dialog.h
  ui/gui_password_dialog.h
  )

list(APPEND VireLibrary_SOURCES
  ui/tui_utils.cc
  ui/tui_password_dialog.cc
  ui/gui_password_dialog.cc
  )

list(APPEND VireLibrary_TESTS
  # ui/testing/test-ui-tui_password_dialog.cc
  # ui/testing/test-ui-gui_password_dialog.cc
  )
