//! \file  vire/device/base_electronics_rack_model.h
//! \brief Base class for an electronics rack model
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

#ifndef VIRE_DEVICE_BASE_ELECTRONICS_RACK_MODEL_H
#define VIRE_DEVICE_BASE_ELECTRONICS_RACK_MODEL_H

// Standard library:
#include <iostream>
#include <string>

// This project:
#include <vire/device/base_device_model.h>

namespace vire {

  namespace device {

    //! \brief Base class for electronics rack models
    /**
     *
     *        Rack model:
     *
     *        +-------------------------+
     *        |                         |
     *        |                         |
     *     2  |         Crate 2         |
     *        |                         |
     *        |                         |
     *        |                         |
     *        +-------------------------+
     *        |                         |
     *        |                         |
     *     1  |         Crate 1         |
     *        |                         |
     *        |                         |
     *        |                         |
     *        +-------------------------+
     *        |                         |
     *        |                         |
     *     0  |         Crate 0         |
     *        |                         |
     *  Slot  |                         |
     *        |                         |
     *        +-------------------------+
     *
     *
     */
    class base_electronics_rack_model : public base_device_model
    {
    public:

      //! Default constructor
      base_electronics_rack_model();

      //! Destructor
      virtual ~base_electronics_rack_model();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Check if a maximum number of crates is set
      bool has_max_number_of_crates() const;

      //! Set the max number of crates
      void set_max_number_of_crates(uint32_t);

      //! Return the max number of crates
      uint32_t get_max_number_of_crates();

      //! Check if the rack has a known format
      bool has_format() const;

      //! Return the format
      const std::string & get_format() const;

      //! Set the format
      void set_format(const std::string &);

      //! Check if there is a crate at given slot
      bool has_crate(uint32_t crate_slot_) const;

      //! Add a crate
      physical_device &
      add_crate(uint32_t crate_slot_,
                 const base_device_model & crate_model_,
                 const std::string & crate_label_ = "");

      //! Remove a crate
      void remove_crate(uint32_t module_slot_);

      //! Return a non mutable reference to an embedded crate by slot
      const physical_device & get_crate(uint32_t crate_slot_);

    protected:

      //! Set default attribute values
      void _set_defaults();

      //! Overloaded initialization method
      virtual void _at_initialize(const datatools::properties & config_,
                                  model_repository & models_);

      //! Overloaded reset method
      virtual void _at_reset();

      //! Common initialization of rack
      void _rack_initialize(const datatools::properties & config_,
                            model_repository & models_);

      //! Common termination of the rack
      void _rack_reset();

      //! Post remove
      virtual void _post_remove_embedded_physical(const std::string & embedded_label_);

    private:

      std::string _format_; //!< Format of the rack
      uint32_t    _max_number_of_crates_; //!< Maximum number of crates
      indexed_labels_dict_type _crate_labels_; //!< List of crates labels by slot index

      //! Registration of a system factory
      VIRE_DEVICE_REGISTRATION_INTERFACE(base_electronics_rack_model);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN
    };

  } // namespace device

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::base_electronics_rack_model' class:
DR_CLASS_INIT(vire::device::base_electronics_rack_model);
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_BASE_ELECTRONICS_RACK_MODEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
