//! \file vire/device/instance_tree_builder.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/device/instance_tree_builder.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/utility/path.h>
#include <vire/device/physical_device.h>
#include <vire/device/physical_port.h>
#include <vire/device/i_instance.h>
#include <vire/device/slot.h>

namespace vire {

  namespace device {

    struct instance_tree_builder::work {
      const manager *        mgr = nullptr;
      const logical_device * top_level_logical = nullptr;
      path_stack_type        path_stack;
      instance_tree *        tree = nullptr;
      std::size_t            depth = 0;
    };

    // static
    std::size_t instance_tree_builder::depth_no_limit()
    {
      return 0;
    }

    instance_tree_builder::instance_tree_builder()
    {
      _logging_priority_ = datatools::logger::PRIO_FATAL;
      _max_depth_ = depth_no_limit();
      return;
    }

    instance_tree_builder::~instance_tree_builder()
    {
      return;
    }

    datatools::logger::priority instance_tree_builder::get_logging_priority() const
    {
      return _logging_priority_;
    }

    void instance_tree_builder::set_logging_priority(const datatools::logger::priority l_)
    {
      _logging_priority_ = l_;
      return;
    }

    void instance_tree_builder::set_top_level_device_model_name(const std::string & name_)
    {
      _top_level_device_model_name_ = name_;
      return;
    }

    const std::string & instance_tree_builder::get_top_level_device_model_name() const
    {
      return _top_level_device_model_name_;
    }

    void instance_tree_builder::set_top_level_device_instance_name(const std::string & name_)
    {
      _top_level_device_instance_name_ = name_;
      return;
    }

    const std::string & instance_tree_builder::get_top_level_device_instance_name() const
    {
      return _top_level_device_instance_name_;
    }

    bool instance_tree_builder::has_max_depth() const
    {
      return _max_depth_ != depth_no_limit();
    }

    std::size_t instance_tree_builder::get_max_depth() const
    {
      return _max_depth_;
    }

    void instance_tree_builder::set_max_depth(const std::size_t max_depth_)
    {
      _max_depth_ = max_depth_;
      return;
    }

    bool instance_tree_builder::build_no_devices() const
    {
      return _no_devices_;
    }

    void instance_tree_builder::set_no_devices(bool v_)
    {
      _no_devices_ = v_;
      return;
    }

    bool instance_tree_builder::build_no_ports() const
    {
      return _no_ports_;
    }

    void instance_tree_builder::set_no_ports(bool v_)
    {
      _no_ports_ = v_;
      return;
    }

    void instance_tree_builder::configure(const datatools::properties & config_)
    {
      if (config_.has_flag("debug")) {
        set_logging_priority(datatools::logger::PRIO_DEBUG);
      }

      if (_top_level_device_model_name_.empty()) {
        if (config_.has_key("top_level_device_model")) {
          set_top_level_device_model_name(config_.fetch_string("top_level_device_model"));
        }
      }

      if (_top_level_device_instance_name_.empty()) {
        if (config_.has_key("top_level_device_instance")) {
          set_top_level_device_instance_name(config_.fetch_string("top_level_device_instance"));
        }
      }

      if (config_.has_key("no_devices")) {
        set_no_devices(config_.fetch_boolean("no_devices"));
      }

      if (config_.has_key("no_ports")) {
        set_no_ports(config_.fetch_boolean("no_ports"));
      }

      if (config_.has_key("max_depth")) {
        set_max_depth(config_.fetch_positive_integer("max_depth"));
      }

      if (config_.has_key("only_from_paths")) {
        std::set<std::string> only_from_paths;
        config_.fetch("only_from_paths", only_from_paths);
        for (const auto & p : only_from_paths) {
          add_only_from_path(p);
        }
      }

      return;
    }

    void instance_tree_builder::add_only_from_path(const std::string & path_)
    {
      DT_LOG_DEBUG(_logging_priority_, "Adding only-from path '" << path_ << "'...");
      _only_from_paths_.insert(path_);
      return;
    }

    bool instance_tree_builder::has_only_from_path(const std::string & path_) const
    {
      return _only_from_paths_.count(path_);
    }

    const std::set<std::string> & instance_tree_builder::get_only_from_paths() const
    {
      return _only_from_paths_;
    }

    void instance_tree_builder::reset()
    {
      _only_from_paths_.clear();
      _top_level_device_model_name_.clear();
      _top_level_device_instance_name_.clear();
      _max_depth_ = depth_no_limit();
      _no_devices_ = false;
      _no_ports_ = false;
      return;
    }

    void instance_tree_builder::build(const manager & devmgr_, instance_tree & tree_)
    {
      DT_THROW_IF(!devmgr_.is_initialized(), std::logic_error,
                  "Device manager is not initialized!");
      tree_.reset();
      _work_.reset(new work);
      _work_->mgr = &devmgr_;
      _work_->tree = &tree_;

      if (_top_level_device_model_name_.empty()) {
        _top_level_device_model_name_ = _work_->mgr->get_top_level_device_model_name();
      }

      if (_top_level_device_instance_name_.empty()) {
        _top_level_device_instance_name_ = _work_->mgr->get_setup_label() + vire::utility::path::setup_separator();
      }

      _top_build_();

      _work_.reset();
      return;
    }

    void instance_tree_builder::_top_build_()
    {
      if (_work_->top_level_logical == nullptr) {
        DT_THROW_IF(!_work_->mgr->has_device_model(_top_level_device_model_name_),
                    std::logic_error,
                    "Cannot find the top-level device model with name '" << _top_level_device_model_name_
                    << "' in the device models manager !");
        _work_->top_level_logical
          = &(_work_->mgr->get_device_model(_top_level_device_model_name_).get_logical());
        DT_LOG_DEBUG(get_logging_priority(), "Top level logical @ " << _work_->top_level_logical);
      }

      _build_();

      return;
    }

    void instance_tree_builder::_build_()
    {
      instance_info_ptr pinfo(new instance_info);
      pinfo->set_path(_top_level_device_instance_name_);
      pinfo->set_logical_device(*_work_->top_level_logical);
      _work_->tree->set_parent_path(_top_level_device_instance_name_);
      _work_->tree->add_instance(pinfo);
      _work_->path_stack.push_back(pinfo->get_path());
      _build_device_(pinfo->get_logical_device());
      // _build_embedded_devices_(pinfo->get_logical_device());
      _work_->tree->lock();
      return;
    }

    void instance_tree_builder::_build_embedded_devices_(const logical_device & mother_log_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      // Map the daughter embedded devices of the logical device:
      for (logical_device::daughters_dict_type::const_iterator i
             = mother_log_.get_daughters().begin();
           i != mother_log_.get_daughters().end();
           i++) {
        const std::string & embedded_name = i->first;
        const physical_device & embedded_phys = *(i->second);
        DT_LOG_DEBUG(get_logging_priority(),
                     "Embedded physical device '" << embedded_name << "' with physical name "
                     << "'" << embedded_phys.get_name() << "'");
        const logical_device & embedded_logical = embedded_phys.get_logical();
        DT_THROW_IF(!embedded_phys.has_instance(), std::logic_error,
                    "Missing instance in embedded device '" << embedded_name << "'!");
        const i_instance & phys_instance = embedded_phys.get_instance();
        DT_LOG_DEBUG(get_logging_priority(), "Number of items: " << phys_instance.get_number_of_items());

        // Loop on replicated embedded devices:
        for (size_t item = 0; item < phys_instance.get_number_of_items(); item++) {
          DT_LOG_DEBUG(get_logging_priority(), "-> embedded item #" << item << ": ");
          // Get the current item instance in the mother slotting system:
          slot item_slot;
          phys_instance.fetch_slot(item, item_slot);
          {
            DT_LOG_DEBUG(get_logging_priority(), "-> embedded child slot : " << item_slot);
          }
          const std::string & mother_path = _work_->path_stack.back();
          // Compute the vector of sub-addresses:
          std::vector<uint32_t> embedded_indexes;
          item_slot.fetch_coordinates(embedded_indexes, true);
          DT_LOG_DEBUG(get_logging_priority(), "-> embedded index: size=[" << embedded_indexes.size() << "]");
          for (int idx = 0; idx < (int) embedded_indexes.size(); idx++) {
            DT_LOG_DEBUG(get_logging_priority(), "  -> embedded index: idx=[" << idx << "]");
          }
          DT_LOG_DEBUG(get_logging_priority(),
                       "Embedded index number of coordinates = ["
                       << embedded_indexes.size() << "]");
          for (int i = 0; i < (int) embedded_indexes.size(); i++) {
            DT_LOG_DEBUG(get_logging_priority(), " - embedded_indexes[" << i << "]=" << embedded_indexes[i]);
          }
          DT_LOG_DEBUG(get_logging_priority(), "embedded mother_path   = '" << mother_path << "'");
          std::string embedded_path;
          vire::utility::path::build_child_with_indexes(mother_path, embedded_name, embedded_indexes, embedded_path);

          DT_LOG_DEBUG(get_logging_priority(), "Attempt to add embedded device with path = '" << embedded_path << "'...");
          bool add_it = false;

          if (_only_from_paths_.empty()) {
            add_it = true;
          } else {
            DT_LOG_DEBUG(_logging_priority_, "Checking only-from path selectors for '" << embedded_path << "'...");
            if (!add_it) {
              bool found_only_self = false;
              for (const auto & only_from_path : _only_from_paths_) {
                if (embedded_path == only_from_path) {
                  found_only_self = true;
                  break;
                }
              }
              if (found_only_self) {
                add_it = true;
              }
            }
            if (!add_it) {
              bool found_only_parent = false;
              for (const auto & only_from_path : _only_from_paths_) {
                DT_LOG_DEBUG(_logging_priority_,
                             "Check if '" << embedded_path << "' is a parent of only-from '" << only_from_path << "'");
                if (vire::utility::path::is_parent_of(embedded_path, only_from_path)) {
                  DT_LOG_DEBUG(_logging_priority_, "Found '" << embedded_path << "' as a parent of only-from '" << only_from_path << "'");
                  found_only_parent = true;
                  break;
                }
              }
              if (found_only_parent) {
                add_it = true;
              }
            }
            if (!add_it) {
              // At least one "only-from" path should parent the embedded device:
              bool found_only = false;
              for (const auto & path : _only_from_paths_) {
                if (vire::utility::path::is_child_of(embedded_path, path)) {
                  found_only = true;
                  DT_LOG_DEBUG(_logging_priority_, "Found '" << embedded_path << "' as a child of only-from '" << path << "'");
                  break;
                }
              }
              if (found_only) {
                add_it = true;
              }
            }
          }
          DT_LOG_DEBUG(_logging_priority_, "Selection of '" << embedded_path << "' : " << std::boolalpha << add_it);

          if (add_it) {
            instance_info_ptr pinfo(new instance_info);
            pinfo->set_path(embedded_path);
            pinfo->set_logical_device(embedded_logical);
            _work_->tree->add_instance(pinfo);
            bool add_it = true;
            _work_->path_stack.push_back(embedded_path);
            bool build_it = true;
            if (has_max_depth()) {
              if (_work_->depth > _max_depth_) {
                build_it = false;
              }
            }
            if (build_it) {
              DT_LOG_DEBUG(get_logging_priority(), "Built device for '" << embedded_logical.get_name() << "'");
              DT_LOG_DEBUG(get_logging_priority(), "Path stack:");
              for (path_stack_type::const_iterator ianc = _work_->path_stack.begin();
                   ianc != _work_->path_stack.end();
                   ianc++) {
                DT_LOG_DEBUG(get_logging_priority(), "  Ancestors' path = '" << *ianc << "'");
              }
              _build_device_(embedded_logical);
            } else {
              DT_LOG_DEBUG(get_logging_priority(), "-> DO NOT TRAVERSE THE DEVICE TREE FURTHER.");
            }
            _work_->path_stack.pop_back();
          }
        } // for instantiated item in the physical embedded device
      } // for embedded devices

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void instance_tree_builder::_build_device_(const logical_device & mother_log_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      ++_work_->depth;

      if (!build_no_ports()) {
        // Build records for all ports in the mother logical device:
        _build_ports_(mother_log_);
      }

      // Build records for all embedded devices in the mother logical device:
      _build_embedded_devices_(mother_log_);

      // if (!build_no_links()) {
      //   // Build mapping records for all mapped links in the mother logical device:
      //   //_build_links_(mother_log_);
      // }

      --_work_->depth;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void instance_tree_builder::_build_ports_(const logical_device & mother_log_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // Map the ports of the logical device:
      for (logical_device::ports_dict_type::const_iterator i
             = mother_log_.get_ports().begin();
           i != mother_log_.get_ports().end();
           i++) {
        const std::string & port_name = i->first;
        const physical_port & port_phys = *(i->second);
        const logical_port & port_logical = port_phys.get_logical();

        DT_THROW_IF(!port_phys.has_instance(), std::logic_error, "Missing instance in port '" << port_name << "'!");
        const i_instance & phys_instance = port_phys.get_instance();
        DT_LOG_TRACE(get_logging_priority(), "Number of items: " << phys_instance.get_number_of_items());

        // Loop on replicated ports:
        for (size_t item = 0; item < phys_instance.get_number_of_items(); item++) {
          DT_LOG_DEBUG (get_logging_priority(), "-> item #" << item << ": ");
          // Get the current item instance in the mother slotting system:
          slot item_slot;
          phys_instance.fetch_slot(item, item_slot);
          {
            DT_LOG_DEBUG(get_logging_priority(), "-> child slot : " << item_slot);
          }
          const std::string & mother_path = _work_->path_stack.back();
          // Compute the vector of sub-addresses:
          std::vector<uint32_t> port_indexes;
          item_slot.fetch_coordinates(port_indexes, true);
          DT_LOG_DEBUG(get_logging_priority(), "-> port index: size=[" << port_indexes.size() << "]");
          for (int idx = 0; idx < (int) port_indexes.size(); idx++) {
            DT_LOG_DEBUG(get_logging_priority(), "  -> port index: idx=[" << idx << "]");
          }
          DT_LOG_DEBUG(get_logging_priority(),
                       "Port index number of coordinates = ["
                       << port_indexes.size() << "]");
          for (int i = 0; i < (int) port_indexes.size(); i++) {
            DT_LOG_DEBUG(get_logging_priority(), " - port_indexes[" << i << "]=" << port_indexes[i]);
          }
          DT_LOG_DEBUG(get_logging_priority(), "Port mother_path   = '" << mother_path << "'");
          std::string port_path;
          vire::utility::path::build_child_with_indexes(mother_path, port_name, port_indexes, port_path);
          DT_LOG_DEBUG(get_logging_priority(), "Attempt to add port with path = '" << port_path << "'...");
          instance_info_ptr pinfo(new instance_info);
          pinfo->set_path(port_path);
          pinfo->set_logical_port(port_logical);
          _work_->tree->add_instance(pinfo);
        } // for instantiated item in the physical port
      } // for ports

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

  } // end of namespace device

} // end of namespace vire
