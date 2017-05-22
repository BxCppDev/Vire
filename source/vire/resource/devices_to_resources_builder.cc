// vire/resource/devices_to_resources_builder.cc
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

// Ourselves:
#include <vire/resource/devices_to_resources_builder.h>

// Third party:
// - Boost:
#include <boost/algorithm/string/replace.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/logger.h>

// This project:
#include <vire/resource/resource.h>
#include <vire/resource/role.h>
#include <vire/resource/manager.h>
#include <vire/resource/datapoint_resource_instance.h>
#include <vire/resource/method_resource_instance.h>
#include <vire/device/manager.h>
#include <vire/device/mapping.h>
#include <vire/device/logical_device.h>
#include <vire/device/logical_port.h>
#include <vire/device/physical_port.h>
#include <vire/device/base_device_model.h>
#include <vire/device/base_port_model.h>
#include <vire/device/mapping.h>
#include <vire/utility/path.h>

namespace vire {

  namespace resource {

    devices_to_resources_builder::devices_to_resources_builder()
      : base_resource_builder()
    {
      _initialized_      = false;
      _device_manager_   = 0;
      _device_mapping_   = 0;
      return;
    }

    devices_to_resources_builder::~devices_to_resources_builder()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool devices_to_resources_builder::is_initialized() const
    {
      return _initialized_;
    }

    void devices_to_resources_builder::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Builder is already initialized!");
      DT_THROW_IF(!has_device_manager(), std::logic_error, "Missing device manager!");
      _initialized_ = true;
      return;
    }

    void devices_to_resources_builder::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Builder is not initialized!");
      _initialized_ = false;
      _device_mapping_   = 0;
      _device_manager_   = 0;
      return;
    }

    bool devices_to_resources_builder::has_device_manager() const
    {
      return _device_manager_ != 0;
    }

    void devices_to_resources_builder::set_device_manager(const ::vire::device::manager & device_manager_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Builder is already initialized!");
      DT_THROW_IF(!device_manager_.is_initialized(), std::logic_error,
                  "Device manager is not initialized!");
      DT_THROW_IF(!device_manager_.is_mapping_requested(), std::logic_error,
                  "Device manager has no mapping requested!");
      DT_THROW_IF(!device_manager_.get_mapping().is_initialized(), std::logic_error,
                  "Device mapping is not initialized!");
      _device_manager_ = &device_manager_;
      _device_mapping_ = &device_manager_.get_mapping();
      return;
    }

    const ::vire::device::manager & devices_to_resources_builder::get_device_manager() const
    {
      DT_THROW_IF(!has_device_manager(), std::logic_error,
                  "Builder has no device manager!");
      return *_device_manager_;
    }

    void devices_to_resources_builder::build_resources()
    {
      DT_LOG_TRACE_ENTERING(datatools::logger::PRIO_TRACE);
      const ::vire::device::manager & device_manager = *_device_manager_;
      const ::vire::device::mapping & device_mapping = *_device_mapping_;
      ::vire::resource::manager & resource_manager   = _grab_resource_manager();

      std::set<std::string> dev_paths;
      device_mapping.build_paths(dev_paths, 0);
      for (std::set<std::string>::const_iterator ipath = dev_paths.begin();
           ipath != dev_paths.end();
           ipath++) {
        const std::string & the_path = *ipath;
        const vire::device::mapping_info & minfo = device_mapping.get_mapping_info(the_path);
        if (minfo.is_device()) {
          std::cerr << "*** DEVEL *** devices_to_resources_builder::build_resources: "
                    << "Mapping info: '" << the_path << "' (device)"
                    << std::endl;
          if (minfo.has_logical_device()) {
            const vire::device::logical_device & log_dev = minfo.get_logical_device();
            if (log_dev.has_model()) {
              const vire::device::base_device_model & dev_model = log_dev.get_model();
              const std::type_info & tdev   = typeid(dev_model);
              const std::type_info & tmodel = typeid(vire::device::base_datapoint_model);
              if (tdev == tmodel) {

                // We found a datapoint:
                const vire::device::base_datapoint_model & dp_model =
                  dynamic_cast<const vire::device::base_datapoint_model &>(dev_model);

                // Check for the read method port:
                bool dp_read_method = false;
                if (dp_model.has_port(vire::device::base_datapoint_model::read_method_name())) {
                  dp_read_method = true;
                }
                std::cerr << "*** DEVEL *** devices_to_resources_builder::build_resources: "
                          << "base_datapoint_model: dp_read_method=" << dp_read_method << ""
                          << std::endl;
                if (dp_read_method) {
                  const vire::device::physical_port & dp_read_method_port
                    = dev_model.get_port(vire::device::base_datapoint_model::read_method_name());
                  const vire::device::logical_port & dp_read_method_port_log = dp_read_method_port.get_logical();
                  const vire::device::base_method_port_model & dp_read_method_port_model =
                    dynamic_cast<const vire::device::base_method_port_model &>(dp_read_method_port_log.get_model());
                  // We create a new resource:
                  resource dp_read_resource;
                  dp_read_resource.set_id(resource_manager.candidate_resource_id());
                  std::string dp_read_path;
                  vire::utility::path::build_child(the_path, vire::device::base_datapoint_model::read_method_name(), dp_read_path);
                  dp_read_resource.set_path(dp_read_path);
                  dp_read_resource.set_access(vire::utility::RW_READABLE);
                  std::string dp_read_name = dp_read_path;
                  vire::utility::path::to_address(dp_read_path, dp_read_name);
                  // boost::algorithm::replace_first(dp_read_name, vire::utility::path::setup_separator(), vire::utility::path::address_separator());
                  // boost::algorithm::replace_all(dp_read_name, vire::utility::path::path_separator(), vire::utility::path::address_separator());
                  dp_read_resource.set_name(dp_read_name);
                  method_resource_instance * dp_read_method_res_inst_ptr = new method_resource_instance;
                  boost::shared_ptr<base_resource_instance> sh_ri(dp_read_method_res_inst_ptr);
                  dp_read_method_res_inst_ptr->set_model(dp_read_method_port_model);
                  dp_read_method_res_inst_ptr->initialize_simple();
                  dp_read_resource.set_resource_instance(sh_ri);
                  dp_read_resource.initialize_simple();
                  resource_manager.add_resource(dp_read_resource);
                }

                // Check for the write method port:
                bool dp_write_method = false;
                if (dp_model.has_port(vire::device::base_datapoint_model::write_method_name())) {
                  dp_write_method = true;
                }
                std::cerr << "*** DEVEL *** devices_to_resources_builder::build_resources: "
                          << "base_datapoint_model: dp_write_method=" << dp_write_method << ""
                          << std::endl;
                if (dp_write_method) {
                  const vire::device::physical_port & dp_write_method_port
                    = dev_model.get_port(vire::device::base_datapoint_model::write_method_name());
                  const vire::device::logical_port & dp_write_method_port_log = dp_write_method_port.get_logical();
                  const vire::device::base_method_port_model & dp_write_method_port_model =
                    dynamic_cast<const vire::device::base_method_port_model &>(dp_write_method_port_log.get_model());
                  // We create a new resource:
                  resource dp_write_resource;
                  dp_write_resource.set_id(resource_manager.candidate_resource_id());
                  std::string dp_write_path;
                  // = the_path + '/' + vire::device::base_datapoint_model::write_method_name();
                  vire::utility::path::build_child(the_path, vire::device::base_datapoint_model::write_method_name(), dp_write_path);
                  dp_write_resource.set_path(dp_write_path);
                  dp_write_resource.set_access(vire::utility::RW_WRITABLE);
                  std::string dp_write_name;
                  // = dp_write_path;
                  vire::utility::path::to_address(dp_write_path, dp_write_name);
                  // boost::algorithm::replace_first(dp_write_name, vire::device::mapping::setup_path_sep(), ".");
                  // boost::algorithm::replace_all(dp_write_name, "/", ".");
                  dp_write_resource.set_name(dp_write_name);
                  method_resource_instance * dp_write_method_res_inst_ptr = new method_resource_instance;
                  boost::shared_ptr<base_resource_instance> sh_ri(dp_write_method_res_inst_ptr);
                  dp_write_method_res_inst_ptr->set_model(dp_write_method_port_model);
                  dp_write_method_res_inst_ptr->initialize_simple();
                  dp_write_resource.set_resource_instance(sh_ri);
                  dp_write_resource.initialize_simple();
                  resource_manager.add_resource(dp_write_resource);
                }
              }
            }
          }
        } else if (minfo.is_port()) {
          std::cerr << "*** DEVEL *** devices_to_resources_builder::build_resources: "
                    << "Mapping info: '" << the_path << "' (port)"
                    << std::endl;
          if (minfo.has_logical_port()) {
            const vire::device::logical_port & log_port = minfo.get_logical_port();
            if (log_port.has_model()) {
              const vire::device::base_port_model & port_model = log_port.get_model();
              const std::type_info & tport = typeid(port_model);
              const std::type_info & tmodel = typeid(vire::device::base_method_port_model);
              if (tport == tmodel) {
                const vire::device::base_method_port_model & meth_model =
                  dynamic_cast<const vire::device::base_method_port_model &>(port_model);
                std::cerr << "*** DEVEL *** devices_to_resources_builder::build_resources: "
                          << "Method Port Model: '" << port_model.get_name() << "'"
                          << std::endl;
               // We found a method:
                // We create a new resource:
                resource meth_resource;
                meth_resource.set_id(resource_manager.candidate_resource_id());
                meth_resource.set_path(the_path);
                vire::utility::rw_access_type rw_access = vire::utility::RW_INVALID;
                // Default R/W access for a method:
                rw_access = vire::utility::RW_WRITABLE;
                const datatools::introspection::method & the_method = meth_model.get_method();
                if (the_method.has_constness()) {
                  if (the_method.is_constness()) {
                    rw_access = vire::utility::RW_READABLE;
                  }
                }
                meth_resource.set_access(rw_access);
                std::string meth_name = the_path;
                DT_THROW_IF(vire::utility::path::to_address(the_path, meth_name), std::logic_error,
                            "Invalid conversion for method path '" << the_path << "'!");
                meth_resource.set_name(meth_name);
                method_resource_instance * meth_res_inst_ptr
                  = new method_resource_instance;
                meth_res_inst_ptr->set_model(meth_model);
                meth_res_inst_ptr->initialize_simple();
                meth_resource.set_resource_instance(meth_res_inst_ptr);
                meth_resource.initialize_simple();
                resource_manager.add_resource(meth_resource);
                std::cerr << "*** DEVEL *** "
                          << "Method Port Resource: '" << meth_name << "'"
                          << std::endl;
              }
            }
          }
        }
      }

      // Add (hidden) dependency support...
      // Add local role support (expert/tag)...

      DT_LOG_TRACE_EXITING(datatools::logger::PRIO_TRACE);
      return;
    }

  } // namespace resource

} // namespace vire
