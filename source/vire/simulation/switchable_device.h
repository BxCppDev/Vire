//! \file  vire/simulation/switchable_device.h
//! \brief Simulated switchable device
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

#ifndef VIRE_SIMULATION_SWITCHABLE_DEVICE_H
#define VIRE_SIMULATION_SWITCHABLE_DEVICE_H

// This project:
#include <vire/simulation/simple_device.h>

namespace vire {

  namespace simulation {

    /// Simulated switchable device
    class switchable_device
      : public simple_device
    {
    public:
      
      switchable_device();

      virtual ~switchable_device();

      bool is_on() const;
 
      bool is_off() const;

      void switch_it();
    
      void switch_on();
 
      void switch_off();

      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

    private:

      virtual void _at_switch_on_();

      virtual void _at_switch_off_();

      virtual void _at_breakdown_() override;

      virtual void _at_repair_() override;
 
    private:

      std::atomic<bool> _on_;
      
    };
    
  } // namespace simulation

} // namespace vire

#endif // VIRE_SIMULATION_SWITCHABLE_DEVICE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --

