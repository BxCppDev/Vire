//! \file  vire/device/base_datapoint_model.h
//! \brief Base class for a datapoint model
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

#ifndef VIRE_DEVICE_BASE_DATAPOINT_MODEL_H
#define VIRE_DEVICE_BASE_DATAPOINT_MODEL_H

// Standard library:
#include <string>
#include <map>
#include <memory>

// Third party
// - Boost
// #include <boost/shared_ptr.hpp>
// - Bayeux/datatools
#include <datatools/introspection/data_description.h>
#include <datatools/introspection/method.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// This project:
#include <vire/utility/rw_access.h>
#include <vire/device/base_device_model.h>
#include <vire/device/base_method_port_model.h>

namespace vire {

  namespace device {

    class base_method_port_model;

    //! \brief Base class for datapoint
    class base_datapoint_model : public base_device_model
    {
    public:

      // Type aliases
      typedef std::shared_ptr<base_method_port_model>           method_port_model_ptr_type;
      typedef std::map<std::string, method_port_model_ptr_type> method_port_model_dict_type;

      static const std::string & read_method_name();
      static const std::string & write_method_name();

      //! Default constructor
      base_datapoint_model();

      //! Destructor
      virtual ~base_datapoint_model();

      //! Check if the R/W access is set
      bool has_rw_access() const;

      //! Set the R/W access
      void set_rw_access(vire::utility::rw_access_type);

      //! Return the R/W access
      vire::utility::rw_access_type get_rw_access() const;

      //! Check if the data description is set
      bool has_data_description() const;

      //! Return the data description
      const datatools::introspection::data_description & get_data_description() const;

      //! Set the data description
      void set_data_description(const datatools::introspection::data_description & dd_);

      //! Check if standard interface is used
      bool is_standard_interface() const;

      //! Set the standard interface flag
      void set_standard_interface(bool);

      //! Return the dictionary of standard interface method port models
      const method_port_model_dict_type & get_standard_interface_method_port_models() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected:

      //! Set default attribute values
      void _set_defaults();

      //! Overloaded initialization method
      virtual void _at_initialize(const datatools::properties & config_,
                                  model_repository & models_);

      //! Overloaded reset method
      virtual void _at_reset();

      //! Common initialization of the datapoint
      void _datapoint_initialize(const datatools::properties & config_,
                                 model_repository & models_);

      //! Common termination of the datapoint
      void _datapoint_reset();

      //! Install the datapoint's standard interface
      void _install_standard_interface(const datatools::properties & config_);

      //! Build the 'read' method
      void _build_read_method(const datatools::properties & config_);

      //! Build the 'write' method
      void _build_write_method(const datatools::properties & config_);

    private:

      vire::utility::rw_access_type              _rw_access_; //!< Datapoint's read/write access
      datatools::introspection::data_description _dd_; //!< Datapoint's data description
      bool _standard_interface_; //!< Flag to activate the standard interface of the datapoint (read/write methods)
      method_port_model_dict_type _si_mpm_; //!< Standard interface methods (store for private method port models)

      //! Registration of a system factory
      VIRE_DEVICE_REGISTRATION_INTERFACE(base_datapoint_model);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace device

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::base_datapoint_model' class:
DR_CLASS_INIT(::vire::device::base_datapoint_model);
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_BASE_DATAPOINT_MODEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
