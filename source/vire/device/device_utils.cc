//! \file vire/device/device_utils.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/device_utils.h>

// Standard Library:
#include <stdexcept>
#include <string>
#include <sstream>

// Third Party:
// - Boost:
#include <boost/algorithm/string.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/base_device_model.h>
#include <vire/device/manager.h>

namespace vire {

  namespace device {

    std::string make_indexed_name(const std::string & prefix_,
                                  uint32_t index_,
                                  const std::string & suffix_)
    {
      std::ostringstream oss;
      oss << prefix_ << index_ << suffix_;
      return oss.str();
    }

    std::string make_double_indexed_name(const std::string & prefix_,
                                         uint32_t index1_,
                                         uint32_t index2_,
                                         const std::string & sep_,
                                         const std::string & suffix_)
    {
      std::ostringstream oss;
      oss << prefix_ << index1_ << sep_ << index2_ << suffix_;
      return oss.str();
    }

    const std::string & logical_name_suffix()
    {
      static const std::string _suffix(".log");
      return _suffix;
    }

    std::string make_logical_name(const std::string & basename_)
    {
      return basename_ + logical_name_suffix();
    }

    const std::string & device_label()
    {
      static const std::string _label("device");
      return _label;
    }

    const std::string & port_label()
    {
      static const std::string _label("port");
      return _label;
    }

    std::string get_label_from_model_type(model_type t_)
    {
      switch (t_) {
      case MODEL_DEVICE: return device_label();
      case MODEL_PORT: return port_label();
      }
      return "";
    }

    model_type get_model_type_from_label(const std::string & label_)
    {
      if (label_ == device_label()) {
        return MODEL_DEVICE;
      } else if (label_ == port_label()) {
        return MODEL_PORT;
      }
      return MODEL_UNDEFINED;
    }

    bool encode_model_key(std::string & full_name_,
                          model_type mtype,
                          const std::string & model_name_)
    {
      full_name_.clear();
      if (mtype == MODEL_UNDEFINED) {
        return false;
      }
      std::ostringstream oss;
      oss << model_name_ << '@' << get_label_from_model_type(mtype);
      full_name_ = oss.str();
      return true;
    }

    bool decode_model_key(const std::string & model_name_info_,
                          model_type & mtype_, std::string & model_name_)
    {
      mtype_ = MODEL_UNDEFINED;
      model_name_.clear();
      typedef std::vector<std::string > split_vector_type;
      split_vector_type splitted_name;
      boost::split(splitted_name, model_name_info_, boost::is_any_of("@"), boost::token_compress_on);
      std::string model_name;
      model_type mtype = MODEL_UNDEFINED;
      if (splitted_name.size() < 1 || splitted_name.size() > 2) {
        return false;
      }
      if (splitted_name.size() > 0) {
        model_name = splitted_name[0];
        boost::trim(model_name);
        mtype = MODEL_DEVICE; // Default
      }
      if (splitted_name.size() > 1) {
        std::string model_type_label = splitted_name[1];
        boost::trim(model_type_label);
        mtype = get_model_type_from_label(model_type_label);
      }
      if (mtype == MODEL_UNDEFINED) {
        model_name_.clear();
        return false;
      }
      mtype_ = mtype;
      model_name_ = model_name;
      return true;
    }

    void model_entry::set_model_type(model_type t_)
    {
      _model_type_ = t_;
      return;
    }

    model_type model_entry::get_model_type() const
    {
      return _model_type_;
    }

    bool model_entry::is_device_model() const
    {
      return _model_type_ == MODEL_DEVICE;
    }

    bool model_entry::is_port_model() const
    {
      return _model_type_ == MODEL_PORT;
    }

    const datatools::properties & model_entry::get_config() const
    {
      return _config_;
    }

    datatools::properties & model_entry::grab_config()
    {
      DT_THROW_IF(is_created(),
                  std::logic_error,
                  "Cannot modify the configuration of model named '" << get_name() << "' !");
      return _config_;
    }

    void model_entry::set_config(const datatools::properties & sc_)
    {
      DT_THROW_IF(is_created(),
                  std::logic_error,
                  "Cannot modify the configuration of model named '" << get_name() << "' !");
      _config_ = sc_;
      return;
    }

    const std::string & model_entry::get_model_id() const
    {
      return _model_id_;
    }

    void model_entry::set_model_id(const std::string & mid_)
    {
      DT_THROW_IF(mid_.empty(), std::logic_error, "Empty model ID is not allowed !");
      _model_id_ = mid_;
      return;
    }

    const std::string & model_entry::get_name() const
    {
      return _name_;
    }

    void model_entry::set_name(const std::string & sn_)
    {
      DT_THROW_IF(sn_.empty(),std::logic_error, "Empty model name is not allowed !");
      _name_ = sn_;
      return;
    }

    const device_model_handle_type & model_entry::get_device_model_handle() const
    {
      return const_cast<model_entry *>(this)->_grab_device_model_handle();
    }

    device_model_handle_type & model_entry::grab_device_model_handle()
    {
      return _grab_device_model_handle();
    }

    device_model_handle_type & model_entry::grab_device_model_handle_plain()
    {
      DT_THROW_IF(!is_device_model(), std::logic_error,
                  "Model entry '" << _name_ << "' is not a device model!");
      return _device_model_handle_;
    }

    const port_model_handle_type & model_entry::get_port_model_handle() const
    {
      return const_cast<model_entry *>(this)->_grab_port_model_handle();
    }

    port_model_handle_type & model_entry::grab_port_model_handle()
    {
      return _grab_port_model_handle();
    }

    port_model_handle_type & model_entry::grab_port_model_handle_plain()
    {
      DT_THROW_IF(!is_port_model(), std::logic_error,
                  "Model entry '" << _name_ << "' is not a port model!");
      return _port_model_handle_;
    }

    device_model_handle_type & model_entry::_grab_device_model_handle()
    {
      DT_THROW_IF(!is_device_model(), std::logic_error,
                  "Model entry '" << _name_ << "' is not a device model!");
      if (! _device_model_handle_) {
        DT_THROW_IF(_device_manager_ == 0, std::logic_error,
                    "No device manager is available to create the device model '"
                    << this->_name_ << "' ! ");
        _device_manager_->_create_device_model(*this);
      }
      if (!is_initialized()) {
        DT_THROW_IF(_device_manager_ == 0,std::logic_error,
                    "No device manager is available to initialize the device model '"
                    << this->_name_ << "' ! ");
        _device_manager_->_initialize_device_model(*this);
      }
      return _device_model_handle_;
    }

    port_model_handle_type & model_entry::_grab_port_model_handle()
    {
      DT_THROW_IF(!is_port_model(), std::logic_error,
                  "Model entry '" << _name_ << "' is not a port model!");
      if (! _port_model_handle_) {
        DT_THROW_IF(_device_manager_ == 0, std::logic_error,
                    "No device manager is available to create the port model '"
                    << this->_name_ << "' ! ");
        _device_manager_->_create_port_model(*this);
      }
      if (!is_initialized()) {
        DT_THROW_IF(_device_manager_ == 0,std::logic_error,
                    "No device manager is available to initialize the port model '"
                    << this->_name_ << "' ! ");
        _device_manager_->_initialize_port_model(*this);
      }
      return _port_model_handle_;
    }

    bool model_entry::has_device_manager() const
    {
      return _device_manager_ != 0;
    }

    // Install a handle to the device manager
    void model_entry::set_device_manager(manager & dev_mgr_)
    {
      _device_manager_ = &dev_mgr_;
      return;
    }

    // Reset the handle to the device manager
    void model_entry::reset_device_manager()
    {
      _device_manager_ = 0;
      return;
    }

    model_entry::model_entry() {
      _device_manager_ = 0;
      _model_type_ = MODEL_UNDEFINED;
      return;
    }

    model_entry::~model_entry() {
      if (_device_model_handle_) {
        _device_model_handle_.reset();
      }
      if (_port_model_handle_) {
        _port_model_handle_.reset();
      }
      return;
    }

    bool model_entry::is_created() const
    {
      if (is_device_model()) {
        return _device_model_handle_.has_data();
      } else if (is_port_model()) {
        return _port_model_handle_.has_data();
      }
      return false;
    }

    bool model_entry::is_initialized() const
    {
      if (!is_created()) {
        return false;
      }
      if (is_device_model()) {
        return _device_model_handle_.get().is_initialized();
      } else if (is_port_model()) {
        return _port_model_handle_.get().is_initialized();
      }
      return false;
    }

    void model_entry::tree_dump(std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_,
                                bool inherit_) const
    {
      if (!title_.empty()) out_ << indent_ << title_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Name : '"
           << _name_
           << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Type of model : '"
           << get_label_from_model_type(_model_type_)
           << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Model class ID : '"
           << _model_id_
           << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Configuration : "
           << _config_.size()
           << " parameter" << (_config_.size() > 1 ? "s" : "")
           << std::endl;
      {
        std::ostringstream indent2ss;
        indent2ss << indent_ << i_tree_dumpable::skip_tag;
        _config_.tree_dump(out_, "", indent2ss.str());
      }
      out_ << indent_ << i_tree_dumpable::tag
           << "Status : ";
      if (is_created()) {
        out_ << "created";
        if (is_initialized()) {
          out_ << " and initialized";
        }
      } else {
        out_ << "not created";
      }
      out_ << std::endl;

      if (is_device_model()) {
        out_ << indent_ << i_tree_dumpable::tag
             << "Device model handle : ";
        if (_device_model_handle_.has_data()) {
          out_ << "[@" << &_device_model_handle_.get() << ']';
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      if (is_port_model()) {
        out_ << indent_ << i_tree_dumpable::tag
             << "Port model handle : ";
        if (_port_model_handle_.has_data()) {
          out_ << "[@" << &_port_model_handle_.get() << ']';
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Device manager : ";
      if (_device_manager_) {
        out_ << "[@" <<  _device_manager_ << ']';
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

    model_repository::model_repository()
    {
      _models_ = 0;
      return;
    }

    bool model_repository::has_models() const
    {
      return _models_ != 0;
    }

    void model_repository::set_models(model_pool_type & models_)
    {
      _models_ = &models_;
      return;
    }

    const model_pool_type & model_repository::get_models() const
    {
      DT_THROW_IF(! has_models(), std::logic_error, "No dictionary of models!");
      return *_models_;
    }

    model_pool_type & model_repository::grab_models()
    {
      DT_THROW_IF(! has_models(), std::logic_error, "No dictionary of models!");
      return *_models_;
    }

    bool model_repository::has_device_model(const std::string & model_name_) const
    {
      const model_pool_type & pool = get_models();
      std::string model_key;
      encode_model_key(model_key, MODEL_DEVICE, model_name_);
      model_pool_type::const_iterator found = pool.find(model_key);
      if (found == pool.end()) {
        return false;
      }
      const model_entry & me = found->second;
      if (!me.is_device_model()) {
        return false;
      }
      return true;
    }

    bool model_repository::has_port_model(const std::string & model_name_) const
    {
      const model_pool_type & pool = get_models();
      std::string model_key;
      encode_model_key(model_key, MODEL_PORT, model_name_);
      model_pool_type::const_iterator found = pool.find(model_key);
      if (found == pool.end()) {
        return false;
      }
      const model_entry & me = found->second;
      if (!me.is_port_model()) {
        return false;
      }
      return true;
    }

    const device_model_handle_type &
    model_repository::get_device_model_handle(const std::string & model_name_) const
    {
      const model_pool_type & pool = get_models();
      std::string model_key;
      encode_model_key(model_key, MODEL_DEVICE, model_name_);
      model_pool_type::const_iterator found = pool.find(model_key);
      DT_THROW_IF(found == pool.end(), std::logic_error,
                  "No model named '" << model_name_ << "'!");
      const model_entry & me = found->second;
      DT_THROW_IF(!me.is_device_model(), std::logic_error,
                  "No device model named '" << model_name_ << "'!");
      return me.get_device_model_handle();
    }

    const base_device_model &
    model_repository::get_device_model(const std::string & model_name_) const
    {
      return get_device_model_handle(model_name_).get();
    }

    const port_model_handle_type &
    model_repository::get_port_model_handle(const std::string & model_name_) const
    {
      const model_pool_type & pool = get_models();
      std::string model_key;
      encode_model_key(model_key, MODEL_PORT, model_name_);
      model_pool_type::const_iterator found = pool.find(model_key);
      DT_THROW_IF(found == pool.end(), std::logic_error,
                  "No model named '" << model_name_ << "'!");
      const model_entry & me = found->second;
      DT_THROW_IF(!me.is_port_model(), std::logic_error,
                  "No port model named '" << model_name_ << "'!");
      return me.get_port_model_handle();
    }

    const base_port_model &
    model_repository::get_port_model(const std::string & model_name_) const
    {
      return get_port_model_handle(model_name_).get();
    }

  } // end of namespace device

} // end of namespace vire
