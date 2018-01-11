// vire/cms/monitoring.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of Vire.
//
// Vire is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Vire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/cms/monitoring.h>

namespace vire {

  namespace cms {

    std::string value_monitoring_mode_label(const value_monitoring_mode_type mode_)
    {
      if (mode_ == VALUE_MONITORING_POLLING) return std::string("polling");
      if (mode_ == VALUE_MONITORING_ONCHANGE) return std::string("onchange");
      return std::string("");
    }

    value_monitoring_mode_type value_monitoring_mode(const std::string & label_)
    {
      if (label_ == "polling") return VALUE_MONITORING_POLLING;
      if (label_ == "onchange") return VALUE_MONITORING_ONCHANGE;
      return VALUE_MONITORING_INVALID;
    }

    /* ============ VALUE MONITORING RECORD ============ */

    void monitored_value_record::reset()
    {
      _status_.reset();
      _value_ = boost::none;
      return;
    }

    bool monitored_value_record::has_status() const
    {
      return _status_.is_valid();
    }

    void monitored_value_record::set_status(const image_status & s_)
    {
      _status_ = s_;
      return;
    }

    const image_status & monitored_value_record::get_status() const
    {
      return _status_;
    }

    image_status & monitored_value_record::grab_status()
    {
      return _status_;
    }

    bool monitored_value_record::has_value() const
    {
      if (_value_) return true;
      return false;
    }

    bool monitored_value_record::is_boolean() const
    {
      return _value_->type() == typeid(bool);
    }

    bool monitored_value_record::is_integer() const
    {
      return _value_->type() == typeid(int);
    }

    bool monitored_value_record::is_real() const
    {
      return _value_->type() == typeid(double);
    }

    bool monitored_value_record::is_string() const
    {
      return _value_->type() == typeid(std::string);
    }

    void monitored_value_record::set_boolean_value(const bool val_)
    {
      _value_ = val_;
      return;
    }

    void monitored_value_record::set_integer_value(const int val_)
    {
      _value_ = val_;
      return;
    }

    void monitored_value_record::set_real_value(const double val_)
    {
      _value_ = val_;
      return;
    }

    void monitored_value_record::set_string_value(const std::string & val_)
    {
      _value_ = val_;
      return;
    }

    bool monitored_value_record::get_boolean_value() const
    {
      return boost::get<bool>(*_value_);
    }

    int monitored_value_record::get_integer_value() const
    {
      return boost::get<int>(*_value_);
    }

    double monitored_value_record::get_real_value() const
    {
      return boost::get<double>(*_value_);
    }

    const std::string & monitored_value_record::get_string_value() const
    {
      return boost::get<std::string>(*_value_);
    }

    const monitored_value_type & monitored_value_record::get_value() const
    {
      return *_value_;
    }

    monitored_value_type & monitored_value_record::grab_value()
    {
      return *_value_;
    }

    void monitored_value_record::reset_value()
    {
      _value_ = boost::none;
      return;
    }

    void monitored_value_record::print_tree(std::ostream & out_,
                                            const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      const std::string & indent = popts.indent;
      if (! popts.title.empty ()) {
        out_ << indent << popts.title << std::endl;
      }

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Status : " << std::boolalpha << has_status() << std::endl;
      if (has_status()) {
        boost::property_tree::ptree status_options;
        status_options.put(base_print_options::indent_key(),
                           indent + datatools::i_tree_dumpable::tags::skip_item());
        _status_.print_tree(out_, status_options);
      }

      out_ << indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Value  : " << std::boolalpha << has_value() << std::endl;
      if (has_value()) {

        out_ << indent << datatools::i_tree_dumpable::inherit_skip_tag(popts.inherit)
             << datatools::i_tree_dumpable::tag << "Type: ";
        int idx = _value_->which();
        switch(idx){
        case 0: out_ << "boolean"; break;
        case 1: out_ << "integer"; break;
        case 2: out_ << "real"; break;
        case 3: out_ << "string"; break;
        }
        out_ << std::endl;

        out_ << indent << datatools::i_tree_dumpable::inherit_skip_tag(popts.inherit)
             << datatools::i_tree_dumpable::last_tag << "Raw value: ";
        if (idx == 0) out_ << std::boolalpha;
        if (idx == 3) out_ << '"';
        out_ << *_value_;
        if (idx == 3) out_ << '"';
        out_ << std::endl;
      }
      return;
    }

  } // namespace cms

} // namespace vire
