//! \file  vire/device/base_device_model.h
//! \brief Base class for a device model
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

#ifndef VIRE_DEVICE_BASE_DEVICE_MODEL_H
#define VIRE_DEVICE_BASE_DEVICE_MODEL_H

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>
#include <set>

// Third party:
// - Bayeux/datatools:
#include <datatools/enriched_base.h>
#include <datatools/handle.h>
#include <datatools/factory_macros.h>
#include <datatools/version_id.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// This project:
#include <vire/vire.h>
#include <vire/device/device_utils.h>
#include <vire/device/device_types.h>
#include <vire/device/device_macros.h>

namespace vire {

  namespace device {

    // Forward class declarations:
    class logical_device;
    class logical_port;
    class physical_device;
    class base_port_model;
    class physical_port;
    class physical_link;
    class i_instance;
    class device_link_info;

    //! \brief Base class for device models
    class base_device_model : public datatools::enriched_base
    {
    public:

      typedef uint32_t device_model_key;
      static const device_model_key INVALID_DEVICE_MODEL_KEY = 0xFFFFFFFF;

      //! Type alias for a dictionary of physical devices
      typedef std::map<std::string, physical_device> physical_device_dict_type;

      //! Type alias for a dictionary of ports
      typedef std::map<std::string, physical_port>   physical_port_dict_type;

      //! Type alias for a dictionary of links
      typedef std::map<std::string, physical_link>   physical_link_dict_type;

      //! Default constructor
      base_device_model();

      //! Constructor
      explicit base_device_model(device_type type_);

      //! Destructor
      virtual ~base_device_model();

      //! Check if device has a model key
      bool has_model_key() const;

      //!< Return the unique device model key
      device_model_key get_model_key() const;

      //!< Set the unique device model key
      void set_model_key(device_model_key);

      //!< Reset the unique device model key
      void reset_model_key();

      //! Check if device has a version
      bool has_version() const;

      //!< Device model version
      const datatools::version_id & get_version() const;

      //!< Set the device model version
      void set_version(const datatools::version_id &);

      //!< Reset the device model version
      void reset_version();

      //! Check if device is virtual
      bool is_virtual() const;

      //! Set the virtual flag of the device
      void set_virtual(bool);

      //! Check if device has a type
      bool has_type() const;

      //! Set the type of the device
      void set_type(device_type type_);

      //! Reset the type of the device
      void reset_type();

      //! Return the type of the device
      device_type get_type() const;

      //! Allow embedded devices (default implementation)
      bool is_allow_embedded_devices() const;

      //! Set the allow embedded devices flag
      void set_allow_embedded_devices(bool);

      //! Check if there is an embedded device with a given label
      bool has_embedded_device(const std::string & device_label_) const;

      //! Add an embedded device
      physical_device & add_embedded_device(const std::string & device_label_,
                                            const base_device_model & device_model_,
                                            const i_instance * instance_ = 0,
                                            bool owned_instance_ = false);

      //! Add an embedded device
      physical_device & add_embedded_device(const std::string & device_label_,
                                            const base_device_model & device_model_,
                                            const i_instance & instance_);

      //! Remove an embedded device
      void remove_embedded_device(const std::string & device_label_);

      //! Return a non mutable reference to an embedded device by label
      const physical_device & get_embedded_device(const std::string & device_label_) const;

      //! Check if a port is defined given its name
      bool has_port(const std::string &) const;

      //! Add a port
      physical_port & add_port(const std::string & port_label_,
                               const base_port_model & port_model_,
                               const i_instance * instance_ = 0,
                               bool owned_instance_ = false);

      //! Add a port
      physical_port & add_port(const std::string & port_label_,
                               const base_port_model & port_model_,
                               const i_instance & instance_);

      //! Remove a port
      void remove_port(const std::string & port_label_);

      //! Return a non mutable reference to a port by label
      const physical_port & get_port(const std::string & port_label_) const;

      //! Check if a link is defined given its name
      bool has_link(const std::string &) const;

      //! Add a link
      physical_link & add_link(const std::string & link_label_,
                               const device_link_info & dli_);

      //! Remove a link
      void remove_link(const std::string & link_label_);

      //! Return a non mutable reference to a link by label
      const physical_link & get_link(const std::string & link_label_) const;

      //! Build the list of embedded device labels
      void fetch_embedded_device_labels(std::set<std::string> & edev_labels_) const;

      //! Build the list of port labels
      void fetch_port_labels(std::set<std::string> & port_labels_) const;

      //! Build the list of link labels
      void fetch_link_labels(std::set<std::string> & link_labels_) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Check if device model is initialized
      bool is_initialized() const;

      //! Initialize the device model
      void initialize_simple();

      //! Initialize the device model using only a list of properties without the needs of other device models
      void initialize_standalone(const datatools::properties &);

      //! Initialize the device model using a list of properties with access to a dictionary of other device models
      void initialize(const datatools::properties& config_,
                      model_repository & models_);

      //! Reset the device model
      void reset();

      /// Check the logical
      bool has_logical() const;

      /// Get a non mutable reference to the embedded logical device
      const logical_device & get_logical() const;

    protected:

      //! Check is a device type can be embedded in the current device model
      virtual bool _check_embedded_type(device_type) const;

      //! Get a mutable reference to the embedded logical device
      logical_device & _grab_logical();

      //! Add an embedded device
      physical_device & _add_embedded_physical(const std::string &,
                                               const logical_device &,
                                               const i_instance * instance_ = 0,
                                               bool owned_instance_ = false);

      //! Remove an embedded device
      void _remove_embedded_physical(const std::string &);

      //! Add a port
      physical_port & _add_physical_port(const std::string &,
                                         const logical_port &,
                                         const i_instance * instance_ = 0,
                                         bool owned_instance_ = false);

      //! Remove a port
      void _remove_physical_port(const std::string &);

      //! Add a link
      physical_link & _add_physical_link(const std::string &,
                                         const device_link_info & dli_);

      //! Remove a link
      void _remove_physical_link(const std::string &);

      //! Overloaded initialization method
      virtual void _at_initialize(const datatools::properties & config_,
                                  model_repository & models_);

      //! Overloaded reset method
      virtual void _at_reset();

      //! Pre add embedded physical device (default implementation does nothing)
      virtual void _pre_add_embedded_physical(const std::string & embedded_label_);

      //! Post add embedded physical device (default implementation does nothing)
      virtual void _post_add_embedded_physical(const std::string & embedded_label_);

      //! Pre remove embedded physical device (default implementation does nothing)
      virtual void _pre_remove_embedded_physical(const std::string & embedded_label_);

      //! Post remove embedded physical device (default implementation does nothing)
      virtual void _post_remove_embedded_physical(const std::string & embedded_label_);

      //! Pre add physical port (default implementation does nothing)
      virtual void _pre_add_physical_port(const std::string & port_label_);

      //! Post add physical port (default implementation does nothing)
      virtual void _post_add_physical_port(const std::string & port_label_);

      //! Pre remove physical port (default implementation does nothing)
      virtual void _pre_remove_physical_port(const std::string & port_label_);

      //! Post remove physical port (default implementation does nothing)
      virtual void _post_remove_physical_port(const std::string & port_label_);

      //! Basic initialization of the device model
      void _device_initialize(const datatools::properties & config_,
                              model_repository & models_);

      //! Common termination of the device model
      void _device_reset();

      //! Set default attribute values
      void _set_defaults();

    private:

      //! Set initialization flag
      void _set_initialized_(bool);

      //! Instantiate the logical
      void _create_logical_();

    private:

      // Internal management:
      bool                       _initialized_; //!< Initialization flag

      // Configuration:
      device_type                _type_;                      //!< Type of the device model
      bool                       _virtual_;                   //!< Virtual device flag
      device_model_key           _model_key_;                 //!< Device model unique identifier
      datatools::version_id      _version_;                   //!< Device model version identifier
      bool                       _allow_embedded_devices_;    //!< Flag to allow embedded devices
      physical_device_dict_type  _embedded_physical_devices_; //!< Embedded physical devices
      physical_port_dict_type    _physical_ports_;            //!< Physical ports
      physical_link_dict_type    _physical_links_;            //!< Physical links

      // Working data:
      datatools::handle<logical_device> _logical_;            //!< Handle pointer to the logical device associated to the device model

      // Factory stuff :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_device_model)

      //! Registration of a system factory
      VIRE_DEVICE_REGISTRATION_INTERFACE(base_device_model)

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI()
#endif // Q_MOC_RUN

    };

  } // namespace device

} // namespace vire

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT_KEY2(vire::device::base_device_model, "vire::device::base_device_model")

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::base_device_model' class:
DR_CLASS_INIT(vire::device::base_device_model)
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_BASE_DEVICE_MODEL_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
