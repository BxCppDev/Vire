// i_password_dialog.cc

// Ourselves:
#include <vire/ui/i_password_dialog.h>

// Third party:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace ui {

    i_password_dialog::i_password_dialog()
    {
      _show_stars_ = false;
      _allow_empty_password_ = false;
      return;
    }

    i_password_dialog::~i_password_dialog()
    {
      return;
    }

    bool i_password_dialog::is_show_stars() const
    {
      return _show_stars_;
    }

    void i_password_dialog::set_show_stars(bool ss_)
    {
      _show_stars_ = ss_;
      return;
    }

    bool i_password_dialog::is_allow_empty_password() const
    {
      return _allow_empty_password_;
    }

    void i_password_dialog::set_allow_empty_password(bool aep_)
    {
      _allow_empty_password_ = aep_;
      return;
    }

  } // namespace ui

} // namespace vire
