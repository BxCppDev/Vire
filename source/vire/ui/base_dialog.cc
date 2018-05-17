// base_dialog.cc

// Ourselves:
#include <vire/ui/base_dialog.h>

// Third party:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace ui {

    dialog_report::operator bool() const
    {
      return success;
    }

    const std::string & dialog_report::what() const
    {
      return message;
    }

    dialog_report make_dialog_report(const bool success_,
                                     const std::string & message_)
    {
      dialog_report r;
      r.success = success_;
      r.message = message_;
      return r;
    }

    base_dialog::base_dialog()
    {
      return;
    }

    base_dialog::~base_dialog()
    {
      return;
    }

    bool base_dialog::has_title() const
    {
      if (_title_) return true;
      return false;
    }

    void base_dialog::set_title(const std::string & title_)
    {
      _title_ = title_;
      return;
    }

    void base_dialog::reset_title()
    {
      _title_ = boost::none;
      return;
    }

    const std::string & base_dialog::get_title() const
    {
      DT_THROW_IF(!has_title(),
                  std::logic_error,
                  "No title!");
      return _title_.value();
    }

  } // namespace ui

} // namespace vire
