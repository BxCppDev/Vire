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

    image_status::image_status()
    {
      return;
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
      return;
    }

    bool image_status::has_timestamp() const
    {
      return vire::time::is_valid(_timestamp_);
    }

    void image_status::set_timestamp(const boost::posix_time::ptime & t_)
    {
      _timestamp_ = t_;
      return;
    }

    const boost::posix_time::ptime & image_status::get_timestamp() const
    {
      return _timestamp_;
    }

    bool image_status::has_missing() const
    {
      return _missing_;
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
      _missing_ = m_;
      return;
    }

    void image_status::reset_missing()
    {
      _missing_ = boost::logic::indeterminate;
      return;
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
      _disabled_ = d_;
      return;
    }

    void image_status::reset_disabled()
    {
      _disabled_ = boost::logic::indeterminate;
      return;
    }

    bool image_status::is_idle() const
    {
      if (!_pending_) {
        return true;
      }
      return false;
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
      _pending_ = p_;
      return;
    }

    void image_status::reset_pending()
    {
      _pending_ = boost::logic::indeterminate;
      return;
    }

    bool image_status::is_success() const
    {
      if (!_failed_) {
        return true;
      }
      return false;
    }

    bool image_status::is_failed() const
    {
      if (_failed_) {
        return true;
      }
      return false;
    }

    void image_status::set_failed(bool e_)
    {
      _failed_ = e_;
      return;
    }

    void image_status::reset_failed()
    {
      _failed_ = boost::logic::indeterminate;
      return;
    }

    void image_status::indeterminate_flags()
    {
      reset_missing();
      reset_disabled();
      reset_pending();
      reset_failed();
      return;
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
