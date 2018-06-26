// vire/cms/image_status.cc - Device/resource CMS image status
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
#include <vire/cms/image_status.h>

// This project:
#include <vire/time/utils.h>
#include <vire/cms/utils.h>

namespace vire {

  namespace cms {

    image_status_signal_emitter::image_status_signal_emitter(const image_status & status_)
      : _status_(status_)
    {
      return;
    }
    
    void image_status_signal_emitter::emit_change()
    {
      emit status_changed();
      return;
    }
    
    const image_status & image_status_signal_emitter::get_status() const
    {
      return _status_;
    }
 
    image_status_signal_emitter & image_status::grab_emitter()
    {
      if (!_emitter_) {
        _emitter_.reset(new image_status_signal_emitter(*this));
      }
      return *_emitter_.get();
    }

    const image_status_signal_emitter & image_status::get_emitter() const
    {
      image_status * mutable_this = const_cast<image_status*>(this);
      return mutable_this->grab_emitter();
    }
 
    void image_status::on_change()
    {
      grab_emitter().emit_change();
      return;
    }

    image_status::image_status()
    {
      return;
    }

    image_status::image_status(const image_status & other_)
    {
      _timestamp_ = other_._timestamp_;
      _missing_ = other_._missing_;
      _disabled_ = other_._disabled_;
      _pending_ = other_._pending_;
      _failed_ = other_._failed_;
      return;
    }
    
    image_status & image_status::operator=(const image_status & other_)
    {
      if (this != &other_) {
        _timestamp_ = other_._timestamp_;
        _missing_ = other_._missing_;
        _disabled_ = other_._disabled_;
        _pending_ = other_._pending_;
        _failed_ = other_._failed_;
        on_change();
      }
      return *this;
    }

    image_status::~image_status()
    {
      return;
    }

    bool image_status::is_valid() const
    {
      if (!::vire::time::is_valid(_timestamp_)) return false;
      return true;
    }

    void image_status::reset()
    {
      indeterminate_flags();
      reset_timestamp();
      return;
    }

    void image_status::reset_timestamp()
    {
      vire::time::invalidate_time(_timestamp_);
      on_change();
      return;
    }

    bool image_status::has_timestamp() const
    {
      return vire::time::is_valid(_timestamp_);
    }

    void image_status::set_timestamp(const boost::posix_time::ptime & t_)
    {
      _timestamp_ = t_;
      on_change();
      return;
    }

    const boost::posix_time::ptime & image_status::get_timestamp() const
    {
      return _timestamp_;
    }

    bool image_status::has_missing() const
    {
      return ! boost::logic::indeterminate(_missing_);
    }

    bool image_status::is_present() const
    {
      if (!_missing_) {
        return true;
      }
      return false;
    }

    bool image_status::is_missing() const
    {
      if (_missing_) {
        return true;
      }
      return false;
    }

    void image_status::set_missing(bool m_)
    {
      if (has_missing()) {
        if (_missing_ && m_) return;
        if (!_missing_ && !m_) return;
      }
      _missing_ = m_;
      on_change();
      return;
    }

    void image_status::reset_missing()
    {
      if (has_missing()) {
        _missing_ = boost::logic::tribool::indeterminate_value;
        on_change();
      }
      return;
    }

    bool image_status::has_disabled() const
    {
      return ! boost::logic::indeterminate(_disabled_);
    }

    bool image_status::is_enabled() const
    {
      if (!_disabled_) {
        return true;
      }
      return false;
    }

    bool image_status::is_disabled() const
    {
      if (_disabled_) {
        return true;
      }
      return false;
    }

    void image_status::set_disabled(bool d_)
    {
      if (has_disabled()) {
        if (_disabled_ && d_) return;
        if (!_disabled_ && !d_) return;
      }
      _disabled_ = d_;
      on_change();
      return;
    }

    void image_status::reset_disabled()
    {
      if (has_disabled()) {
        _disabled_ = boost::logic::tribool::indeterminate_value;
        on_change();
      }
      return;
    }

    bool image_status::is_idle() const
    {
      if (!_pending_) {
        return true;
      }
      return false;
    }

    bool image_status::has_pending() const
    {
      return ! boost::logic::indeterminate(_pending_);
    }

    bool image_status::is_pending() const
    {
      if (_pending_) {
        return true;
      }
      return false;
    }

    void image_status::set_pending(bool p_)
    {
      if (has_pending()) {
        if (_pending_ && p_) return;
        if (!_pending_ && !p_) return;
      }
      _pending_ = p_;
      on_change();
      return;
    }

    void image_status::reset_pending()
    {
      if (has_pending()) {
        _pending_ = boost::logic::tribool::indeterminate_value;
        on_change();
      }
      return;
    }

    bool image_status::is_success() const
    {
      if (!_failed_) {
        return true;
      }
      return false;
    }

    bool image_status::has_failed() const
    {
      return ! boost::logic::indeterminate(_failed_);
    }

    bool image_status::is_failed() const
    {
      if (_failed_) {
        return true;
      }
      return false;
    }

    void image_status::set_failed(bool f_)
    {
      if (has_failed()) {
        if (_failed_ && f_) return;
        if (!_failed_ && !f_) return;
      }
      _failed_ = f_;
      on_change();
      return;
    }

    void image_status::reset_failed()
    {
      if (has_failed()) {
        _failed_ = boost::logic::tribool::indeterminate_value;
        on_change();
      }
      return;
    }

    void image_status::indeterminate_flags()
    {
      _pending_ = boost::logic::indeterminate;
      _disabled_ = boost::logic::indeterminate;
      _failed_ = boost::logic::indeterminate;
      _missing_ = boost::logic::indeterminate;
      on_change();
      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const image_status & status_)
    {
      if (status_.has_missing()) {
        out_ << '?';
      } else {
        if (status_.is_missing()) out_ << 'm';
        else out_ << '-';
      }
      if (status_.has_disabled()) {
        out_ << '?';
      } else {
        if (status_.is_disabled()) out_ << 'd';
        else out_ << '-';
      }
      if (status_.has_pending()) {
        out_ << '?';
      } else {
        if (status_.is_pending()) out_ << 'p';
        else out_ << '-';
      }
      if (status_.has_failed()) {
        out_ << '?';
      } else {
        if (status_.is_failed()) out_ << 'f';
        else out_ << '-';
      }
      out_ << ' ';
      if (status_.has_timestamp()) {
        out_ << vire::time::to_string(status_._timestamp_);
      } else {
        out_ << "?";
      }
      return out_;
    }

    void image_status::print_tree(std::ostream & out_,
                                  const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      const std::string & indent = popts.indent;
      if (! popts.title.empty ()) {
        out_ << indent << popts.title << std::endl;
      }

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Timestamp : " << '[' << boost::posix_time::to_iso_string(_timestamp_) << ']'
           << std::endl;

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Missing   : " << repr(_missing_)
           << std::endl;

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Disabled  : " << repr(_disabled_)
           << std::endl;

      out_ << indent << datatools::i_tree_dumpable::tag
           << "Pending   : " << repr(_pending_)
           << std::endl;

      out_ << indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Failed    : " << repr(_failed_)
           << std::endl;

      return;
    }

  } // namespace cms

} // namespace vire
