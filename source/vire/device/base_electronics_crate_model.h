//! \file  vire/device/base_electronics_crate_model.h
//! \brief Base class for an electronics crate model
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

#ifndef VIRE_DEVICE_BASE_ELECTRONICS_CRATE_MODEL_H
#define VIRE_DEVICE_BASE_ELECTRONICS_CRATE_MODEL_H

// Standard library:
#include <iostream>
#include <string>

// This project:
#include <vire/device/base_device_model.h>

namespace vire {

  namespace device {

    //! \brief Base class for electronics crate models
    /**
     *
     *        Crate model:
     *        +-----------------------------------+
     *        |+-+|+-+|   |   |+-+|   |   |   |   |
     *        ||B|||B|| ^ |   ||B|| ^ | ^ | ^ | ^ |
     *        ||o|||o|| e |   ||o|| e | e | e | e |
     *        ||a|||a|| m |   ||a|| m | m | m | m |
     *        ||r|||r|| p |   ||r|| p | p | p | p |
     *        ||d|||d|| t |   ||d|| t | t | t | t |
     *        || ||| || y |   || || y | y | y | y |
     *        ||0|||1|| v |   ||4|| v | v | v | v |
     *        || ||| ||   |   || ||   |   |   |   |
     *        || ||| ||   |   || ||   |   |   |   |
     *        |+-+|+-+|   |   |+-+|   |   |   |   |
     *        +-----------------------------------+
     *        |                                   |
     *        |                                   |
     *        +-----------------------------------+
     *   Slot   0   1   2   3   4   5   6   7   8
     *
     *
     */
    class base_electronics_crate_model : public base_device_model
    {
    public:

      //! Default constructor
      base_electronics_crate_model();

      //! Destructor
      virtual ~base_electronics_crate_model();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;


      //! Check if a maximum number of modules is set
      bool has_max_number_of_modules() const;

      //! Set the max number of modules
      void set_max_number_of_modules(uint32_t);

      //! Return the max number of modules
      uint32_t get_max_number_of_modules();

      //! Check if the crate has a known format
      bool has_format() const;

      //! Return the format
      const std::string & get_format() const;

      //! Set the format
      void set_format(const std::string &);

      //! Check if there is a module at given slot
      bool has_module(uint32_t module_slot_) const;

      //! Add a module
      physical_device &
      add_module(uint32_t module_slot_,
                 const base_device_model & module_model_,
                 const std::string & module_label_ = "");

      //! Remove a module
      void remove_module(uint32_t module_slot_);

      //! Return a non mutable reference to an embedded module by slot
      const physical_device & get_module(uint32_t module_slot_);

    protected:

      //! Set default attribute values
      void _set_defaults();

      //! Overloaded initialization method
      virtual void _at_initialize(const datatools::properties & config_,
                                  model_repository & models_);

      //! Overloaded reset method
      virtual void _at_reset();

      //! Common initialization of crate
      void _crate_initialize(const datatools::properties & config_,
                             model_repository & models_);

      //! Common termination of the crate
      void _crate_reset();

      //! Post remove
      virtual void _post_remove_embedded_physical(const std::string & embedded_label_);

    private:

      std::string _format_; //!< Format of the crate
      uint32_t    _max_number_of_modules_; //!< Maximum number of modules
      indexed_labels_dict_type _module_labels_; //!< List of module labels by slot index

      //! Registration of a system factory
      VIRE_DEVICE_REGISTRATION_INTERFACE(base_electronics_crate_model);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace device

} // namespace vire

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(vire::device::base_electronics_crate_model,
                        "vire::device::base_electronics_crate_model");

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::base_electronics_crate_model' class:
DR_CLASS_INIT(vire::device::base_electronics_crate_model);
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_BASE_ELECTRONICS_CRATE_MODEL_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
