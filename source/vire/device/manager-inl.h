//! \file  vire/device/manager-inl.h
//! \brief Vire device manager template implementation
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

#ifndef VIRE_DEVICE_MANAGER_INL_H
#define VIRE_DEVICE_MANAGER_INL_H

namespace vire {

  namespace device {

    template <class T>
    bool manager::is_device_model_a(const std::string & name_) const
    {
      std::string model_key;
      encode_model_key(model_key, MODEL_DEVICE, name_);
      model_pool_type::const_iterator found = _models_.find(model_key);
      DT_THROW_IF (found == _models_.end(),
                   std::logic_error,
                   "No device named '" << name_ << "' !");
      const model_entry & entry = found->second;
      if (!entry.is_initialized()) {
        manager * mutable_this = const_cast<manager *>(this);
        model_entry & mutable_entry = const_cast<model_entry &>(entry);
        mutable_this->_initialize_device_model(mutable_entry);
      }
      const std::type_info & ti = typeid(T);
      const std::type_info & tf = typeid(found->second.get_device_model_handle().get());
      return (ti == tf);
    }

    template <class T>
    bool manager::is_port_model_a(const std::string & name_) const
    {
      std::string model_key;
      encode_model_key(model_key, MODEL_PORT, name_);
      model_pool_type::const_iterator found = _models_.find(model_key);
      DT_THROW_IF (found == _models_.end(),
                   std::logic_error,
                   "No port named '" << name_ << "' !");
      const model_entry & entry = found->second;
      if (!entry.is_initialized()) {
        manager * mutable_this = const_cast<manager *>(this);
        model_entry & mutable_entry = const_cast<model_entry &>(entry);
        mutable_this->_initialize_port_model(mutable_entry);
      }
      const std::type_info & ti = typeid(T);
      const std::type_info & tf = typeid(found->second.get_port_model_handle().get());
      return (ti == tf);
    }

    template<class T>
    T & manager::grab_device_model_as(const std::string & name_)
    {
      return dynamic_cast<T&>(grab_device_model(name_));
    }

    template<class T>
    T & manager::grab_port_model_as(const std::string & name_)
    {
      return dynamic_cast<T&>(grab_port_model(name_));
    }

    template<class T>
    const T & manager::get_device_model_as(const std::string & name_) const
    {
      return dynamic_cast<const T&>(get_device_model(name_));
    }

    template<class T>
    const T & manager::get_port_model_as(const std::string & name_) const
    {
      return dynamic_cast<const T&>(get_port_model(name_));
    }

    template <class DeviceClass>
    void manager::register_device_type(const std::string & id_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Device manager is not initialized!");
      _grab_device_factory_register().registration(id_, boost::factory<DeviceClass*>());
      return;
    }

    template <class PortClass>
    void manager::register_port_type(const std::string & id_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Port manager is not initialized!");
      _grab_port_factory_register().registration(id_, boost::factory<PortClass*>());
      return;
    }

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_MANAGER_INL_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
