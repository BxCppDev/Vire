list(APPEND VireLibrary_HEADERS
  ui/tui_utils.h
  ui/base_dialog.h
  ui/base_input_dialog.h
  ui/i_password_dialog.h
  ui/i_string_dialog.h
  ui/i_integer_dialog.h
  ui/i_file_dialog.h
  ui/i_real_dialog.h
  ui/tui_password_dialog.h
  ui/gui_password_dialog.h
  ui/tui_string_dialog.h
  ui/gui_string_dialog.h
  ui/tui_integer_dialog.h
  ui/gui_integer_dialog.h
  ui/tui_file_dialog.h
  ui/gui_file_dialog.h
  ui/tui_real_dialog.h
  ui/gui_real_dialog.h
  #ui/qrealinputdialog.h
  ui/ui_service.h
  )

list(APPEND VireLibrary_SOURCES
  ui/tui_utils.cc
  ui/base_dialog.cc
  ui/base_input_dialog.cc
  ui/i_password_dialog.cc
  ui/i_string_dialog.cc
  ui/i_integer_dialog.cc
  ui/i_file_dialog.cc
  ui/i_real_dialog.cc
  ui/tui_password_dialog.cc
  ui/gui_password_dialog.cc
  ui/tui_string_dialog.cc
  ui/gui_string_dialog.cc
  ui/tui_integer_dialog.cc
  ui/gui_integer_dialog.cc
  ui/tui_file_dialog.cc
  ui/gui_file_dialog.cc
  ui/tui_real_dialog.cc
  ui/gui_real_dialog.cc
  #ui/qrealinputdialog.cc
  ui/ui_service.cc
  )

list(APPEND VireLibrary_TESTS
  ui/testing/test-ui-tui_password_dialog.cc
  #ui/testing/test-ui-gui_password_dialog.cc
  ui/testing/test-ui-tui_string_dialog.cc
  #ui/testing/test-ui-gui_string_dialog.cc
  ui/testing/test-ui-tui_integer_dialog.cc
  #ui/testing/test-ui-gui_integer_dialog.cxx
  ui/testing/test-ui-tui_file_dialog.cxx
  #ui/testing/test-ui-gui_file_dialog.cxx
  ui/testing/test-ui-tui_real_dialog.cc
  #ui/testing/test-ui-gui_real_dialog.cc
  )
