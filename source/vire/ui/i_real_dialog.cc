// i_real_dialog.cc

// Ourselves:
#include <vire/ui/i_real_dialog.h>

// Third party:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/units.h>

namespace vire {

  namespace ui {

    i_real_dialog::i_real_dialog()
    {
      return;
    }

    i_real_dialog::~i_real_dialog()
    {
      return;
    }

    bool i_real_dialog::has_unit_dimension() const
    {
      return ! _unit_dimension_.empty();
    }

    void i_real_dialog::set_unit_dimension(const std::string & dim_)
    {
      if (has_preferred_unit()) {
        DT_THROW_IF(!datatools::units::is_unit_in_dimension_from(get_preferred_unit(), dim_),
                    std::logic_error,
                    "Unit dimension '" << dim_ << "' does not match preferred unit '" << get_preferred_unit() << "'!");
      }
      _unit_dimension_ = dim_;
      return;
    }

    const std::string & i_real_dialog::get_unit_dimension() const
    {
      return _unit_dimension_;
    }

    bool i_real_dialog::has_preferred_unit() const
    {
      return ! _unit_dimension_.empty();
    }

    void i_real_dialog::set_preferred_unit(const std::string & u_)
    {
     if (has_unit_dimension()) {
       DT_THROW_IF(!datatools::units::is_unit_in_dimension_from(u_, get_unit_dimension()),
                    std::logic_error,
                    "Preferred unit '" << u_ << "' does not match unit dimension '" << get_unit_dimension() << "'!");
      }
      _preferred_unit_ = u_;
      return;
    }

    const std::string & i_real_dialog::get_preferred_unit() const
    {
      return _preferred_unit_;
    }

    bool i_real_dialog::has_min_value() const
    {
      if (_min_value_) return true;
      return false;
    }

    void i_real_dialog::set_min_value(const double value_)
    {
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

    double i_real_dialog::get_min_value() const
    {
      DT_THROW_IF(!has_min_value(),
                  std::logic_error,
                  "No min value!");
      return _min_value_.value();
    }

    bool i_real_dialog::has_max_value() const
    {
      if (_max_value_) return true;
      return false;
    }

    void i_real_dialog::set_max_value(const double value_)
    {
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

    double i_real_dialog::get_max_value() const
    {
      DT_THROW_IF(!has_max_value(),
                  std::logic_error,
                  "No max value!");
      return _max_value_.value();
    }

    bool i_real_dialog::has_precision() const
    {
      if (_precision_) return true;
      return false;
    }

    void i_real_dialog::set_precision(const double precision_)
    {
      DT_THROW_IF(precision_ < 0.0,
                  std::logic_error,
                  "Invalid precision!");
      double precision = precision_;
      if (precision <= std::numeric_limits<double>::epsilon()) {
        precision = std::numeric_limits<double>::epsilon();
      }
      _precision_ = precision;
      return;
    }

    double i_real_dialog::get_precision() const
    {
      DT_THROW_IF(!has_precision(),
                  std::logic_error,
                  "No precision!");
      return _precision_.value();
    }
    
    bool i_real_dialog::has_default_value() const
    {
      if (_default_value_) return true;
      return false;
    }

    void i_real_dialog::set_default_value(const double value_)
    {
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

    double i_real_dialog::get_default_value() const
    {
      DT_THROW_IF(!has_default_value(),
                  std::logic_error,
                  "No default value!");
      return _default_value_.value();
    }

  } // namespace ui

} // namespace vire
