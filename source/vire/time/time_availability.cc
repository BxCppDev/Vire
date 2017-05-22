// vire/time/time_availability.cc - Implementation of Vire time availability
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/time/time_availability.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/exception.h>
// - Bayeux/cuts:
#include <cuts/i_cut.h>


namespace vire {

  namespace time {

    void time_availability::_set_defaults()
    {
      _time_resolution_ = boost::posix_time::seconds(1);
      return;
    }

    time_availability::time_availability()
    {
      _time_cut_ = 0;
      _set_defaults();
      return;
    }

    time_availability::time_availability(const cuts::i_cut & time_cut_)
    {
      _time_cut_ = 0;
      _set_defaults();
      set_time_cut(time_cut_);
      return;
    }

    time_availability::~time_availability()
    {
      if (is_valid()) {
        reset();
      }
      return;
    }

    bool time_availability::is_valid() const
    {
      return _time_cut_ != 0 && ! _time_resolution_.is_special() && ! _time_resolution_.is_negative();
    }

    std::string time_availability::get_name() const
    {
      if (is_valid()) return _time_cut_->get_name();
      return "";
    }

    void time_availability::set_time_resolution(const boost::posix_time::time_duration & time_resolution_)
    {
      DT_THROW_IF(time_resolution_.is_special() || time_resolution_.is_negative(),
                  std::logic_error,
                  "Invalid time resolution for time availability with name '" << get_name() << "'!");
      _time_resolution_ = time_resolution_;
      return;
    }

    const boost::posix_time::time_duration & time_availability::get_time_resolution() const
    {
      return _time_resolution_;
    }

    void time_availability::set_time_cut(const cuts::i_cut & time_cut_)
    {
      _time_cut_ = &time_cut_;
      return;
    }

    const cuts::i_cut & time_availability::get_time_cut() const
    {
      return *_time_cut_;
    }

    bool time_availability::accept_time(const boost::posix_time::ptime & time_) const
    {
      return accept_period(boost::posix_time::time_period(time_, time_ + _time_resolution_));
    }

    bool time_availability::accept_period(const boost::posix_time::time_period & period_) const
    {
      cuts::i_cut * active_cut = const_cast<cuts::i_cut *>(_time_cut_);
      active_cut->set_user_data<boost::posix_time::time_period>(period_);
      return active_cut->process() == cuts::SELECTION_ACCEPTED;
    }

    void time_availability::reset()
    {
      _set_defaults();
      _time_cut_ = 0;
      return;
    }

    void time_availability::tree_dump(std::ostream & out_,
                                      const std::string & title_,
                                      const std::string & indent_,
                                      bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Time resolution : " << _time_resolution_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Time cut : ";
      if (_time_cut_) {
        out_ << "'" << _time_cut_->get_name() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << is_valid() << std::endl;

      return;
    }

    void time_availability::draw(std::ostream & out_,
                                 const boost::posix_time::time_period & time_slice_,
                                 const boost::posix_time::time_duration & delta_time_,
                                 int shift_, int mode_) const
    {
      boost::posix_time::ptime start(time_slice_.begin());
      boost::posix_time::ptime end(time_slice_.end());
      boost::posix_time::time_iterator i(start, delta_time_);
      boost::posix_time::ptime time_t_epoch(boost::gregorian::date(1970,1,1));
      int time_shift_ref = (start - time_t_epoch).total_seconds();
      int mode = 0; // Seconds since the start of the time slice
      if (mode_ == 1) {
        mode = 1; // Text representation
      }
      while(i < end) {
        bool available = accept_time(*i);
        if (mode == 1) {
          out_ << boost::posix_time::to_simple_string(*i);
        } else if (mode == 0) {
          out_ << (*i - time_t_epoch).total_seconds() - time_shift_ref;
        }
        out_ << " " << (available ? 1 + shift_ : shift_);
        out_ << std::endl;
        ++i;
      }
      out_ << std::endl;
      out_ << std::endl;
      return;
    }

  } // namespace time

} // namespace vire
