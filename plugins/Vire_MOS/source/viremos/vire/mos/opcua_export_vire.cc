// opcua_export_vire.cc
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
#include <vire/mos/opcua_export_vire.h>

// Standard Library:
#include <iostream>
#include <sstream>
#include <fstream>

// Third Party:
// - Boost:
#include <boost/algorithm/string.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/multi_properties.h>
#include <datatools/i_tree_dump.h>
#include <datatools/introspection/access_type.h>
#include <datatools/introspection/data_type.h>
#include <datatools/introspection/data_layout.h>
#include <datatools/introspection/method.h>
#include <datatools/introspection/argument.h>
#include <datatools/introspection/data_description.h>
#include <datatools/introspection/unit_support.h>

// This project:
#include <vire/device/utils.h>
// #include <vire/mos/datapoint_interface.h>

namespace vire {

  namespace mos {

    // static
    const std::string & opcua_export_vire::get_models_definition_filename()
    {
      static const std::string _filename("models.vdm");
      return _filename;
    }

    // static
    const std::string & opcua_export_vire::get_model_name_suffix()
    {
      static const std::string _suffix(".model");
      return _suffix;
    }

    void opcua_export_vire::_set_defaults()
    {
      _logging_ = datatools::logger::PRIO_FATAL;
      _export_only_public_components_ = true;
      _datapoint_default_interface_ = true;
      return;
    }

    // static
    datatools::introspection::data_type opcua_export_vire::translate_type(Type type_)
    {
      datatools::introspection::data_type the_type
        = datatools::introspection::DATA_TYPE_INVALID;
      switch (type_) {
      case TYPE_BOOL   : the_type = datatools::introspection::DATA_TYPE_BOOLEAN; break;
      case TYPE_INT8   : the_type = datatools::introspection::DATA_TYPE_INT8; break;
      case TYPE_INT16  : the_type = datatools::introspection::DATA_TYPE_INT16; break;
      case TYPE_INT32  : the_type = datatools::introspection::DATA_TYPE_INT32; break;
      case TYPE_INT64  : the_type = datatools::introspection::DATA_TYPE_INT64; break;
      case TYPE_FLOAT  : the_type = datatools::introspection::DATA_TYPE_FLOAT; break;
      case TYPE_DOUBLE : the_type = datatools::introspection::DATA_TYPE_DOUBLE; break;
      case TYPE_STRING : the_type = datatools::introspection::DATA_TYPE_STRING; break;
      }
      return the_type;
    }

    // static
    vire::utility::rw_access_type
    opcua_export_vire::translate_rw_access(const has_info_interface & with_info_, bool strict_)
    {
      vire::utility::rw_access_type rw_access = vire::utility::RW_READABLE;
      if (!(with_info_.get_info() == boost::none)) {
        const Info & info = with_info_.get_info().get();
        if (info.config != boost::none) {
          if (info.config.get()) {
            if (strict_) {
              rw_access = vire::utility::RW_WRITABLE;
            } else {
              rw_access = vire::utility::RW_READABLE_WRITABLE;
            }
          }
        }
      }
      return rw_access;
    }

    // static
    void opcua_export_vire::translate_data_description(const Argument & argument_,
                                                       datatools::introspection::data_description & dd_)
    {
      dd_.reset();

      // Type:
      datatools::introspection::data_type the_type = datatools::introspection::DATA_TYPE_INVALID;
      DT_THROW_IF(argument_.get_type() == TYPE_INVALID, std::logic_error, "Argument has no type!");
      the_type = translate_type(argument_.get_type());
      dd_.set_type(the_type);

      // Layout:
      datatools::introspection::data_layout arg_layout = datatools::introspection::DATA_LAYOUT_INVALID;
      arg_layout = datatools::introspection::DATA_LAYOUT_SCALAR;
      dd_.set_layout(arg_layout);

      // Special unit support:
      bool with_unit = false;
      std::string the_implicit_unit_symbol;
      datatools::introspection::unit_info uinfo;
      if (argument_.get_unit()) {
        the_implicit_unit_symbol = argument_.get_unit().get().unit;
        uinfo.set_unit_support(datatools::introspection::UNIT_SUPPORT_IMPLICIT_UNIT);
        uinfo.set_implicit_unit_symbol(the_implicit_unit_symbol);
        with_unit = true;
      }

      if (with_unit) {
        dd_.set_unit_info(uinfo);
      }

      return;
    }

    // static
    void opcua_export_vire::translate_data_description(const SimpleDatapoint & sdatapoint_,
                                                       datatools::introspection::data_description & dd_)
    {
      dd_.reset();

      // Type:
      datatools::introspection::data_type the_type = datatools::introspection::DATA_TYPE_INVALID;
      DT_THROW_IF(sdatapoint_.get_type() == TYPE_INVALID, std::logic_error, "Datapoint has no type!");
      the_type = translate_type(sdatapoint_.get_type());
      dd_.set_type(the_type);

      // Layout:
      datatools::introspection::data_layout dp_layout = datatools::introspection::DATA_LAYOUT_INVALID;
      dp_layout = datatools::introspection::DATA_LAYOUT_SCALAR;
      std::size_t dp_array_size = 0;
      if (sdatapoint_.array_size) {
        dp_array_size = sdatapoint_.array_size.get();
        if (dp_array_size != 0) {
          DT_THROW_IF(dp_array_size < 1, std::logic_error,
                      "Invalid array size for datapoint '" << sdatapoint_.name << "'!");
          dp_layout = datatools::introspection::DATA_LAYOUT_VECTOR_WITH_FIXED_SIZE;
        }
      }
      dd_.set_layout(dp_layout);
      if (datatools::introspection::is_vector(dp_layout)) {
        dd_.set_vector_fixed_size(dp_array_size);
      }

      // Special unit support:
      bool with_unit = false;
      std::string the_implicit_unit_symbol;
      datatools::introspection::unit_info uinfo;
      if (sdatapoint_.get_unit()) {
        the_implicit_unit_symbol = sdatapoint_.get_unit().get().unit;
        uinfo.set_unit_support(datatools::introspection::UNIT_SUPPORT_IMPLICIT_UNIT);
        uinfo.set_implicit_unit_symbol(the_implicit_unit_symbol);
        with_unit = true;
      }

      if (with_unit) {
        dd_.set_unit_info(uinfo);
      }
      return;
    }

    opcua_export_vire::opcua_export_vire()
    {
      _set_defaults();
      return;
    }

    opcua_export_vire::~opcua_export_vire()
    {
      return;
    }

    void opcua_export_vire::set_logging(datatools::logger::priority prio_)
    {
      _logging_ = prio_;
      return;
    }

    datatools::logger::priority opcua_export_vire::get_logging() const
    {
      return _logging_;
    }

    bool opcua_export_vire::is_export_only_public_components() const
    {
      return _export_only_public_components_;
    }

    void opcua_export_vire::set_export_only_public_components(bool x_)
    {
      _export_only_public_components_ = x_;
      return;
    }

    bool opcua_export_vire::is_datapoint_default_interface() const
    {
      return _datapoint_default_interface_;
    }

    void opcua_export_vire::set_datapoint_default_interface(bool ddi_)
    {
      _datapoint_default_interface_ = ddi_;
      return;
    }

    const std::string & opcua_export_vire::get_export_path() const
    {
      return _export_path_;
    }

    void opcua_export_vire::set_export_path(const std::string & xp_)
    {
      _export_path_ = xp_;
      return;
    }

    bool opcua_export_vire::has_model_name() const
    {
      return !_model_name_.empty();
    }

    const std::string & opcua_export_vire::get_model_name() const
    {
      return _model_name_;
    }

    void opcua_export_vire::set_model_name(const std::string & name_)
    {
      _model_name_ = name_;
      return;
    }

    void opcua_export_vire::process(const vire::mos::OPCUA & server_)
    {
      DT_THROW_IF(_export_path_.empty(), std::logic_error, "Missing export path!");

      boost::filesystem::path base_dir = _export_path_;
      _process_server("", base_dir, server_);

      return;
    }

    void opcua_export_vire::_process_compound_datapoint(const std::string & base_name_,
                                                        const boost::filesystem::path & base_dir_,
                                                        const CompoundDatapoint & cdatapoint_)
    {
      boost::filesystem::path base_dir = base_dir_;
      std::string datapoint_model_base_name = cdatapoint_.name;
      base_dir /= datapoint_model_base_name;
      std::string datapoint_model_name = datapoint_model_base_name;
      if (! base_name_.empty()) {
        datapoint_model_name = base_name_ + '.' + datapoint_model_base_name;
      }
      boost::filesystem::create_directory(base_dir);
      boost::filesystem::path datapoint_model_file_path = base_dir / get_models_definition_filename();
      device_entry_type cdp_desc;
      cdp_desc.name = datapoint_model_name + get_model_name_suffix();
      cdp_desc.model_type = "device";
      cdp_desc.class_id = "vire::device::base_device_model";
      cdp_desc.config.set_description("Device model configuration parameters");
      cdp_desc.config.store_string("aux.vire.mos.device.flavor",
                                   "vire::mos::CompoundDatapoint",
                                   "OPCUA MOS compound datapoint");

      // Fetch special userinfos:
      _scan_device_userinfos(cdatapoint_, cdp_desc);

      // Fetch special attributes:
      _scan_device_attributes(cdatapoint_, cdp_desc);

      // Scan embedded datapoints:
      _scan_datapoints(cdatapoint_,
                       datapoint_model_name,
                       base_dir,
                       cdp_desc.config);

      if (_logging_ >= datatools::logger::PRIO_DEBUG) {
        cdp_desc.tree_dump(std::clog, "Compound datapoint '" + datapoint_model_name + "' :", "[debug]: ");
      }

      // Export models:
      datatools::multi_properties datapoint_def("name", "type");

      // Populate with the device model:
      datapoint_def.add(cdp_desc.name + "@" + cdp_desc.model_type,
                        cdp_desc.class_id,
                        cdp_desc.config);
      datapoint_def.write(datapoint_model_file_path.string());

      return;
    }

    void opcua_export_vire::_process_simple_datapoint(const std::string & base_name_,
                                                      const boost::filesystem::path & base_dir_,
                                                      const SimpleDatapoint & sdatapoint_)
    {
      boost::filesystem::path base_dir = base_dir_;
      std::string datapoint_model_base_name = sdatapoint_.name;
      base_dir /= datapoint_model_base_name;
      std::string datapoint_model_name = datapoint_model_base_name;
      if (! base_name_.empty()) {
        datapoint_model_name = base_name_ + '.' + datapoint_model_base_name;
      }
      boost::filesystem::create_directory(base_dir);
      boost::filesystem::path datapoint_model_file_path = base_dir / get_models_definition_filename();
      device_entry_type sdp_desc;
      sdp_desc.name = datapoint_model_name + get_model_name_suffix();
      sdp_desc.model_type = "device";
      sdp_desc.class_id = "vire::device::base_datapoint_model";
      sdp_desc.config.set_description("Device model configuration parameters");
      sdp_desc.config.store_string("aux.vire.mos.device.flavor",
                                   "vire::mos::SimpleDatapoint",
                                   "OPCUA MOS simple datapoint");

      // R/W access:
      vire::utility::rw_access_type dp_rw_access = translate_rw_access(sdatapoint_);
      sdp_desc.config.store_string("rw_access",
                                   vire::utility::to_string(dp_rw_access),
                                   "Datapoint R/W access mode");

      // Data description:
      datatools::introspection::data_description dp_dd;
      translate_data_description(sdatapoint_, dp_dd);
      dp_dd.export_to_config(sdp_desc.config,
                             datatools::introspection::data_description::DD_XC_DEFAULT,
                             "data.");

      // Fetch special userinfos:
      _scan_device_userinfos(sdatapoint_, sdp_desc);

      // Fetch special attributes:
      _scan_device_attributes(sdatapoint_, sdp_desc);

      // Scan embedded methods:
      std::vector<device_entry_type> method_port_model_descriptions;
      _scan_methods(sdatapoint_,
                    datapoint_model_name,
                    base_dir,
                    sdp_desc.config,
                    method_port_model_descriptions);

      if (_logging_ >= datatools::logger::PRIO_DEBUG) {
        sdp_desc.tree_dump(std::clog, "Simple datapoint '" + datapoint_model_name + "' :", "[debug]: ");
      }

      // if (is_datapoint_default_interface()) {
      //   // Equip the datapoint with a standard interface of methods:
      //   datapoint_interface_builder dp_builder;
      //   dp_builder.set_dp_rw_access(dp_rw_access);
      //   dp_builder.set_dp_dd(dp_dd);
      //   dp_builder.build_interface(method_port_model_descriptions);
      // }

      // Export models:
      datatools::multi_properties datapoint_def("name", "type");

      // Populate with the resource/method port models:
      for (const auto & method_port_model_desc : method_port_model_descriptions) {
        datapoint_def.add(method_port_model_desc.name + "@" + method_port_model_desc.model_type,
                          method_port_model_desc.class_id,
                          method_port_model_desc.config);
      }

      // Populate with the (simple datapoint) device model:
      datapoint_def.add(sdp_desc.name + "@" + sdp_desc.model_type,
                        sdp_desc.class_id,
                        sdp_desc.config);
      datapoint_def.write(datapoint_model_file_path.string());

      return;
    }

    void opcua_export_vire::_process_simple_device(const std::string & base_name_,
                                                   const boost::filesystem::path & base_dir_,
                                                   const SimpleDevice & sdevice_)
    {
      boost::filesystem::path base_dir = base_dir_;
      std::string device_model_base_name = sdevice_.name;
      base_dir /= device_model_base_name;
      std::string device_model_name = device_model_base_name;
      if (! base_name_.empty()) {
        device_model_name = base_name_ + '.' + device_model_base_name;
      }
      boost::filesystem::create_directory(base_dir);
      boost::filesystem::path device_model_file_path = base_dir / get_models_definition_filename();
      device_entry_type sdev_desc;
      sdev_desc.name = device_model_name + get_model_name_suffix();
      sdev_desc.model_type = "device";
      sdev_desc.class_id = "vire::device::base_device_model";
      sdev_desc.config.set_description("Device model configuration parameters");
      sdev_desc.config.store_string("aux.vire.mos.device.flavor",
                                    "vire::mos::SimpleDevice",
                                    "OPCUA MOS simple device");

      // Fetch special userinfos:
      _scan_device_userinfos(sdevice_, sdev_desc);

      // Fetch special attributes:
      _scan_device_attributes(sdevice_, sdev_desc);

      // Scan embedded datapoints:
      std::vector<device_entry_type> embedded_datapoints_model_descriptions;
      _scan_datapoints(sdevice_,
                       device_model_name,
                       base_dir,
                       sdev_desc.config);

      // Scan embedded methods:
      std::vector<device_entry_type> method_port_model_descriptions;
      _scan_methods(sdevice_,
                    device_model_name,
                    base_dir,
                    sdev_desc.config,
                    method_port_model_descriptions);

      if (_logging_ >= datatools::logger::PRIO_DEBUG) {
        sdev_desc.tree_dump(std::clog, "Simple device '" + device_model_name + "' :", "[debug]: ");
      }

      // Export models:
      datatools::multi_properties device_def("name", "type");

      // Populate with the resource/method port models:
      for (const auto & method_port_model_desc : method_port_model_descriptions) {
        device_def.add(method_port_model_desc.name + "@" + method_port_model_desc.model_type,
                       method_port_model_desc.class_id,
                       method_port_model_desc.config);
      }

      // Populate with the (simple) device model:
      device_def.add(sdev_desc.name + "@" + sdev_desc.model_type,
                     sdev_desc.class_id,
                     sdev_desc.config);
      device_def.write(device_model_file_path.string());

      return;
    }

    void opcua_export_vire::_process_compound_device(const std::string & base_name_,
                                                     const boost::filesystem::path & base_dir_,
                                                     const CompoundDevice & cdevice_)
    {
      boost::filesystem::path base_dir = base_dir_;
      std::string device_model_base_name = cdevice_.name;
      base_dir /= device_model_base_name;
      std::string device_model_name = device_model_base_name;
      if (! base_name_.empty()) {
        device_model_name = base_name_ + '.' + device_model_base_name;
      }
      boost::filesystem::create_directory(base_dir);
      boost::filesystem::path device_model_file_path = base_dir / get_models_definition_filename();
      device_entry_type cdev_desc;
      cdev_desc.name = device_model_name + get_model_name_suffix();
      cdev_desc.model_type = "device";
      cdev_desc.class_id = "vire::device::base_device_model";
      cdev_desc.config.set_description("Device model configuration parameters");
      cdev_desc.config.store_string("aux.vire.mos.device.flavor",
                                    "vire::mos::CompoundDevice",
                                    "OPCUA MOS compound device");

      // Fetch special userinfos:
      _scan_device_userinfos(cdevice_, cdev_desc);

      // Fetch special attributes:
      _scan_device_attributes(cdevice_, cdev_desc);

      // Scan embedded devices:
      _scan_devices(cdevice_,
                    device_model_name,
                    base_dir,
                    cdev_desc.config);

      // Scan embedded datapoints:
      _scan_datapoints(cdevice_,
                       device_model_name,
                       base_dir,
                       cdev_desc.config);

      if (_logging_ >= datatools::logger::PRIO_DEBUG) {
        cdev_desc.tree_dump(std::clog, "Compound device '" + device_model_name + "' :", "[debug]: ");
      }

      // Export models:
      datatools::multi_properties device_def("name", "type");

      // Populate with the (compound) device model:
      device_def.add(cdev_desc.name + "@" + cdev_desc.model_type,
                     cdev_desc.class_id,
                     cdev_desc.config);
      device_def.write(device_model_file_path.string());

      return;
    }

    void opcua_export_vire::_process_server(const std::string & base_name_,
                                            const boost::filesystem::path & base_dir_,
                                            const OPCUA & server_)
    {
      boost::filesystem::path base_dir = base_dir_;
      std::string server_model_base_name = server_.name;
      if (has_model_name()) {
        server_model_base_name = get_model_name();
      }

      base_dir /= server_model_base_name;
      std::string server_model_name = server_model_base_name;
      if (! base_name_.empty()) {
        server_model_name = base_name_ + '.' + server_model_base_name;
      }
      boost::filesystem::create_directory(base_dir);
      boost::filesystem::path server_model_file_path = base_dir / get_models_definition_filename();
      device_entry_type svr_desc;
      svr_desc.name = server_model_name + get_model_name_suffix();
      svr_desc.model_type = "device";
      svr_desc.class_id = "vire::device::base_device_model";
      // svr_desc.class_id = "vire::mos::opcua_server_device_model";
      svr_desc.config.set_description("Device model configuration parameters");
      svr_desc.config.store_string("aux.vire.mos.device.flavor",
                                   "vire::mos::OPCUA",
                                   "OPCUA top level MOS server");

      // Fetch special userinfos:
      _scan_device_userinfos(server_, svr_desc);

      // Fetch special attributes:
      _scan_device_attributes(server_, svr_desc);

      // Scan embedded devices:
      _scan_devices(server_,
                    server_model_name,
                    base_dir,
                    svr_desc.config);

      if (_logging_ >= datatools::logger::PRIO_DEBUG) {
        svr_desc.tree_dump(std::clog, "OPCUA server '" + server_model_name + "' :", "[debug]: ");
      }

      // Export models:
      datatools::multi_properties server_def("name", "type");

      // Populate with the device model:
      server_def.add(svr_desc.name + "@" + svr_desc.model_type,
                     svr_desc.class_id,
                     svr_desc.config);
      server_def.write(server_model_file_path.string());

      return;
    }

    void opcua_export_vire::_scan_devices(const has_devices_interface & with_devices_,
                                          const std::string & base_name_,
                                          const boost::filesystem::path & base_dir_,
                                          datatools::properties & with_devices_config_)
    {
     // Record data about embedded (simple/compound) device instances:
      std::vector<std::string> embedded_device_labels;
      std::vector<std::string> embedded_device_class_ids;
      std::vector<int>         embedded_device_mults;

      // Scan simple devices:
      for (const auto & sdevice : with_devices_.get_simple_devices()) {
        DT_LOG_DEBUG(_logging_, "Scanning simple device '" << sdevice.name << "'");

        // Check if device is published:
        if (! is_published(sdevice, sdevice.name) && is_export_only_public_components()) {
          // We don't export this simple device component which is part of the non public interface:
          std::cerr << "DEVEL: opcua_export_vire::_scan_devices:   --> DON'T EXPORT" << std::endl;
          continue;
        }

        // Process the daughter device model:
        _process_simple_device(base_name_, base_dir_, sdevice);

        // Record the daughter device instance:
        embedded_device_labels.push_back(sdevice.name);
        embedded_device_class_ids.push_back(base_name_ + '.' + sdevice.name);
        int nb_instances = -1;
        if (sdevice.multiplicity) {
          nb_instances = sdevice.multiplicity.get();
        }
        embedded_device_mults.push_back(nb_instances);
      }

      // Scan compound devices:
      for (const auto & cdevice : with_devices_.get_compound_devices()) {
        DT_LOG_DEBUG(_logging_, "Scanning compound device '" << cdevice.name << "'");

        // Check if device is published:
        if (! is_published(cdevice, cdevice.name) && is_export_only_public_components()) {
          // We don't export this compound device component which is part of the non public interface:
          std::cerr << "DEVEL: opcua_export_vire::_scan_devices:   --> DON'T EXPORT" << std::endl;
          continue;
        }

        // Process the daughter device model:
        _process_compound_device(base_name_, base_dir_, cdevice);

        // Record the daughter device instance:
        embedded_device_labels.push_back(cdevice.name);
        embedded_device_class_ids.push_back(base_name_ + '.' + cdevice.name);
        int nb_instances = -1; // Default single instance
        if (cdevice.multiplicity) {
          nb_instances = cdevice.multiplicity.get();
        }
        embedded_device_mults.push_back(nb_instances);
      }

      // Build embedded devices:
      if (embedded_device_labels.size() > 0) {
        with_devices_config_.store("embedded_devices.labels",
                                   embedded_device_labels,
                                   "List of embedded devices");
        for (int i = 0; i < (int) embedded_device_labels.size(); i++) {
          const std::string & embedded_device_label = embedded_device_labels[i];
          int embedded_device_mult = embedded_device_mults[i];
          {
            // Embedded device model:
            std::string edl_model_name = embedded_device_class_ids[i] + get_model_name_suffix();
            std::ostringstream edl_model_key;
            edl_model_key << "embedded_devices.model." << embedded_device_label;
            with_devices_config_.store_string(edl_model_key.str(),
                                              edl_model_name,
                                              "The model of the embedded device");
          }
          {
            // Embedded device instance:
            if (embedded_device_mult > 0) {
              std::ostringstream edl_instance_key;
              edl_instance_key << "embedded_devices.instance." << embedded_device_label;
              std::ostringstream edl_instance_rule;
              edl_instance_rule << "vire::device::regular_1d_instance="
                                << "[nslots=" << embedded_device_mult << ";first_slot=(1:0);step=(1)]";
              with_devices_config_.store_string(edl_instance_key.str(),
                                                edl_instance_rule.str(),
                                                "The instantiation of the embedded device");
            }
          }
        }
      }

      return;
    }

    void opcua_export_vire::_scan_datapoints(const has_datapoints_interface & device_with_datapoints_,
                                             const std::string & base_name_,
                                             const boost::filesystem::path & base_dir_,
                                             datatools::properties & device_config_)
    {
      // Record data about embedded (simple/compound) datapoint instances:
      std::vector<std::string> embedded_datapoint_labels;
      std::vector<std::string> embedded_datapoint_class_ids;
      std::vector<int>         embedded_datapoint_mults;

      // Scan simple datapoints:
      for (const auto & sdatapoint : device_with_datapoints_.get_simple_datapoints()) {
        DT_LOG_DEBUG(_logging_, "Scanning simple datapoint '" << sdatapoint.name << "'");

        // Check if simple DP is published:
        if (! is_published(sdatapoint, sdatapoint.name) && is_export_only_public_components()) {
          // We don't export this simple datapoint component which is part of the non public interface:
          std::cerr << "DEVEL: opcua_export_vire::_scan_datapoints:   --> DON'T EXPORT" << std::endl;
          continue;
        }

        // Process the daughter datapoint device model:
        _process_simple_datapoint(base_name_, base_dir_, sdatapoint);

        // Record the daughter device instance:
        embedded_datapoint_labels.push_back(sdatapoint.name);
        embedded_datapoint_class_ids.push_back(base_name_ + '.' + sdatapoint.name);
        int nb_instances = -1;
        if (sdatapoint.multiplicity) {
          nb_instances = sdatapoint.multiplicity.get();
        }
        embedded_datapoint_mults.push_back(nb_instances);
      }

      // Scan compound datapoints:
      for (const auto & cdatapoint : device_with_datapoints_.get_compound_datapoints()) {
        DT_LOG_DEBUG(_logging_, "Scanning compound datapoint '" << cdatapoint.name << "'");

        // Check if compound DP is published:
        if (! is_published(cdatapoint, cdatapoint.name) && is_export_only_public_components()) {
          // We don't export this compound datapoint component which is part of the non public interface:
          std::cerr << "DEVEL: opcua_export_vire::_scan_datapoints:   --> DON'T EXPORT" << std::endl;
          continue;
        }

        // Process the daughter datapoint device model:
        _process_compound_datapoint(base_name_, base_dir_, cdatapoint);

        // Record the daughter device instance:
        embedded_datapoint_labels.push_back(cdatapoint.name);
        embedded_datapoint_class_ids.push_back(base_name_ + '.' + cdatapoint.name);
        int nb_instances = -1; // Default single instance
        if (cdatapoint.multiplicity) {
          nb_instances = cdatapoint.multiplicity.get();
        }
        embedded_datapoint_mults.push_back(nb_instances);
      }

      // Build embedded datapoints:
      if (embedded_datapoint_labels.size() > 0) {
        device_config_.store("embedded_devices.labels",
                             embedded_datapoint_labels,
                             "List of embedded datapoint devices");
        for (int i = 0; i < (int) embedded_datapoint_labels.size(); i++) {
          const std::string & embedded_datapoint_label = embedded_datapoint_labels[i];
          int embedded_datapoint_mult = embedded_datapoint_mults[i];
          {
            // Embedded device model:
            std::string edl_model_name = embedded_datapoint_class_ids[i] + get_model_name_suffix();
            std::ostringstream edl_model_key;
            edl_model_key << "embedded_devices.model." << embedded_datapoint_label;
            device_config_.store_string(edl_model_key.str(),
                                        edl_model_name,
                                        "The model of the embedded datapoint device");
          }
          {
            // Embedded device instance:
            if (embedded_datapoint_mult > 0) {
              std::ostringstream edl_instance_key;
              edl_instance_key << "embedded_devices.instance." << embedded_datapoint_label;
              std::ostringstream edl_instance_rule;
              edl_instance_rule << "vire::device::regular_1d_instance="
                                << "[nslots=" << embedded_datapoint_mult << ";first_slot=(1:0);step=(1)]";
              device_config_.store_string(edl_instance_key.str(),
                                          edl_instance_rule.str(),
                                          "The instantiation of the embedded datapoint device");
            }
          }
        }
      }

      return;
    }

    void opcua_export_vire::_scan_methods(const has_methods_interface & device_with_methods_,
                                          const std::string & base_name_,
                                          const boost::filesystem::path & base_dir_,
                                          datatools::properties & device_config_,
                                          std::vector<device_entry_type> & method_port_model_descriptions_)
    {

      // Scan methods:
      std::vector<std::string> method_port_labels;
      std::vector<int>         method_port_mults;
      std::vector<bool>        method_port_public;
      for (const auto & method : device_with_methods_.get_methods()) {
        DT_LOG_DEBUG(_logging_, "Scanning method '" << method.name << "'...");

        // Check if compound DP is published:
        bool public_method = is_published(method, method.name);
        if (! public_method && is_export_only_public_components()) {
          // We don't export this method component which is part of the non public interface:
          std::cerr << "DEVEL: opcua_export_vire::_scan_methods:   --> DON'T EXPORT" << std::endl;
          continue;
        }

        // Record the daughter port model description:
        {
          device_entry_type dummy;
          method_port_model_descriptions_.push_back(dummy);
        }
        device_entry_type & method_port_model_desc = method_port_model_descriptions_.back();
        method_port_model_desc.name = base_name_ + '.' + method.name + get_model_name_suffix();
        method_port_model_desc.model_type = "port";
        // method_port_model_desc.class_id = "vire::device::base_port_model";
        method_port_model_desc.class_id = "vire::device::base_method_port_model";
        // Populate method port config with argument desc...
        _populate_method_port(method, method_port_model_desc.config);

        // Record the daughter port instance:
        int nb_instances = -1; // Default single instance
        method_port_labels.push_back(method.name);
        method_port_mults.push_back(nb_instances);
        method_port_public.push_back(public_method);
      }

      // Build method ports:
      if (method_port_labels.size() > 0) {
        device_config_.store("ports.labels",
                             method_port_labels,
                             "List of method ports");

        for (int i = 0; i < (int) method_port_labels.size(); i++) {
          const std::string & method_port_label = method_port_labels[i];
          int method_port_mult = -1;
          {
            // Method/resource port:
            std::string mpl_model_name = base_name_ + '.' + method_port_label + get_model_name_suffix();
            std::ostringstream mpl_model_key;
            mpl_model_key << "ports.model." << method_port_label;
            device_config_.store_string(mpl_model_key.str(),
                                        mpl_model_name,
                                        "Model of the embedded method port");
          }
        }
      }
      return;
    }

    void opcua_export_vire::_populate_method_port(const Method & method_,
                                                  datatools::properties & method_port_config_)
    {
      // R/W access:
      vire::utility::rw_access_type method_rw_access = translate_rw_access(method_, true);
      method_port_config_.store_string("rw_access",
                                       vire::utility::to_string(method_rw_access),
                                       "Method R/W access mode");

      // Bayeux method:
      datatools::introspection::method dt_method;
      dt_method.set_constness(method_rw_access == vire::utility::RW_READABLE);

      // Loop:
      int arg_counter = 0;
      for (const auto & arg : method_.arguments) {
        // Bayeux method argument:
        datatools::introspection::argument dt_arg;
        std::string arg_name = arg.name;
        std::string arg_description;
        if (arg.description) {
          arg_description = arg.description.get();
        }
        // Access:
        datatools::introspection::access_type arg_access = datatools::introspection::ACCESS_INVALID;
        DT_THROW_IF(!arg.access, std::logic_error, "Argument '" << arg.name << "' has no access!");
        if (arg.access.get() == ::vire::mos::ACCESS_INPUT) {
          arg_access = datatools::introspection::ACCESS_INPUT;
        } else if (arg.access.get() == ::vire::mos::ACCESS_OUTPUT) {
          arg_access = datatools::introspection::ACCESS_OUTPUT;
        }
        // Data description:
        datatools::introspection::data_description arg_dd;
        translate_data_description(arg, arg_dd);

        // Populate the argument:
        dt_arg.set_name(arg_name);
        dt_arg.set_rank(arg_counter);
        if (! arg_description.empty()) {
          dt_arg.set_description(arg_description);
        }
        dt_arg.set_access(arg_access);
        dt_arg.set_data_description(arg_dd);

        // Add the argument to the method:
        dt_method.add_argument(dt_arg);
        arg_counter++;
      }

      // Export the method configuration:
      dt_method.export_to_config(method_port_config_,
                                 datatools::enriched_base::EXPORT_CONFIG_DEFAULT
                                 | datatools::introspection::method::METHOD_XC_CONSTNESS
                                 | datatools::introspection::method::METHOD_XC_TYPE_ID
                                 | datatools::introspection::method::METHOD_XC_ARGUMENTS,
                                 "method.");
     if (_logging_ >= datatools::logger::PRIO_DEBUG) {
        method_port_config_.tree_dump(std::cerr, "Method port config: ", "[devel] ");
      }
      return;
    }

    void opcua_export_vire::_scan_device_userinfos(const has_userinfos_interface & with_userinfos_,
                                                   device_entry_type & model_desc_)
    {
      {
        // Fetch specific device class identifier:
        const auto & found = std::find_if(with_userinfos_.get_userinfos().begin(),
                                          with_userinfos_.get_userinfos().end(),
                                          UserInfo::has_name_predicate(vire::device::class_id_key()));
        if (found != with_userinfos_.get_userinfos().end()) {
          model_desc_.class_id = found->value;
        }
      }

      bool has_role_name = false;
      bool has_role_tags = false;

      if (!has_role_name) {
        // Fetch specific device local role name:
        const auto & found = std::find_if(with_userinfos_.get_userinfos().begin(),
                                          with_userinfos_.get_userinfos().end(),
                                          UserInfo::has_name_predicate(vire::device::local_role_name_key()));
        if (found != with_userinfos_.get_userinfos().end()) {
          const std::string & role_name_value = found->value;
          if (role_name_value == vire::device::local_role_expert()) {
            model_desc_.config.store_string("aux." + vire::device::local_role_name_key(),
                                            vire::device::local_role_expert(),
                                            "Device is accessed only with expert local role");
            has_role_name = true;
          } else {
            DT_LOG_WARNING(_logging_, "Unsupported device local role name '" << found->value << "'");
          }
        }
      }

      if (!has_role_tags) {
        // Fetch specific list of device local role tags:
        const auto & found = std::find_if(with_userinfos_.get_userinfos().begin(),
                                          with_userinfos_.get_userinfos().end(),
                                          UserInfo::has_name_predicate(vire::device::local_role_tags_key()));
        if (found != with_userinfos_.get_userinfos().end()) {
          const std::string & role_tags_value = found->value;
          if (!role_tags_value.empty()) {
            typedef std::vector<std::string > split_vector_type;
            split_vector_type role_tags;
            boost::split(role_tags, role_tags_value, boost::is_any_of(","), boost::token_compress_on);
            if (role_tags.size() > 0) {
              std::vector<std::string> role_tags_safe;
              for (int i = 0; i < role_tags.size(); i++) {
                std::string role_tag = role_tags[i];
                boost::trim(role_tag);
                if (!role_tag.empty()) {
                  role_tags_safe.push_back(role_tag);
                }
              }
              if (role_tags_safe.size() > 0) {
                model_desc_.config.store("aux." + vire::device::local_role_tags_key(),
                                         role_tags_safe,
                                         "Device is accessed only with specific local role tags");
                has_role_tags = true;
              }
            }
          } else {
            DT_LOG_WARNING(_logging_, "Empty device local role tags '" << found->value << "'");
          }
        }
      }

      return;
    }

    void opcua_export_vire::_scan_device_attributes(const has_attributes_interface & common_,
                                                    device_entry_type & common_model_desc_)
    {
      // {
      //   // Fetch specific device class identifier:
      //   const auto & found = std::find_if(common_.get_attributes().begin(),
      //                                     common_.get_attributes().end(),
      //                                     Attribut::has_name_predicate(XXX));
      //   if (found != common_.get_attributes().end()) {
      //     common_model_desc_.xxx = found->value;
      //   }
      // }
      return;
    }

    bool opcua_export_vire::is_published(const has_info_interface & with_info_, const std::string & name_) const
    {
      // Default is published:
      bool published = true;
      if (with_info_.get_info()) {
        const Info & info = with_info_.get_info().get();
        if (info.scope_access) {
          // We found explicit "scope access" information:
          const ScopeAccess & scope_access = info.scope_access.get();
          std::cerr << "DEVEL: opcua_export_vire::is_published: name='" << name_ << "': ScopeAccess = '" << scope_access << "'" << std::endl;
          if (scope_access == SCOPE_ACCESS_INTERNAL) {
            published = false;
          } else if (scope_access == SCOPE_ACCESS_PROTECTED) {
            published = false;
          }
        }
      }
      return published;
    }

  } // namespace mos

} // namespace vire
