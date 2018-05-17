// i_integer_dialog.cc

// Ourselves:
#include <vire/ui/i_integer_dialog.h>

// Third party:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace ui {

    i_integer_dialog::i_integer_dialog()
    {
      return;
    }

    i_integer_dialog::~i_integer_dialog()
    {
      return;
    }

    bool i_integer_dialog::has_min_value() const
    {
      if (_min_value_) return true;
      return false;
    }

    void i_integer_dialog::set_min_value(const int value_)
    {
      if (has_allowed_values()) {
        DT_THROW(std::logic_error,
                 "Min value is not allowed!");
      }
      if (has_max_value()) {
        DT_THROW_IF(value_ >= get_max_value(),
                    std::logic_error,
                    "Invalid min value!");
      }
      if (has_default_value()) {
        DT_THROW_IF(value_ > get_default_value(),
                    std::logic_error,
                    "Min value conflicts with default value!");
      }
     _min_value_ = value_;
      return;
    }

    int i_integer_dialog::get_min_value() const
    {
      DT_THROW_IF(!has_min_value(),
                  std::logic_error,
                  "No min value!");
      return _min_value_.value();
    }

    bool i_integer_dialog::has_max_value() const
    {
      if (_max_value_) return true;
      return false;
    }

    void i_integer_dialog::set_max_value(const int value_)
    {
      if (has_allowed_values()) {
        DT_THROW(std::logic_error,
                 "Max value is not allowed!");
      }
      if (has_min_value()) {
        DT_THROW_IF(value_ <= get_min_value(),
                    std::logic_error,
                    "Invalid max value!");
      }
      if (has_default_value()) {
        DT_THROW_IF(value_ < get_default_value(),
                    std::logic_error,
                    "Max value conflicts with default value!");
      }
      _max_value_ = value_;
      return;
    }

    int i_integer_dialog::get_max_value() const
    {
      DT_THROW_IF(!has_max_value(),
                  std::logic_error,
                  "No max value!");
      return _max_value_.value();
    }

    bool i_integer_dialog::has_default_value() const
    {
      if (_default_value_) return true;
      return false;
    }

    void i_integer_dialog::set_default_value(const int value_)
    {
      if (has_allowed_values()) {
        DT_THROW_IF(!has_allowed_value(value_),
                    std::logic_error,
                    "Default value is not allowed!");
      }
      if (has_min_value()) {
        DT_THROW_IF(value_ < get_min_value(),
                    std::logic_error,
                    "Default value is out of range!");
      }
      if (has_max_value()) {
        DT_THROW_IF(value_ > get_max_value(),
                    std::logic_error,
                    "Default value is out of range!");
      }
      _default_value_ = value_;
      return;
    }

    int i_integer_dialog::get_default_value() const
    {
      DT_THROW_IF(!has_default_value(),
                  std::logic_error,
                  "No default value!");
      return _default_value_.value();
    }

    bool i_integer_dialog::has_allowed_values() const
    {
      return _allowed_values_.size() > 0;
    }

    const std::set<int> & i_integer_dialog::get_allowed_values() const
    {
      return _allowed_values_;
    }

    void i_integer_dialog::add_allowed_value(const int value_)
    {
      DT_THROW_IF(has_min_value() || has_max_value(),
                  std::logic_error,
                  "Enumerated value is not allowed");
      _allowed_values_.insert(value_);
      return;
    }

    bool i_integer_dialog::has_allowed_value(const int value_) const
    {
      return _allowed_values_.count(value_);
    }

  } // namespace ui

} // namespace vire
