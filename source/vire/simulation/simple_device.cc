//! \file vire/simulation/simple_device.cc
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
#include <vire/simulation/simple_device.h>

// Standard Library:
// #include <memory>

namespace vire {

  namespace simulation {

    simple_device::simple_device()
    {
      _out_of_order_.store(false);
      return;
    }

    // virtual
    simple_device::~simple_device()
    {
      return;
    }

    bool simple_device::is_out_of_order() const
    {
      return _out_of_order_.load();
    }
      
    void simple_device::breakdown()
    {
      _out_of_order_.store(true);
      _at_breakdown_();
       return;
    }

    void simple_device::repair()
    {
      _out_of_order_.store(false);
      _at_repair_();
      return;
    }

    // virtual
    void simple_device::_at_breakdown_()
    {
      return;
    }

    // virtual
    void simple_device::_at_repair_()
    {
      return;
    }
   
    // virtual
    void simple_device::print_tree(std::ostream & out_,
                                   const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      this->datatools::enriched_base::print_tree(out_, base_print_options::force_inheritance(options_));

      out_ <<  popts.indent << inherit_tag(popts.inherit)
           << "Out of order : " << std::boolalpha << is_out_of_order() << std::endl;

      return;
    }

  } // namespace simulation

} // namespace vire
