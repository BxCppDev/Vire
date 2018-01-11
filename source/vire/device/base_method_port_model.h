//! \file  vire/device/base_method_port_model.h
//! \brief Base class for a method port model
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

#ifndef VIRE_DEVICE_BASE_METHOD_PORT_MODEL_H
#define VIRE_DEVICE_BASE_METHOD_PORT_MODEL_H

// Standard library:
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <datatools/introspection/method.h>

// This project:
#include <vire/utility/rw_access.h>
#include <vire/device/base_port_model.h>

namespace vire {

  namespace device {

    //! \brief Base class for method port models
    class base_method_port_model
      : public base_port_model
    {
    public:

      //! Type aliad for a shared pointer on a method
      typedef std::shared_ptr<datatools::introspection::method> method_ptr_type;

      //! Default constructor
      base_method_port_model();

      //! Destructor
      virtual ~base_method_port_model();

      //! Check if the R/W access is set
      bool has_rw_access() const;

      //! Set the R/W access
      void set_rw_access(vire::utility::rw_access_type);

      //! Return the R/W access
      vire::utility::rw_access_type get_rw_access() const;

      //! Check if method is set
      bool has_method() const;

      //! Set the method
      void set_method(const method_ptr_type &);

      //! Reset the method
      void reset_method();

      //! Return the description of the method
      const ::datatools::introspection::method & get_method() const;

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

      //! Set default attribute values
      void _set_defaults();

    private:

      vire::utility::rw_access_type _rw_access_; //!< Read/write access
      method_ptr_type               _method_;    //!< Description/introspection of the method

      //! Registration of a system factory
      VIRE_DEVICE_PORT_REGISTRATION_INTERFACE(base_method_port_model);

    };

  } // namespace device

} // namespace vire

#endif // VIRE_DEVICE_BASE_METHOD_PORT_MODEL_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
