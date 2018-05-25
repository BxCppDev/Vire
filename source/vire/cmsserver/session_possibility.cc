//! \file vire/cmsserver/session_possibility.cc
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
#include <vire/cmsserver/session_possibility.h>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace cmsserver {

    // static
    std::string session_possibility::action_to_label(const action_type a_)
    {
      switch(a_) {
      case action_enter_session : return std::string("enter");
      case action_create_session : return std::string("create");
      default: break;
      }
      return std::string("");
    }

    session_possibility::session_possibility()
    {
      _action_ = action_undefined;
      _when_ = vire::time::invalid_time_interval();
      return;
    }

    session_possibility::~session_possibility()
    {
      return;
    }

    bool session_possibility::is_valid() const
    {
      if (_action_ == action_undefined) return false;
      if (_role_id_.empty()) return false;
      if (_action_ == action_enter_session) {
        if (_session_key_.empty()) return false;
        if (!vire::time::is_valid(_when_)) return false;
      }
      // if (_action_ == action_create_session) {
      //   if (_usecase_id_.empty()) return false;
      //   if (!vire::time::is_valid(_when_)) return false;
      // }
      return true;
    }

    void session_possibility::reset()
    {
      _action_ = action_undefined;
      _role_id_.clear();
      _when_ = vire::time::invalid_time_interval();
      _session_key_.clear();
      _process_name_.clear();
      _process_description_.clear();
      _process_type_id_.clear();
      return;
    }

    void session_possibility::make_enter_session(const std::string & role_id_,
                                                 const std::string & session_key_,
                                                 const boost::posix_time::ptime & end_)
    {
      _action_ = action_enter_session;
      _role_id_ = role_id_;
      boost::posix_time::time_period when(vire::time::now_utc(), end_);
      DT_THROW_IF(!vire::time::is_valid(when), std::logic_error,
                  "Invalid end : " << boost::posix_time::to_iso_string(end_));
      _when_ = when;
      _session_key_ = session_key_;
      _process_name_.clear();
      _process_description_.clear();
      _process_type_id_.clear();
      return;
    }

    // void session_possibility::make_create_session(const std::string & usecase_id_,
    //                                               const std::string & role_id_,
    //                                               const boost::posix_time::time_period & when_)
    // {
    //   _action_ = action_create_session;
    //   _session_id_.clear();
    //   _usecase_id_ = usecase_id_;
    //   _role_id_ = role_id_;
    //   DT_THROW_IF(!vire::time::is_valid(when_), std::logic_error, "Invalid time period!");
    //   _when_ = when_;
    //   return;
    // }

    bool session_possibility::is_enter_session() const
    {
      return (_action_ == action_enter_session);
    }

    bool session_possibility::is_create_session() const
    {
      return (_action_ == action_create_session);
    }

    void session_possibility::set_role_id(const std::string & id_)
    {
      _role_id_ = id_;
      return;
    }

    const std::string & session_possibility::get_role_id() const
    {
      return _role_id_;
    }

    void session_possibility::set_when(const boost::posix_time::time_period & when_)
    {
      _when_ = when_;
      return;
    }

    const boost::posix_time::time_period &
    session_possibility::get_when() const
    {
      return _when_;
    }

    void session_possibility::set_session_key(const std::string & key_)
    {
      _session_key_ = key_;
      return;
    }

    const std::string & session_possibility::get_session_key() const
    {
      return _session_key_;
    }

    void session_possibility::set_process_name(const std::string & name_)
    {
      _process_name_ = name_;
      return;
    }

    const std::string & session_possibility::get_process_name() const
    {
      return _process_name_;
    }

    void session_possibility::tree_dump(std::ostream & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Action:  '" << action_to_label(_action_) << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Role ID: '" << _role_id_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "When: " << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::skip_tag
           << datatools::i_tree_dumpable::tag
           << "Begin: [" << boost::posix_time::to_iso_string(_when_.begin()) << ']' << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::skip_tag
           << datatools::i_tree_dumpable::last_tag
           << "End:   [" << boost::posix_time::to_iso_string(_when_.end()) << ']' << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Session key: '" << _session_key_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Process name: '" << _process_name_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Process description: '" << _process_description_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Valid: " << std::boolalpha << is_valid() << std::endl;

      return;
    }

  } // namespace cmsserver

} // namespace vire
