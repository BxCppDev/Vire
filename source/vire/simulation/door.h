//! \file  vire/simulation/door.h
//! \brief Simulated door
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

#ifndef VIRE_SIMULATION_DOOR_H
#define VIRE_SIMULATION_DOOR_H

// Standard library:
#include <memory>

// This project:
#include <vire/simulation/simple_device.h>
#include <vire/simulation/room.h>

namespace vire {

  namespace simulation {

    /// Simulated door
    class door
      : public simple_device
    {
    public:
      
      door();

      virtual ~door();

      bool is_open() const;
 
      bool is_closed() const;

      void open();
 
      void close();

      bool is_locked() const;
 
      bool is_unlocked() const;

      void lock();
 
      void unlock();

      bool has_room_in() const;

      void set_room_in(const room &);
      
      void reset_room_in();

      const room & get_room_in() const;
      
      bool has_room_out() const;

      void set_room_out(const room &);
     
      void reset_room_out();

      const room & get_room_out() const;

      void reset();
      
      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

    private:

      virtual void _at_open_();

      virtual void _at_close_();

      virtual void _at_lock_();

      virtual void _at_unlock_();

      virtual void _at_breakdown_() override;

      virtual void _at_repair_() override;
 
    private:

      const room * _room_in_ = nullptr;
      const room * _room_out_ = nullptr;
      std::atomic<bool> _closed_; ///< Closed status
      std::atomic<bool> _locked_; ///< Locked status
      
    };
    
  } // namespace simulation

} // namespace vire

#endif // VIRE_SIMULATION_DOOR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --

