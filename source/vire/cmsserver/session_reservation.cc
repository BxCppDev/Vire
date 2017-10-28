//! \file vire/cmsserver/session_reservation.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/session_reservation.h>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace cmsserver {

    // Serialization implementation
    DATATOOLS_SERIALIZATION_IMPLEMENTATION(session_reservation, "vire::cmsserver::session_reservation")

    session_reservation::session_reservation()
    {
      return;
    }

    session_reservation::~session_reservation()
    {
      return;
    }

    bool session_reservation::is_valid() const
    {
      if (!has_sinfo()) return false;
      if (!has_pinfo()) return false;
      return true;
    }

    void session_reservation::reset()
    {
      _sinfo_.reset();
      _pinfo_.reset();
      _start_macro_.clear();
      _stop_macro_.clear();
      return;
    }

    bool session_reservation::has_sinfo() const
    {
      return _sinfo_.is_valid();
    }

    void session_reservation::set_sinfo(const session_info & si_)
    {
      _sinfo_ = si_;
      return;
    }

    const session_info & session_reservation::get_sinfo() const
    {
      return _sinfo_;
    }

    bool session_reservation::has_pinfo() const
    {
      return _pinfo_.is_valid();
    }

    void session_reservation::set_pinfo(const process_info & pi_)
    {
      _pinfo_ = pi_;
      return;
    }

    const process_info & session_reservation::get_pinfo() const
    {
      return _pinfo_;
    }

    void session_reservation::set_start_macro(const std::string & sm_)
    {
      _start_macro_ = sm_;
      return;
    }

    bool session_reservation::has_start_macro() const
    {
      return !_start_macro_.empty();
    }

    const std::string & session_reservation::get_start_macro() const
    {
      return _start_macro_;
    }

    void session_reservation::set_stop_macro(const std::string & sm_)
    {
      _start_macro_ = sm_;
      return;
    }

    bool session_reservation::has_stop_macro() const
    {
      return !_stop_macro_.empty();
    }

    const std::string & session_reservation::get_stop_macro() const
    {
      return _stop_macro_;
    }

    void session_reservation::tree_dump(std::ostream & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }


      out_ << indent_ << i_tree_dumpable::tag
           << "Session info    : ";
      out_ << std::endl;
      _sinfo_.tree_dump(out_, "", indent_ + "|   ");

      out_ << indent_ << i_tree_dumpable::tag
           << "Process info    : ";
      out_ << std::endl;
      _pinfo_.tree_dump(out_, "", indent_ + "|   ");

      out_ << indent_ << i_tree_dumpable::tag
           << "Start macro    : ";
      if (has_start_macro()) {
        out_ << "'" << _start_macro_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Stop macro     : ";
      if (has_stop_macro()) {
        out_ << "'" << _stop_macro_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Valid   : " << std::boolalpha << is_valid() << std::endl;

      return;
    }

    bool operator<(const session_reservation & r1_,
                   const session_reservation & r2_)
    {
      vire::utility::comparison_result comp = session_reservation::compare(r1_, r2_);
      DT_THROW_IF(comp == vire::utility::COMPARISON_NOT_APPLICABLE,
                  std::logic_error,
                  "Comparison is not applicable!");
      return comp == vire::utility::COMPARISON_LESS;
    }

    vire::utility::comparison_result session_reservation::compare(const session_reservation & r1_,
                                                                  const session_reservation & r2_)
    {
      if (!r1_.is_valid()) return vire::utility::COMPARISON_NOT_APPLICABLE;
      if (!r2_.is_valid()) return vire::utility::COMPARISON_NOT_APPLICABLE;
      const session_info & si1 = r1_.get_sinfo();
      const session_info & si2 = r2_.get_sinfo();
      const boost::posix_time::time_period & when1 = si1.get_when();
      const boost::posix_time::time_period & when2 = si2.get_when();
      const boost::posix_time::ptime & b1 = when1.begin();
      const boost::posix_time::ptime & b2 = when2.begin();
      const boost::posix_time::ptime & l1 = when1.last();
      const boost::posix_time::ptime & l2 = when2.last();
      // Compare first bounds:
      vire::utility::comparison_result comp = vire::time::compare(b1, b2);
      if (comp == vire::utility::COMPARISON_LESS) {
        return vire::utility::COMPARISON_LESS;
      } else if (comp == vire::utility::COMPARISON_EQUAL) {
        // Now compare last bounds:
        vire::utility::comparison_result comp2 = vire::time::compare(l2, l1);
        if (comp2 == vire::utility::COMPARISON_LESS) {
          return vire::utility::COMPARISON_LESS;
        } else if (comp2 == vire::utility::COMPARISON_EQUAL) {
          return vire::utility::COMPARISON_EQUAL;
        } else if (comp2 == vire::utility::COMPARISON_GREATER) {
          return vire::utility::COMPARISON_GREATER;
        } else {
          return vire::utility::COMPARISON_NOT_APPLICABLE;
        }
      } else if (comp == vire::utility::COMPARISON_GREATER) {
        return vire::utility::COMPARISON_GREATER;
      }
      return vire::utility::COMPARISON_NOT_APPLICABLE;
    }

  } // namespace cmsserver

} // namespace vire
