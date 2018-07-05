//! \file  vire/simulation/simple_device.h
//! \brief Simulated base simple device
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

#ifndef VIRE_SIMULATION_SIMPLE_DEVICE_H
#define VIRE_SIMULATION_SIMPLE_DEVICE_H

// Standard library:
#include <atomic>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>

// This project:
#include <vire/simulation/base_object.h>

namespace vire {

  namespace simulation {

    /// Simulated base simple device
    class simple_device
      : public base_object
    {
    public:
      
      simple_device();

      virtual ~simple_device();

      bool is_out_of_order() const;
      
      void breakdown();

      void repair();

      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

    private:

      virtual void _at_breakdown_();

      virtual void _at_repair_();
      
    private:

      std::atomic<bool> _out_of_order_;
      
    };
    
  } // namespace simulation

} // namespace vire

#endif // VIRE_SIMULATION_SIMPLE_DEVICE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
