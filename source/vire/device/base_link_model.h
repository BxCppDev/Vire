//! \file  vire/device/base_link_model.h
//! \brief Base class for a link model
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

#ifndef VIRE_DEVICE_BASE_LINK_MODEL_H
#define VIRE_DEVICE_BASE_LINK_MODEL_H

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
#include <vire/device/link_types.h>
#include <vire/device/device_macros.h>
#include <vire/device/port_utils.h>
#include <vire/device/port_types.h>

namespace vire {

  namespace device {

    // Forward class declarations:
    class logical_link;

    //! \brief Base class for link models
    class base_link_model : public datatools::enriched_base
    {
    public:

      //! Check if the link is virtual
      bool is_virtual() const;

      //! Set the virtual flag
      void set_virtual(bool v_);

      //! Check if link has a type
      bool has_type() const;

      //! Set the type of the link
      void set_type(link_type type_);

      //! Reset the type of the link
      void reset_type();

      //! Return the type of the link
      link_type get_type() const;

      //! Check if start port has a type
      bool has_start_port_type() const;

      //! Set the type of the start port
      void set_start_port_type(port_type type_);

      //! Reset the type of the start port
      void reset_start_port_type();

      //! Return the type of the start port
      port_type get_start_port_type() const;

      //! Check if stop port has a type
      bool has_stop_port_type() const;

      //! Set the type of the stop port
      void set_stop_port_type(port_type type_);

      //! Reset the type of the stop port
      void reset_stop_port_type();

      //! Return the type of the stop port
      port_type get_stop_port_type() const;

      /*
      //! Check if start port has a I/O type
      bool has_start_port_io_type() const;

      //! Set the I/O type of the start port
      void set_start_port_io_type(port_io_type type_);

      //! Reset the I/O type of the start port
      void reset_start_port_io_type();

      //! Return the I/O type of the start port
      port_io_type get_start_port_io_type() const;

      //! Check if stop port has a I/O type
      bool has_stop_port_io_type() const;

      //! Set the I/O type of the stop port
      void set_stop_port_io_type(port_io_type type_);

      //! Reset the I/O type of the stop port
      void reset_stop_port_io_type();

      //! Return the I/O type of the stop port
      port_io_type get_stop_port_io_type() const;
      */

      //! Check if link has a format
      bool has_format() const;

      //! Set the format of the link
      void set_format(const std::string &);

      //! Reset the format of the link
      void reset_format();

      //! Return the format of the link
      const std::string & get_format() const;

      //! Default constructor
      base_link_model();

      //! Constructor
      explicit base_link_model(link_type type_);

      //! Destructor
      virtual ~base_link_model();

      //! Check if the link model is initialized
      bool is_initialized() const;

      //! Initialize the link model
      void initialize_simple();

      //! Initialize the link model from a set of configuration properties
      void initialize(const datatools::properties & config_);

      //! Reset the link model
      void reset();

      /// Check the logical
      bool has_logical() const;

      /// Get a non mutable reference to the logical link
      const logical_link & get_logical() const;

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

      //! Get a mutable reference to the logical link
      logical_link & _grab_logical();

    private:

      //! Set initialization flag
      void _set_initialized_(bool);

      //! Instantiate the logical
      void _create_logical_();

    private:

      // Internal management:
      bool _initialized_; //!< Initialization flag

      // Configuration:
      bool         _virtual_;         //!< Virtual flag
      link_type    _type_;            //!< Type of the link model (optional)
      port_type    _start_port_type_; //!< Type of the start connectable port model (optional)
      port_type    _stop_port_type_;  //!< Type of the stop connectable port model (optional)
      // port_io_type _start_port_io_type_; //!< I/O type of the start connectable port model
      // port_io_type _stop_port_io_type_;  //!< I/O type of the stop connectable port model
      std::string  _format_;             //!< Format (optional)

      // Working data:
      datatools::handle<logical_link> _logical_; //!< Handle pointer to the logical link associated to the link model

      // Factory stuff :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_link_model);

      //! Registration of a system factory
      VIRE_DEVICE_LINK_REGISTRATION_INTERFACE(base_link_model);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace device

} // namespace vire

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(vire::device::base_link_model, "vire::device::base_link_model");

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::base_link_model' class:
DR_CLASS_INIT(vire::device::base_link_model);
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_BASE_LINK_MODEL_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
