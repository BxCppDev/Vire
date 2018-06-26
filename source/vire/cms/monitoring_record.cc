// vire/cms/monitoring_record.cc
//
// Copyright (c) 2017-208 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cms/monitoring_record.h>

namespace vire {

  namespace cms {
   
    monitoring_record_signal_emitter::monitoring_record_signal_emitter(const monitoring_record & record_)
      : _record_(record_)
    {
      return;
    }
    
    void monitoring_record_signal_emitter::emit_value_change()
    {
      emit sig_value_changed();
      return;
    }
     
    const monitoring_record & monitoring_record_signal_emitter::get_record() const
    {
      return _record_;
    }
  
    monitoring_record_signal_emitter & monitoring_record::grab_emitter()
    {
      if (!_emitter_) {
        _emitter_.reset(new monitoring_record_signal_emitter(*this));
      }
      return *_emitter_.get();
    }

    const monitoring_record_signal_emitter & monitoring_record::get_emitter() const
    {
      monitoring_record * mutable_this = const_cast<monitoring_record*>(this);
      return mutable_this->grab_emitter();
    }

    void monitoring_record::on_value_change()
    {
      grab_emitter().emit_value_change();
      return;
    }

    monitoring_record::monitoring_record(bool allowed_value_)
    {
      if (!allowed_value_) {
        _disallow_value_();
      }
      return;
    }
    
    monitoring_record::~monitoring_record()
    {
      return;
    }

    bool monitoring_record::is_allowed_value() const
    {
      return _allowed_value_;
    }

    void monitoring_record::_disallow_value_()
    {
      _allowed_value_ = false;
      return;
    }

    void monitoring_record::disallow_value()
    {
      _disallow_value_();
      return;
    }

    void monitoring_record::reset()
    {
      _allowed_value_ = true;
      _status_.reset();
      _value_ = boost::none;
      return;
    }

    bool monitoring_record::has_status() const
    {
      return _status_.is_valid();
    }

    void monitoring_record::set_status(const image_status & s_)
    {
      _status_ = s_;
      return;
    }

    const image_status & monitoring_record::get_status() const
    {
      return _status_;
    }

    image_status & monitoring_record::grab_status()
    {
      return _status_;
    }

    bool monitoring_record::has_value() const
    {
      if (_allowed_value_ && _value_) return true;
      return false;
    }

    bool monitoring_record::is_boolean() const
    {
      return has_value() && _value_->type() == typeid(bool);
    }

    bool monitoring_record::is_integer() const
    {
      return has_value() && _value_->type() == typeid(int);
    }

    bool monitoring_record::is_real() const
    {
      return has_value() && _value_->type() == typeid(double);
    }

    bool monitoring_record::is_string() const
    {
      return has_value() && _value_->type() == typeid(std::string);
    }

    void monitoring_record::set_boolean_value(const bool val_)
    {
      bool changed = false;
      if (! _value_ ) {
        changed = true;
      } else {
        if (_value_->type() != typeid(bool)) {
          changed = true;
        } else  if (boost::get<bool>(*_value_) != val_) {
          changed = true;
        }
      }
      if (changed) {
        _grab_opt_value_() = val_;
        on_value_change();
      }
      return;
    }

    void monitoring_record::set_integer_value(const int val_)
    {
      bool changed = false;
      if (! _value_ ) {
        changed = true;
      } else {
        if (_value_->type() != typeid(int)) {
          changed = true;
        } else  if (boost::get<int>(*_value_) != val_) {
          changed = true;
        }
      }
      if (changed) {
        _grab_opt_value_() = val_;
        on_value_change();
      }
      return;
    }

    void monitoring_record::set_real_value(const double val_)
    {
      bool changed = false;
      if (! _value_ ) {
        changed = true;
      } else {
        if (_value_->type() != typeid(double)) {
          changed = true;
        } else  if (boost::get<double>(*_value_) != val_) {
          changed = true;
        }
      }
      if (changed) {
        _grab_opt_value_() = val_;
        on_value_change();
      }
      return;
    }

    void monitoring_record::set_string_value(const std::string & val_)
    {
       bool changed = false;
      if (! _value_ ) {
        changed = true;
      } else {
        if (_value_->type() != typeid(std::string)) {
          changed = true;
        } else  if (boost::get<std::string>(*_value_) != val_) {
          changed = true;
        }
      }
      if (changed) {
        _grab_opt_value_() = val_;
        on_value_change();
      }
      return;
    }

    bool monitoring_record::get_boolean_value() const
    {
      DT_THROW_IF(!is_boolean(), std::logic_error, "No boolean value!");
      return boost::get<bool>(*_value_);
    }

    int monitoring_record::get_integer_value() const
    {
      DT_THROW_IF(!is_integer(), std::logic_error, "No integer value!");
      return boost::get<int>(*_value_);
    }

    double monitoring_record::get_real_value() const
    {
      DT_THROW_IF(!is_real(), std::logic_error, "No real value!");
      return boost::get<double>(*_value_);
    }

    const std::string & monitoring_record::get_string_value() const
    {
      DT_THROW_IF(!is_string(), std::logic_error, "No string value!");
      return boost::get<std::string>(*_value_);
    }

    boost::optional<monitored_value_type> & monitoring_record::_grab_opt_value_()
    {
      DT_THROW_IF(!_allowed_value_, std::logic_error,
                  "Cannot access not allowed value!")
      return _value_;
    }

    const monitored_value_type & monitoring_record::get_value() const
    {
      return *_value_;
    }

    monitored_value_type & monitoring_record::grab_value()
    {
      return *_value_;
    }

    void monitoring_record::reset_value()
    {
      _value_ = boost::none;
      return;
    }

    void monitoring_record::print_tree(std::ostream & out_,
                                       const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      const std::string & indent = popts.indent;
      if (! popts.title.empty ()) {
        out_ << indent << popts.title << std::endl;
      }

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Status : ";
      if (!has_status()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_status()) {
        boost::property_tree::ptree status_options;
        status_options.put(base_print_options::indent_key(),
                           indent + datatools::i_tree_dumpable::tags::skip_item());
        _status_.print_tree(out_, status_options);
      }

      out_ << indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Value  : ";
      if (!_allowed_value_) {
        out_ << "<not allowed>";
      } else if (!has_value()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (_allowed_value_ && has_value()) {

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
