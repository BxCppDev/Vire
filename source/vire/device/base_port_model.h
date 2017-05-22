//! \file  vire/device/base_port_model.h
//! \brief Base class for a port model
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

#ifndef VIRE_DEVICE_BASE_PORT_MODEL_H
#define VIRE_DEVICE_BASE_PORT_MODEL_H

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <datatools/enriched_base.h>
#include <datatools/handle.h>
#include <datatools/factory_macros.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// This project:
#include <vire/vire.h>
#include <vire/device/device_utils.h>
#include <vire/device/port_types.h>
#include <vire/device/port_utils.h>
#include <vire/device/device_macros.h>

namespace vire {

  namespace device {

    // Forward class declarations:
    class logical_port;

    //! \brief Base class for port models
    class base_port_model : public datatools::enriched_base
    {
    public:

      //! Check if the port is virtual
      bool is_virtual() const;

      //! Set the virtual flag
      void set_virtual(bool v_);

      //! Check if port has a type
      bool has_type() const;

      //! Set the type of the port
      void set_type(port_type type_);

      //! Reset the type of the port
      void reset_type();

      //! Return the type of the port
      port_type get_type() const;

      //! Check if port has a I/O type
      bool has_io_type() const;

      //! Set the I/O type of the port
      void set_io_type(port_io_type type_);

      //! Reset the I/O type of the port
      void reset_io_type();

      //! Return the I/O type of the port
      port_io_type get_io_type() const;

      //! Check if port has a format
      bool has_format() const;

      //! Set the format of the port
      void set_format(const std::string &);

      //! Reset the format of the port
      void reset_format();

      //! Return the format of the port
      const std::string & get_format() const;

      //! Default constructor
      base_port_model();

      //! Constructor
      explicit base_port_model(port_type type_);

      //! Destructor
      virtual ~base_port_model();

      //! Check if the port model is initialized
      bool is_initialized() const;

      //! Initialize the port model
      void initialize_simple();

      //! Initialize the port model from a set of configuration properties
      void initialize(const datatools::properties & config_);

      //! Reset the port model
      void reset();

      /// Check the logical
      bool has_logical() const;

      /// Get a non mutable reference to the logical port
      const logical_port & get_logical() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected:

      //! Overloaded initialization method
      virtual void _at_initialize(const datatools::properties & config_);

      //! Overloaded reset method
      virtual void _at_reset();

      //! Set default attributes values
      void _set_defaults();

      //! Get a mutable reference to the logical port
      logical_port & _grab_logical();

    private:

      //! Set initialization flag
      void _set_initialized_(bool);

      //! Instantiate the logical
      void _create_logical_();

    private:

      // Internal management:
      bool _initialized_; //!< Initialization flag

      // Configuration:
      bool         _virtual_; //!< Virtual flag
      port_type    _type_;    //!< Type of the port model (optional)
      port_io_type _io_type_; //!< I/O type of the port model (optional)
      std::string  _format_;  //!< Format (optional)

      // Working data:
      datatools::handle<logical_port> _logical_; //!< Handle pointer to the logical port associated to the port model

      // Factory stuff :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_port_model)

      //! Registration of a system factory
      VIRE_DEVICE_PORT_REGISTRATION_INTERFACE(base_port_model)

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI()
#endif // Q_MOC_RUN

    };

  } // namespace device

} // namespace vire

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(vire::device::base_port_model, "vire::device::base_port_model")

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::base_port_model' class:
DR_CLASS_INIT(vire::device::base_port_model)
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_BASE_PORT_MODEL_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
