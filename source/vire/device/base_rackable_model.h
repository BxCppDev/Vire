//! \file  vire/device/base_rackable_model.h
//! \brief Base class for a rackable device model
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_DEVICE_BASE_RACKABLE_MODEL_H
#define VIRE_DEVICE_BASE_RACKABLE_MODEL_H

// Standard library:
#include <iostream>
#include <string>

// This project:
#include <vire/device/base_device_model.h>

namespace vire {

  namespace device {

    //! \brief Base class for rackable device models
    /**
     *
     *        Rackable model:
     *        +-----------------------------------+  ^
     *        | format                            |  |
     *        |                                   |  | height
     *        +-----------------------------------+  v
     *        <----------------------------------->
     *                         width
     */
    class base_rackable_model
      : public base_device_model
    {
    public:

      //! Default constructor
      base_rackable_model();

      //! Destructor
      virtual ~base_rackable_model();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;


      //! Check if a height is set
      bool has_height() const;

      //! Set the height of the crate
      void set_height(uint32_t);

      //! Return the height of the crate
      uint32_t get_height() const;

      //! Check if a width is set
      bool has_width() const;

      //! Set the width of the crate
      void set_width(uint32_t);

      //! Return the width of the crate
      uint32_t get_width() const;

      //! Check if the crate has a known format
      bool has_format() const;

      //! Return the format
      const std::string & get_format() const;

      //! Set the format
      void set_format(const std::string &);

    protected:

      //! Set default attribute values
      void _set_defaults();

      //! Overloaded initialization method
      virtual void _at_initialize(const datatools::properties & config_,
                                  model_repository & models_);

      //! Overloaded reset method
      virtual void _at_reset();

      //! Common initialization of crate
      void _rackable_initialize(const datatools::properties & config_,
				model_repository & models_);

      //! Common termination of the crate
      void _rackable_reset();

    private:

      std::string _format_; //!< Format of the rackable device
      uint32_t    _height_; //!< Height of the rackable device
      uint32_t    _width_;  //!< Width of the rackable device

      //! Registration of a system factory
      VIRE_DEVICE_REGISTRATION_INTERFACE(base_rackable_model);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace device

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::base_rackable_model' class:
DR_CLASS_INIT(vire::device::base_rackable_model);
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_BASE_RACKABLE_MODEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
