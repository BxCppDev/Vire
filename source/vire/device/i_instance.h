//! \file  vire/device/i_instance.h
//! \brief Base interface class for instance object
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

#ifndef VIRE_DEVICE_I_INSTANCE_H
#define VIRE_DEVICE_I_INSTANCE_H

// Standard library:
#include <iostream>
#include <string>
#include <vector>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// #include <boost/serialization/access.hpp>
// - Bayeux/datatools:
// #include <datatools/i_serializable.h>
#include <datatools/i_tree_dump.h>
#include <datatools/factory_macros.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// This project:
#include <vire/device/space.h>
#include <vire/device/slot.h>

namespace vire {

  namespace device {

    // Forward class declaration:
    class i_instance;
    class single_instance;

    //! Return the default instance constant
    const i_instance & default_instance();

    //! \brief Abstract base class for instance classes
    //!
    //! An instance represents the positioning rule for a concrete (physical) object
    //! within the instantiation space of its mother (logical) object
    //! An instance can represent a single positioning : a unique daughter object is positioned
    //! in its mother.
    //! An instance can represent the one dimensional replicated positioning of the same logical object
    //! in its mother. Example: repeat 5 times the same model of a dining table in a refectory and arrange
    //! them along a line.
    //! An instance can represent the two dimensional replicated positioning of the same logical object
    //! in its mother. Example: repeat 3x2=6 times the same model of an egg in a egg carton with 6 slots.
    //!
    class i_instance : public datatools::i_tree_dumpable
    {
    public:

      //! Default constructor
      i_instance();

      //! Destructor
      virtual ~i_instance();

      //! Return the logging priority
      datatools::logger::priority get_logging() const;

      //! Set the logging priority
      void set_logging(const datatools::logger::priority);

      //! Check the validity of the dimension of the instantiation space
      bool has_dimension() const;

      //! Return the dimension of the instantiation space
      virtual space::dimension_type get_dimension() const = 0;

      //! Check validity
      virtual bool is_valid() const = 0;

      //! Reset
      virtual void reset() = 0;

      //! Return the number of instance items
      virtual size_t get_number_of_items() const = 0;

      //! Check if the instance handles multiple instances
      bool is_multiple() const;

      //! Check if the instance is a singleton
      bool is_singleton() const;

      //! Compute an array of indexes in a multidimensional frame from the item rank/index
      // virtual size_t compute_index_map(std::vector<uint32_t> & map_, int item_) const;

      //! Compute the slot at given item rank/index
      virtual void fetch_slot(int item_, slot & s_) const = 0;

      //! Check if a given slot belongs to the instantiation space
      bool has_slot(const slot & s_) const;

      //! Check if the instance is included in the reference instance
      bool belongs_to(const i_instance & ref_) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_         = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

      //! Parse from a string
      virtual bool from_string(const std::string &) = 0;

      //! Allocate a new instance object from the class identifier and a string
      static i_instance * create(const std::string & instance_id_, const std::string & from_);

      //! Allocate a new instance object from the rule
      static i_instance * create(const std::string & rule_);

    private:

      datatools::logger::priority _logging_ = datatools::logger::PRIO_FATAL;

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI()
#endif // Q_MOC_RUN

      // Factory stuff :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(i_instance)

    };

  } // namespace device

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the vire::device::i_instance class :
DR_CLASS_INIT(::vire::device::i_instance)
#endif // Q_MOC_RUN

// Instance registration:

#define VIRE_DEVICE_INSTANCE_REGISTRATION_INTERFACE(DeviceInstanceClassName) \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::device::i_instance, DeviceInstanceClassName) \
  /**/

#define VIRE_DEVICE_INSTANCE_REGISTRATION_IMPLEMENT(DeviceInstanceClassName, DeviceInstanceClassId) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(::vire::device::i_instance, DeviceInstanceClassName, DeviceInstanceClassId) \
  /**/

#endif // VIRE_DEVICE_I_INSTANCE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
