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

    // static
    const int32_t session_reservation::INVALID_ID;

    session_reservation::session_reservation()
      : _id_(INVALID_ID)
      , _last_update_(vire::time::invalid_time())
      , _validation_time_(vire::time::invalid_time())
      , _cancellation_time_(vire::time::invalid_time())
      , _when_(vire::time::invalid_time_interval())
    {
      return;
    }

    session_reservation::~session_reservation()
    {
      return;
    }

    bool session_reservation::is_valid() const
    {
      if (!has_id()) return false;
      if (!has_resource_scope()) return false;
      if (!has_when()) return false;
      if (!has_use_case_info()) return false;
      return true;
    }

    void session_reservation::reset()
    {
      _id_ = INVALID_ID;
      _booked_by_.clear();
      vire::time::invalidate_time(_last_update_);
      _validated_ = false;
      _validated_by_.clear();
      vire::time::invalidate_time(_validation_time_);
      _cancelled_ = false;
      _cancelled_by_.clear();
      vire::time::invalidate_time(_cancellation_time_);
      _resource_scope_.clear();
      vire::time::invalidate_time_interval(_when_);
      _use_case_info_.reset();
      _start_macro_.clear();
      _stop_macro_.clear();
      return;
    }

    bool session_reservation::has_id() const
    {
      return _id_ != INVALID_ID;
    }

    void session_reservation::set_id(const int32_t id_)
    {
      _id_ = id_ >= 0 ? id_ : INVALID_ID;
      return;
    }

    int32_t session_reservation::get_id() const
    {
      return _id_;
    }

    bool session_reservation::has_sequence_id() const
    {
      return _sequence_id_ != INVALID_ID;
    }

    void session_reservation::set_sequence_id(const int32_t sequence_id_)
    {
      _sequence_id_ = sequence_id_ >= 0 ? sequence_id_ : INVALID_ID;
      return;
    }

    int32_t session_reservation::get_sequence_id() const
    {
      return _sequence_id_;
    }

    bool session_reservation::has_booked_by() const
    {
      return ! _booked_by_.empty();
    }

    const std::string & session_reservation::get_booked_by() const
    {
      return _booked_by_;
    }

    void session_reservation::set_booked_by(const std::string & bb_)
    {
      _booked_by_ = bb_;
      return;
    }

    bool session_reservation::has_last_update() const
    {
      return vire::time::is_valid(_last_update_);
    }

    const boost::posix_time::ptime & session_reservation::get_last_update() const
    {
      return _last_update_;
    }

    void session_reservation::set_last_update(const boost::posix_time::ptime & lu_)
    {
      _last_update_ = lu_;
      return;
    }

    bool session_reservation::is_validated() const
    {
      return _validated_;
    }

    void session_reservation::set_validated(bool v_)
    {
      _validated_ = v_;
      return;
    }

    bool session_reservation::has_validated_by() const
    {
      return ! _validated_by_.empty();
    }

    const std::string & session_reservation::get_validated_by() const
    {
      return _validated_by_;
    }

    void session_reservation::set_validated_by(const std::string & vb_)
    {
      _validated_by_ = vb_;
      return;
    }

    bool session_reservation::has_validation_time() const
    {
      return vire::time::is_valid(_validation_time_);
     }

    void session_reservation::set_validation_time(const boost::posix_time::ptime & t_)
    {
      _validation_time_ = t_;
      return;
    }

    const boost::posix_time::ptime & session_reservation::get_validation_time() const
    {
      return _validation_time_;
    }


    bool session_reservation::is_cancelled() const
    {
      return _cancelled_;
    }

    void session_reservation::set_cancelled(bool v_)
    {
      _cancelled_ = v_;
      return;
    }

    bool session_reservation::has_cancelled_by() const
    {
      return ! _cancelled_by_.empty();
    }

    const std::string & session_reservation::get_cancelled_by() const
    {
      return _cancelled_by_;
    }

    void session_reservation::set_cancelled_by(const std::string & vb_)
    {
      _cancelled_by_ = vb_;
      return;
    }

    bool session_reservation::has_cancellation_time() const
    {
      return vire::time::is_valid(_cancellation_time_);
     }

    void session_reservation::set_cancellation_time(const boost::posix_time::ptime & t_)
    {
      _cancellation_time_ = t_;
      return;
    }

    const boost::posix_time::ptime & session_reservation::get_cancellation_time() const
    {
      return _cancellation_time_;
    }

    bool session_reservation::has_resource_scope() const
    {
      return !_resource_scope_.empty();
    }

    void session_reservation::set_resource_scope(const std::string & resource_scope_)
    {
      _resource_scope_ = resource_scope_;
      return;
    }

    const std::string & session_reservation::get_resource_scope() const
    {
      return _resource_scope_;
    }

    bool session_reservation::has_when() const
    {
      return vire::time::is_valid(_when_);
    }

    void session_reservation::set_when(const boost::posix_time::time_period & when_)
    {
      _when_ = when_;
      return;
    }

    const boost::posix_time::time_period & session_reservation::get_when() const
    {
      return _when_;
    }

    bool session_reservation::has_use_case_info() const
    {
      return _use_case_info_.is_valid();
    }

    void session_reservation::set_use_case_info(const use_case_info & uci_)
    {
      _use_case_info_ = uci_;
      return;
    }

    const use_case_info & session_reservation::get_use_case_info() const
    {
      return _use_case_info_;
    }

    bool session_reservation::has_start_macro() const
    {
      return !_start_macro_.empty();
    }

    const std::string & session_reservation::get_start_macro() const
    {
      return _start_macro_;
    }

    void session_reservation::set_start_macro(const std::string & sm_)
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

    void session_reservation::set_stop_macro(const std::string & sm_)
    {
      _stop_macro_ = sm_;
      return;
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
           << "ID          : " << get_id() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Sequence ID : " << get_sequence_id() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Booked by   : " << get_booked_by() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Last update : " << vire::time::to_string(get_last_update()) << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Validated   : " << std::boolalpha << is_validated() << std::endl;

      if (is_validated()) {
        out_ << indent_ << i_tree_dumpable::tag
             << "Validated by   : " << get_validated_by() << std::endl;

        out_ << indent_ << i_tree_dumpable::tag
             << "Validation time : " << vire::time::to_string(_validation_time_) << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Cancelled   : " << std::boolalpha << is_cancelled() << std::endl;

      if (is_cancelled()) {
        out_ << indent_ << i_tree_dumpable::tag
             << "Cancelled by   : " << get_cancelled_by() << std::endl;

        out_ << indent_ << i_tree_dumpable::tag
             << "Cancellation time : " << vire::time::to_string(_cancellation_time_) << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Resource scope : ";
      if (has_resource_scope()) {
        out_ << "'" << _resource_scope_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "When        : ";
      if (has_when()) {
        out_ << boost::posix_time::to_simple_string(_when_);
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Use case info    : ";
      out_ << std::endl;
      _use_case_info_.tree_dump(out_, "", indent_ + "|   ");

      out_ << indent_ << i_tree_dumpable::tag
           << "Start macro : ";
      if (has_start_macro()) {
        out_ << "'" << _start_macro_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Stop macro  : ";
      if (has_stop_macro()) {
        out_ << "'" << _stop_macro_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << std::boolalpha << is_valid() << std::endl;

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
      const boost::posix_time::time_period & when1 = r1_.get_when();
      const boost::posix_time::time_period & when2 = r2_.get_when();
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
