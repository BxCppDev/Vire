//! \file  vire/device/device_utils.h
//! \brief Vire device utilities
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

#ifndef VIRE_DEVICE_DEVICE_UTILS_H
#define VIRE_DEVICE_DEVICE_UTILS_H

// Standard library
#include <string>
#include <map>

// Third party
// - Boost
#include <boost/cstdint.hpp>

// - Bayeux/datatools
#include <datatools/handle.h>
#include <datatools/bit_mask.h>
#include <datatools/i_tree_dump.h>
#include <datatools/properties.h>

namespace vire {

  //! Nested namespace for the Vire library's device module
  namespace device {

    //! Build a name from a prefix and an integral index
    std::string make_indexed_name(const std::string & prefix_,
                                  uint32_t index_,
                                  const std::string & suffix_= "");

    //! Build a name from a prefix and two integral indexes
    std::string make_double_indexed_name(const std::string & prefix_,
                                         uint32_t index1_,
                                         uint32_t index2_,
                                         const std::string & sep_ = "_",
                                         const std::string & suffix_= "");

    //! Return the name suffix of logical objects
    const std::string & logical_name_suffix();

    //! Return the name of a logical object from its base name
    std::string make_logical_name(const std::string & basename_);

    // Forward class declarations:
    class logical_device;
    class physical_device;
    class base_device_model;
    class base_port_model;
    class manager;

    //! \brief Type alias for handle to device model
    typedef datatools::handle<base_device_model> device_model_handle_type;

    //! \brief Type alias for handle to port model
    typedef datatools::handle<base_port_model> port_model_handle_type;

    //! Return the label associated to device objects
    const std::string & device_label();

    //! Return the label associated to port objects
    const std::string & port_label();

    //! \brief Type of model (device, port...)
    enum model_type {
      MODEL_UNDEFINED = 0, //!< Undefined model
      MODEL_DEVICE    = 1, //!< Device model
      MODEL_PORT      = 2  //!< Port model
    };

    //! Get label from the model type
    std::string get_label_from_model_type(model_type);

    //! Get model type from the label
    model_type get_model_type_from_label(const std::string &);

    //! \brief Record entry for model objects stored in the dictionary of the device manager
    class model_entry : public datatools::i_tree_dumpable
    {
    public:

      //! Return the device model name
      const std::string & get_name() const;

      //! Set the device model name
      void set_name(const std::string &);

      //! Return the model type
      model_type get_model_type() const;

      //! Set the model type
      void set_model_type(model_type);

      //! Check if the model is a device model
      bool is_device_model() const;

      //! Check if the model is a port model
      bool is_port_model() const;

      //! Return the model ID
      const std::string & get_model_id() const;

      //! Set the model ID
      void set_model_id(const std::string &);

      //! Return a non mutable reference to the model configuration
      const datatools::properties & get_config() const;

      //! Return a mutable reference to the model configuration
      datatools::properties & grab_config();

      //! Set the model configuration
      void set_config(const datatools::properties &);

      //! Check if there is the device manager
      bool has_device_manager() const;

      //! Install a handle to the device manager
      void set_device_manager(manager &);

      //! Reset the handle to the device manager
      void reset_device_manager();

      //! Default constructor
      model_entry();

      //! Destructor
      ~model_entry();

      //! Check if the device model is created
      bool is_created() const;

      //! Check if the device model is initialized
      bool is_initialized() const;

      //! Smart print
      virtual void tree_dump(std::ostream& out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherits_ = false) const;

      //! Return a non mutable handle to the device model
      const device_model_handle_type & get_device_model_handle() const;

      //! Return a mutable handle to the device model
      device_model_handle_type & grab_device_model_handle();

      //! Return a mutable handle to the device model
      device_model_handle_type & grab_device_model_handle_plain();

      //! Return a non mutable handle to the port model
      const port_model_handle_type & get_port_model_handle() const;

      //! Return a mutable handle to the port model
      port_model_handle_type & grab_port_model_handle();

      //! Return a mutable handle to the port model
      port_model_handle_type & grab_port_model_handle_plain();

    private:

      //! Return a mutable handle to the initialized device model
      device_model_handle_type & _grab_device_model_handle();

      //! Return a mutable handle to the initialized port model
      port_model_handle_type & _grab_port_model_handle();

    private:

      // Configuration:
      std::string           _name_;           //!< The name of the model
      model_type            _model_type_;     //!< Type of model
      std::string           _model_id_;       //!< The class identifier of the model
      datatools::properties _config_;         //!< The configuration of the model
      manager *             _device_manager_; //!< Handle to a device manager

      // Working internal data:
      device_model_handle_type _device_model_handle_; //!< The handle for the allocated device model
      port_model_handle_type   _port_model_handle_;   //!< The handle for the allocated port model

    };

    //! Dictionary of device model handles by label
    typedef std::map<std::string, device_model_handle_type> device_model_dict_type;

    //! Dictionary of device model handles by index
    typedef std::map<uint32_t, device_model_handle_type> indexed_device_model_dict_type;

    //! Dictionary of device model names by index
    typedef std::map<uint32_t, std::string> indexed_labels_dict_type;

    //! Dictionary of model entries by label
    typedef std::map<std::string, model_entry> model_pool_type;

    //! Decode the model type and name from a string
    bool decode_model_key(const std::string & full_name_,
                          model_type & mtype_,
                          std::string & model_name_);

    //! Encode the model type and name to a string
    bool encode_model_key(std::string & full_name_,
                          model_type mtype,
                          const std::string & model_name_);

    //! \brief Repository of various kinds of models
    class model_repository
    {
    public:

      //! Default constructor
      model_repository();

      //! Set reference to a dictionary of models
      void set_models(model_pool_type & models_);

      //! Check models
      bool has_models() const;

      //! Return a dictionary of models
      const model_pool_type & get_models() const;

      //! Return a dictionary of models
      model_pool_type & grab_models();

      //! Check is a device model with given name exists
      bool has_device_model(const std::string & model_name_) const;

      //! Check is a port model with given name exists
      bool has_port_model(const std::string & model_name_) const;

      //! Return a handle to a device model given its name
      const device_model_handle_type & get_device_model_handle(const std::string & model_name_) const;

      //! Return a handle to a port model given its name
      const port_model_handle_type & get_port_model_handle(const std::string & model_name_) const;

      //! Return a reference to a device model given its name
      const base_device_model & get_device_model(const std::string & model_name_) const;

      //! Return a reference to a port model given its name
      const base_port_model & get_port_model(const std::string & model_name_) const;

    private:

      model_pool_type * _models_; //!< Dictionary of models

    };

    //! Handle on physical device
    typedef datatools::handle<physical_device> physical_device_handle_type;

    //! Dictionary of physical device handles
    typedef std::map<std::string, physical_device_handle_type> physical_device_dict_type;

    //! Predicate to find a value in a map
    template<class T>
    struct map_data_compare : public std::binary_function<typename T::value_type,
                                                          typename T::mapped_type,
                                                          bool>
    {
    public:

      bool operator() (typename T::value_type & pair_,
                       typename T::mapped_type value_) const
      {
        return pair_.second == value_;
      }

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_DEVICE_UTILS_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
