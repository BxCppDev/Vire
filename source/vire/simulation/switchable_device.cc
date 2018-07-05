//! \file vire/simulation/switchable_device.cc
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
#include <vire/simulation/switchable_device.h>

namespace vire {

  namespace simulation {

    switchable_device::switchable_device()
    {
      _on_.store(false);
      return;
    }

    // virtual
    switchable_device::~switchable_device()
    {
      return;
    }

    // virtual
    void switchable_device::_at_breakdown_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "Switch off the device at breakdown!");
      _on_.store(false);
      return;
    }

    // virtual
    void switchable_device::_at_repair_()
    {
      _on_.store(false);
      return;
    }
 
    bool switchable_device::is_on() const
    {
      return _on_.load();
    }
 
    bool switchable_device::is_off() const
    {
      return !is_on();
    }

    void switchable_device::switch_it()
    {
      if (is_on()) switch_off();
      else switch_on();
      return;
    }
      
    void switchable_device::switch_on()
    {
      _at_switch_on_();
      _on_.store(true);
      return;
    }
 
    void switchable_device::switch_off()
    {
      _on_.store(false);
      _at_switch_off_();
      return;
    }

    // virtual
    void switchable_device::_at_switch_on_()
    {
      return;
    }

    // virtual
    void switchable_device::_at_switch_off_()
    {
      return;
    }
   
    // virtual
    void switchable_device::print_tree(std::ostream & out_,
                                       const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      this->simple_device::print_tree(out_, base_print_options::force_inheritance(options_));

      out_ <<  popts.indent << inherit_tag(popts.inherit)
           << "On : " << std::boolalpha << is_on() << std::endl;

      return;
    }

  } // namespace simulation

} // namespace vire
