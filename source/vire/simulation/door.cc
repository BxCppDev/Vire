//! \file vire/simulation/door.cc
//
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/simulation/door.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace simulation {

    door::door()
    {
      _closed_.store(true);
      _locked_.store(false);
      return;
    }

    // virtual
    door::~door()
    {
      return;
    }

    // virtual
    void door::_at_breakdown_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "Switch off the device at breakdown!");
      return;
    }

    // virtual
    void door::_at_repair_()
    {
      _closed_.store(true);
      _locked_.store(false);
      return;
    }
 
    bool door::is_open() const
    {
      return !is_closed();
    }
 
    bool door::is_closed() const
    {
      return _closed_.load();
    }

    void door::open()
    {
      _at_open_();
      _closed_.store(false);
      return;
    }
 
    void door::close()
    {
      _closed_.store(true);
      _at_close_();
      return;
    }

    // virtual
    void door::_at_close_()
    {
      return;
    }

    // virtual
    void door::_at_open_()
    {
      return;
    }
 
    bool door::is_locked() const
    {
      return _locked_.load();
    }
 
    bool door::is_unlocked() const
    {
      return !is_locked();
    }

    void door::lock()
    {
      DT_THROW_IF(!is_closed(), std::logic_error,
                  "Cannot lock an open door!");
      _at_lock_();
      _locked_.store(true);
      return;
    }
 
    void door::unlock()
    {
      DT_THROW_IF(!is_locked(), std::logic_error,
                  "Cannot unlock an unlocked door!");
      _locked_.store(false);
      _at_unlock_();
      return;
    }

    // virtual
    void door::_at_lock_()
    {
      return;
    }

    // virtual
    void door::_at_unlock_()
    {
      return;
    }
 
    bool door::has_room_in() const
    {
      return _room_in_ != nullptr;
    }

    void door::set_room_in(const room & rin_)
    {
      _room_in_ = &rin_;
      return;
    }
      
    void door::reset_room_in()
    {
      _room_in_ = nullptr;
      return;
    }

    const room & door::get_room_in() const
    {
      DT_THROW_IF(!has_room_in(), std::logic_error, "No room in!");
      return *_room_in_;
    }
  
    bool door::has_room_out() const
    {
      return _room_out_ != nullptr;
    }

    void door::set_room_out(const room & rin_)
    {
      _room_out_ = &rin_;
      return;
    }
      
    void door::reset_room_out()
    {
      _room_out_ = nullptr;
      return;
    }

    const room & door::get_room_out() const
    {
      DT_THROW_IF(!has_room_out(), std::logic_error, "No room out!");
      return *_room_out_;
    }

    void door::reset()
    {
      reset_room_in();
      reset_room_out();
      return;
    }
 
    // virtual
    void door::print_tree(std::ostream & out_,
                          const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      this->simple_device::print_tree(out_, base_print_options::force_inheritance(options_));

      out_ <<  popts.indent << tag
           << "Room in : ";
      if (has_room_in()) {
        out_ << "'" << get_room_in().get_name() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Room out : ";
      if (has_room_out()) {
        out_ << "'" << get_room_out().get_name() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      
      out_ <<  popts.indent << tag
           << "Closed : " << std::boolalpha << is_closed() << std::endl;

      out_ <<  popts.indent << inherit_tag(popts.inherit)
           << "Locked : " << std::boolalpha << is_locked() << std::endl;

      return;
    }

  } // namespace simulation

} // namespace vire
