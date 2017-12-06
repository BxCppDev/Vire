//! \file  vire/device/base_electronics_module_model.h
//! \brief Base class for an electronics module model
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

#ifndef VIRE_DEVICE_BASE_ELECTRONICS_MODULE_MODEL_H
#define VIRE_DEVICE_BASE_ELECTRONICS_MODULE_MODEL_H

// Standard library:
#include <iostream>
#include <string>

// This project:
#include <vire/device/base_device_model.h>

namespace vire {

  namespace device {

    //! \brief Base class for electronics module/board models
    class base_electronics_module_model
      : public base_device_model
    {
    public:

      //! Default constructor
      base_electronics_module_model();

      //! Destructor
      virtual ~base_electronics_module_model();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Check if the module slot width is known
      bool has_slot_width() const;

      //! Return the slot width
      uint32_t get_slot_width() const;

      //! Set the slot width
      void set_slot_width(uint32_t);

      //! Check if the module maximum number of mezzanine modules is known
      bool has_max_number_of_mezzanine_modules() const;

      //! Return the max number of mezzanine modules
      uint32_t get_max_number_of_mezzanine_modules() const;

      //! Set the max number of mezzanine modules
      void set_max_number_of_mezzanine_modules(uint32_t);

      //! Check if the module has a known format
      bool has_format() const;

      //! Return the format
      const std::string & get_format() const;

      //! Set the format
      void set_format(const std::string &);

      //! Check if module has a known format for mezzanine modules
      bool has_mezzanine_format() const;

      //! Return the mezzanine format
      const std::string & get_mezzanine_format() const;

      //! Set the mezzanine format
      void set_mezzanine_format(const std::string &);

      //! Check if there is a mezzanine module at given slot
      bool has_mezzanine_module(uint32_t mezzanine_slot_) const;

      //! Add a mezzanine module
      physical_device &
      add_mezzanine_module(uint32_t mezzanine_slot_,
                           const base_device_model & mezzanine_model_,
                           const std::string & mezzanine_label_ = "");

      //! Remove a mezzanine module
      void remove_mezzanine_module(uint32_t mezzanine_slot_);

      //! Return a non mutable reference to an embedded mezzanine_module by slot
      const physical_device & get_mezzanine_module(uint32_t mezzanine_slot_);

    protected:

      //! Overloaded initialization method
      virtual void _at_initialize(const datatools::properties & config_,
                                  model_repository & models_);

      //! Overloaded reset method
      virtual void _at_reset();

      //! Basic initialization of the module model
      void _module_initialize(const datatools::properties & config_,
                              model_repository & models_);

      //! Common termination of the module model
      void _module_reset();

      //! Set default attribute values
      void _set_defaults();

      //! Post remove
      virtual void _post_remove_embedded_physical(const std::string & embedded_label_);

    private:

      std::string _format_; //!< Format of the module
      uint32_t    _slot_width_; //!< Width of the module in slot unit
      uint32_t    _max_number_of_mezzanine_modules_; //!< Maximum number of mezzanine modules
      std::string _mezzanine_format_; //!< Format of the embedded mezzanine modules
      indexed_labels_dict_type _mezzanine_labels_; //!< List of mezzanine labels by slot index

      //! Registration of a system factory
      VIRE_DEVICE_REGISTRATION_INTERFACE(base_electronics_module_model);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace device

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::base_electronics_module_model' class:
DR_CLASS_INIT(vire::device::base_electronics_module_model);
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_BASE_ELECTRONICS_MODULE_MODEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
