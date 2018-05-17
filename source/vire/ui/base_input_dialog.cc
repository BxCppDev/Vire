// base_input_dialog.cc

// Ourselves:
#include <vire/ui/base_input_dialog.h>

// Third party:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace ui {

    base_input_dialog::base_input_dialog()
    {
      return;
    }

    base_input_dialog::~base_input_dialog()
    {
      return;
    }

    bool base_input_dialog::has_label() const
    {
      if (_label_) return true;
      return false;
    }

    void base_input_dialog::set_label(const std::string & label_)
    {
      _label_ = label_;
      return;
    }

    void base_input_dialog::reset_label()
    {
      _label_ = boost::none;
      return;
    }

    const std::string & base_input_dialog::get_label() const
    {
      DT_THROW_IF(!has_label(),
                  std::logic_error,
                  "No label!");
      return _label_.value();
    }

  } // namespace ui

} // namespace vire
