// i_string_dialog.cc

// Ourselves:
#include <vire/ui/i_string_dialog.h>

// Third party:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace ui {

    i_string_dialog::i_string_dialog()
    {
      return;
    }

    i_string_dialog::~i_string_dialog()
    {
      return;
    }

    bool i_string_dialog::has_default_value() const
    {
      if (_default_value_) return true;
      return false;
    }

    void i_string_dialog::set_default_value(const std::string & value_)
    {
      if (has_allowed_values()) {
        DT_THROW_IF(!has_allowed_value(value_),
                    std::logic_error,
                    "Default value is not allowed");
      }
      _default_value_ = value_;
      return;
    }

    const std::string & i_string_dialog::get_default_value() const
    {
      DT_THROW_IF(!has_default_value(),
                  std::logic_error,
                  "No default value!");
      return _default_value_.value();
    }

    bool i_string_dialog::has_allowed_values() const
    {
      return _allowed_values_.size() > 0;
    }

    const std::set<std::string> & i_string_dialog::get_allowed_values() const
    {
      return _allowed_values_;
    }

    void i_string_dialog::add_allowed_value(const std::string & value_)
    {
      _allowed_values_.insert(value_);
      return;
    }

    bool i_string_dialog::has_allowed_value(const std::string & value_) const
    {
      return _allowed_values_.count(value_);
    }

  } // namespace ui

} // namespace vire
