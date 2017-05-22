// vire/device/i_instance.cc
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/device/i_instance.h>

// Third party:
// - Boost:
#include <boost/algorithm/string/trim.hpp>

// This project:
#include <vire/device/single_instance.h>

namespace vire {

  namespace device {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(i_instance,
                                                     "vire::device::i_instance/__system__");

    const i_instance & default_instance()
    {
      static boost::scoped_ptr<single_instance> _ptr;
      if (!_ptr) {
        _ptr.reset(new single_instance);
        _ptr->grab_slot().make_dimension_0(true);
      }
      return dynamic_cast<const i_instance &>(*_ptr);
    }

    i_instance::i_instance()
    {
      return;
    }

    i_instance::~i_instance()
    {
      DT_LOG_TRACE_ENTERING(datatools::logger::PRIO_TRACE);
      return;
    }

    datatools::logger::priority i_instance::get_logging() const
    {
      return _logging_;
    }

    void i_instance::set_logging(const datatools::logger::priority p_)
    {
      _logging_ = p_;
      return;
    }

    bool i_instance::has_dimension() const
    {
      return get_dimension() != space::DIM_INVALID;
    }

    bool i_instance::is_multiple() const
    {
      return get_number_of_items() > 1;
    }

    bool i_instance::is_singleton() const
    {
      // DT_LOG_TRACE_ENTERING(datatools::logger::PRIO_TRACE);
      return get_number_of_items() == 1;
    }

    bool i_instance::has_slot(const slot & s_) const
    {
      if (s_.get_dimension() != this->get_dimension()) {
        return false;
      }
      for (int item = 0; item < (int) get_number_of_items(); item++) {
        slot s;
        this->fetch_slot(item, s);
        if (s_ == s) {
          return true;
        }
      }
      return false;
    }

    bool i_instance::belongs_to(const i_instance & ref_) const
    {
      for (std::size_t i = 0; i < get_number_of_items(); i++) {
        slot s;
        fetch_slot(i, s);
        if (!ref_.has_slot(s)) {
          return false;
        }
      }
      return true;
    }

    void i_instance::tree_dump(std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_,
                                bool inherit_) const
    {
      // DT_LOG_TRACE_ENTERING(datatools::logger::PRIO_TRACE);
      if (! title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Address    = [@" << this << "]" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Dimension  = ";
      if (has_dimension()) {
        out_ << get_dimension();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Multiple   = " << (is_multiple()? "<yes>" : "<no>" ) << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Singleton   = " << (is_singleton()? "<yes>" : "<no>" ) << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Number of items = " << get_number_of_items() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "List of slots: "
           << std::endl;
      for (std::size_t i = 0; i < get_number_of_items(); i++) {
        slot s;
        fetch_slot(i, s);
        out_ << indent_ << datatools::i_tree_dumpable::skip_tag;
        if (i + 1 == get_number_of_items()) {
          out_ << datatools::i_tree_dumpable::last_tag;
        } else {
          out_ << datatools::i_tree_dumpable::tag;
        }
        out_ << "Slot #" << i << " : " << s;
        out_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Valid      = " << (is_valid() ? "<yes>" : "<no>" ) << std::endl;
      // DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "TEST A...");

      // DT_LOG_TRACE_EXITING(datatools::logger::PRIO_TRACE);
      return;
    }

    // static
    i_instance * i_instance::create(const std::string & instance_id_, const std::string & from_)
    {
      i_instance * allocated = 0;
      if (::vire::device::i_instance::get_system_factory_register().has(instance_id_)) {
        const ::vire::device::i_instance::factory_register_type::factory_type & the_factory
          = ::vire::device::i_instance::get_system_factory_register().get(instance_id_);
        allocated = the_factory();
        if (! allocated->from_string(from_)) {
          delete allocated;
          allocated = 0;
        }
      }
      return allocated;
    }

    // static
    i_instance * i_instance::create(const std::string & rule_)
    {
      i_instance * allocated = 0;
      std::string instance_id;
      std::string from;
      std::istringstream iss(rule_);
      std::getline(iss, instance_id, '=');
      boost::trim(instance_id);
      std::getline(iss, from);
      boost::trim(from);
      // std::cerr << "DEVEL: i_instance::create: instance_id = '" << instance_id << "'" << std::endl;
      // std::cerr << "DEVEL: i_instance::create: from        = '" << from << "'" << std::endl;
      allocated = create(instance_id, from);
      return allocated;
    }

  } // namespace device

} // namespace vire
