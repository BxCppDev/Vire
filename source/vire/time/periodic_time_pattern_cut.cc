// vire/time/periodic_time_pattern_cut.cc - Implementation of Vire periodic time pattern cut
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Ourselves
#include <vire/time/periodic_time_pattern_cut.h>

namespace vire {

  namespace time {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(periodic_time_pattern_cut, "vire::time::periodic_time_pattern_cut");

    periodic_time_pattern_cut::periodic_time_pattern_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _period_ = boost::posix_time::time_duration(boost::posix_time::not_a_date_time);
      return;
    }

    periodic_time_pattern_cut::~periodic_time_pattern_cut()
    {
      if (is_initialized()) {
        this->periodic_time_pattern_cut::reset();
      }
      return;
    }

    bool periodic_time_pattern_cut::has_valid_time_pattern() const
    {
      return _time_pattern_.has_data();
    }

    const cuts::i_cut & periodic_time_pattern_cut::get_time_pattern() const
    {
      DT_THROW_IF(!has_valid_time_pattern(), std::logic_error,
                  "Cut '" << get_name() << "' has nopattern!");
      return _time_pattern_->get();
    }

    void periodic_time_pattern_cut::set_time_pattern(cut_handle_type & time_pattern_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized !");
      _time_pattern_ = time_pattern_;
      return;
    }

    bool periodic_time_pattern_cut::has_valid_period() const
    {
      return !_period_.is_special();
    }

    const boost::posix_time::time_duration & periodic_time_pattern_cut::get_period() const
    {
      return _period_;
    }

    void periodic_time_pattern_cut::set_period(const boost::posix_time::time_duration & period_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized !");
      _period_ = period_;
      return;
    }

    void periodic_time_pattern_cut::initialize(const datatools::properties & config_,
                                               datatools::service_manager & /* service_manager_ */,
                                               cuts::cut_handle_dict_type & cut_dict_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized !");

      _common_initialize(config_);

      if (! has_valid_time_pattern()) {
        std::string cut_name;
        DT_THROW_IF(!a_configuration.has_key("time_pattern"),
                    std::logic_error,
                    "Missing 'time_pattern' name property for periodic time pattern cut !");
        cut_name = a_configuration.fetch_string("time_pattern");
        cut_handle_dict_type::iterator found = a_cut_dict.find(cut_name);
        DT_THROW_IF(found == a_cut_dict.end(),
                    std::logic_error,
                    "Can't find any cut named '"
                    << cut_name
                    << "' from the external dictionnary for this NOT cut ! ");
        this->set_time_pattern(found->second.grab_initialized_cut_handle());
      }

      if (! has_valid_period()) {
        if (config_.has_key("period")) {
          const std::string & period_str = config_.fetch_string("period");
          boost::posix_time::time_duration duration
            = boost::posix_time::duration_from_string(duration_str);
          stop_time = start_time + duration;
        }
      }

      DT_THROW_IF(! has_valid_time_pattern(), std::logic_error,
                  "Cut '" << get_name() << "' : Missing valid time pattern!");
      DT_THROW_IF(! has_valid_period(), std::logic_error,
                  "Cut '" << get_name() << "' : Missing valid period!");

      _set_initialized(true);
      return;
    }

    void periodic_time_pattern_cut::reset()
    {
      _set_initialized(false);
      _period_ = boost::posix_time::time_duration(boost::posix_time::not_a_date_time);
      _time_pattern_.reset();
      this->cuts::i_cut::_reset();
      return;
    }

    void periodic_time_pattern_cut::set_time_pattern(cuts::cut_handle_type & a_cut_handle)
    {
      DT_THROW_IF(&a_cut_handle.grab () == this,
                  std::logic_error,
                  "Self-referenced periodic time pattern cut '"
                  << get_name () << "' is not allowed !");
      _time_pattern_ = a_cut_handle;
      return;
    }

    void periodic_time_pattern_cut::_at_set_user_data()
    {
      _export_user_data_to(_time_pattern_.grab());
      return;
    }

    void periodic_time_pattern_cut::_at_reset_user_data()
    {
      _time_pattern_.grab().reset_user_data();
      return;
    }

      virtual int _accept();

    void periodic_time_pattern_cut::tree_dump(std::ostream & out_,
                                      const std::string & title_,
                                      const std::string & indent_,
                                      bool inherit_) const
    {
      this->cuts::i_cut::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Time pattern : " // << _period_
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Period : " // << _period_
           << std::endl;

      return;
    }

  } // namespace time

} // namespace vire
