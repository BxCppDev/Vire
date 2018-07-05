//! \file vire/cmsserver/base_use_case.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/base_use_case.h>

// Standard Library:
#include <thread>
#include <chrono>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/datatools/units.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/introspection/method.h>

// This project:
#include <vire/device/manager.h>
#include <vire/cmsserver/uc_factory.h>
#include <vire/cmsserver/uc_error.h>
#include <vire/cmsserver/uc_time_constraints.h>
#include <vire/device/base_port_model.h>
#include <vire/device/logical_port.h>
#include <vire/device/physical_port.h>
#include <vire/device/logical_device.h>
#include <vire/device/physical_device.h>
#include <vire/device/base_datapoint_model.h>

namespace vire {

  namespace cmsserver {

    // Factory system register :
    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_use_case,
                                                     "vire::cmsserver::base_use_case/_system_")

    VIRE_USE_CASE_REGISTRATION_IMPLEMENT(base_use_case, "vire::cmsserver::base_use_case")

    
    // static
    std::string base_use_case::run_mode_to_label(const run_mode_type mode_)
    {
      switch (mode_) {
      case RUN_MODE_DRYRUN : return std::string("dry_run");
      case RUN_MODE_RUN    : return std::string("run");
      case RUN_MODE_DEAD   : return std::string("dead");
      }
      return std::string("");
    }

    // static
    std::string base_use_case::to_string(const construction_stage_type stage_)
    {
      switch (stage_) {
      case CONSTRUCT_READY : return std::string("ready");
      case CONSTRUCT_CONFIG_SETUP : return std::string("config_setup");
      case CONSTRUCT_CONFIG_COMPO : return std::string("config_compo");
      case CONSTRUCT_COMPOSED : return std::string("composed");
      case CONSTRUCT_SCOPE_SPECS : return std::string("scope_specs");
      case CONSTRUCT_DISTRIBUTABLE_MOUNT_SPECS : return std::string("distributable_mount_specs");
      case CONSTRUCT_FUNCTIONAL_SPECS : return std::string("functional_specs");
     }
      return std::string("");
    }

    base_use_case::construction_stage_type base_use_case::get_construction_stage() const
    {
      return _construct_stage_;
    }

    // virtual
    bool base_use_case::is_run_system_optimizable() const
    {
      return false;
    }

    // virtual
    bool base_use_case::is_repeatable() const
    {
      return true;
    }
 
    const uc_construction_context_type & base_use_case::get_cc() const
    {
      return _cc_;
    }

    void base_use_case::set_cc(const uc_construction_context_type & cc_)
    {
      _cc_ = cc_;
      return;
    }
        
    // virtual
    void base_use_case::_build_scope_requirements_(uc_parametrised_resource_specifications & specs_) const
    {
       return;
    }

    bool base_use_case::has_mount_point(const std::string & name_) const
    {
      return _mount_points_.count(name_);
    }

    bool base_use_case::is_completed_mount_points() const
    {
      // if (_mount_points_.size() == _resource_requirements_.size()) {
      //   return true;
      // }
      return false;
    }
    
    void base_use_case::set_device_mount_point(const std::string & name_,
                                               const std::string & device_path_)
    {
      /*
      DT_THROW_IF(_resource_requirements_.is_empty(),
                  std::logic_error,
                  "This use case has no relative requirements!");
      const uc_parametrised_resource_specifications & rfr = _resource_requirements_;
      DT_THROW_IF(!rfr.has(name_),
                  std::logic_error,
                  "No relative required object with name '" << name_ << "' is defined!");
      DT_THROW_IF(!rfr.is_device(name_),
                  std::logic_error,
                  "Relative required object with name '" << name_ << "' is not a device!");
      DT_THROW_IF(has_mount_point(name_),
                  std::logic_error,
                  "Relative mount point with name '" << name_ << "' is already set!");
      _mount_points_[name_] = device_path_;
      */
      return;
    }

    void base_use_case::set_resource_mount_point(const std::string & name_,
                                                 const std::string & resource_path_)
    {
      /*
      DT_THROW_IF(_resource_requirements_.is_empty(),
                  std::logic_error,
                  "This use case has no relative requirements!");
      const uc_parametrised_resource_specifications & rfr = _resource_requirements_;
      DT_THROW_IF(!rfr.has(name_),
                  std::logic_error,
                  "No relative required object with name '" << name_ << "' is defined!");
      DT_THROW_IF(!rfr.is_resource(name_),
                  std::logic_error,
                  "Relative required object with name '" << name_ << "' is not a resource!");
      DT_THROW_IF(has_mount_point(name_),
                  std::logic_error,
                  "Relative mount point with name '" << name_ << "' is already set!");
      _mount_points_[name_] = resource_path_;
      */
      return;
    }

    void base_use_case::set_run_mode(const run_mode_type mode_)
    {
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_UNDEF,
                  std::logic_error,
                  "Invalid construction stage!");
      DT_THROW_IF(_run_mode_ != RUN_MODE_UNDEF, std::logic_error, "Run mode is already set!");
      _run_mode_ = mode_;
      _construct_stage_ = CONSTRUCT_READY;
      return;
    }

    const uc_parametrised_resource_specifications &
    base_use_case::get_scope_requirements() const
    {
      return _scope_requirements_;
    }
 
    void base_use_case::build_scope_requirements()
    {
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_COMPOSED,
                  std::logic_error,
                  "Construction stage is already initialized!");
      
      // Call back computing the scope resource requirements:
      _build_scope_requirements_(_scope_requirements_);

      // // Fetch scope resource requirement
      // const uc_composition_description & comp_desc = this->get_composition_description();
      // std::vector<std::string> names;
      // comp_desc.build_daughter_names(names); 
      // for (const std::string & dname : names) {
      //   composition_dict_type::const_iterator found = _composition_.find(dname);
      //   DT_THROW_IF(found == _composition_.end(),
      //               std::logic_error,
      //               "No daughter use case named '" << dname << "'!");
      //   const uc_composition_description::daughter_entry & de
      //       = comp_desc.get_daughter(dname);
      //   // XXX add_daughter_scope_requirements(*de.instance.get());
      // }

      _construct_stage_ = CONSTRUCT_SCOPE_SPECS;
      return;
    }

    void base_use_case::build_distributable_mounting_table()
    {
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_SCOPE_SPECS,
                  std::logic_error,
                  "Invalid construction stage!");
      
      // Call back computing the type's scope resource requirements:
      _build_distributable_mounting_table_(_distributable_mounting_table_);
       
      _setup_distributable_mounting_table_();
     
      _check_distributable_mounting_table_();

      if (!_distributable_mounting_table_.is_locked()) {
        _distributable_mounting_table_.lock();
      }
      _construct_stage_ = CONSTRUCT_DISTRIBUTABLE_MOUNT_SPECS;
      return;
    }
    
    void base_use_case::_build_distributable_mounting_table_(uc_resource_mounting_table & mtab_)
    {
      return;
    }

    void base_use_case::_setup_distributable_mounting_table_()
    {
      datatools::properties mounting_config;
      _configuration_.export_and_rename_starting_with(mounting_config, "mounting_table.", "");
      if (mounting_config.size()) {
        DT_THROW_IF(_distributable_mounting_table_.is_locked(),
                    std::logic_error,
                    "Resource mounting directives seems available but distributable resource mounting table is system locked!");
        std::set<std::string> from_setup_names;
        for (composition_dict_type::const_iterator i = _composition_.begin();
             i != _composition_.end();
             i++) {
          if (i->second.from_setup) {
            from_setup_names.insert(i->first);
          }
        }
        _distributable_mounting_table_.configure_only_for(mounting_config, from_setup_names);
      }
      return;
    }
 
    void base_use_case::_check_distributable_mounting_table_()
    {
      std::map<uc_port_id, std::string> unsatisfied_daughter_scope_ids;
      for (composition_dict_type::const_iterator idaughter = _composition_.begin();
           idaughter != _composition_.end();
           idaughter++) {
        // Traverse the dictionary of daughter use cases:
        const std::string & daughter_name = idaughter->first;
        const composition_entry_type & ce = idaughter->second;
        const base_use_case & daughter_uc = *ce.instance.get();
        const uc_parametrised_resource_specifications & daughter_scope_specs = daughter_uc.get_scope_requirements();
        std::set<std::string> daughter_scope_keys;
        daughter_scope_specs.build_keys(daughter_scope_keys);
        for (const std::string & daughter_scope_key : daughter_scope_keys) {
          // Traverse the collection of scope resource specs fro this daughter:
          uc_port_id from_id(daughter_name, daughter_scope_key);
          if (!_distributable_mounting_table_.has(from_id)) {
            unsatisfied_daughter_scope_ids[from_id] = "Daughter scope ID '" + from_id.to_string() + "' does not exist in the mounting table";
            continue;
          }
          const uc_resource_mount_link & mlink = _distributable_mounting_table_.get(from_id);
          const std::string & to_key = mlink.get_to_key();
          if (!_scope_requirements_.has(to_key)) {
            unsatisfied_daughter_scope_ids[from_id] = "Missing use case's scope resource requirement '" + to_key + "'";
            continue;
          }
          const uc_parametrised_resource_specifications::resource_spec_entry & parent_spec
            = _scope_requirements_.get_specification(to_key);
          const uc_parametrised_resource_specifications::resource_spec_entry & daughter_spec
            = daughter_scope_specs.get_specification(daughter_scope_key);
          std::tuple<bool,std::string> ret = match_mount(daughter_spec,
                                                         parent_spec,
                                                         mlink.get_relative_path());
          if (! std::get<0>(ret)) {
            unsatisfied_daughter_scope_ids[from_id] = "Incompatible scope resource requirement from '" + from_id.to_string() + "' with use case's scope spec '" + to_key + "' : " + std::get<1>(ret);
            continue;
          }
        }
      }
      if (unsatisfied_daughter_scope_ids.size()) {
        for (const auto & p : unsatisfied_daughter_scope_ids) {
          DT_LOG_FATAL(get_logging_priority(), "Unsatisfied daughter scope ID '" << p.first << "' : reason: " << p.second);
        }
        DT_THROW(std::logic_error,
                 "There are unsatisfied daughter use case's scope resource mounting specifications!");
      }
      return;
    }

    std::tuple<bool,std::string>
    base_use_case::match_mount(const uc_parametrised_resource_specifications::resource_spec_entry & from_spec_,
                               const uc_parametrised_resource_specifications::resource_spec_entry & to_spec_,
                               const vire::utility::relative_path & relpath_) const
    {
      if (from_spec_.is_complete() || to_spec_.is_complete()) {
        std::string error_msg = "Incomplete resource specifications!";
        return std::make_tuple(false, error_msg);
      }

      if (to_spec_.is_resource()) {
        // ****************************
        // *   To == plain resource   *
        // ****************************
        if (from_spec_.is_device()) {
          // Eliminate trivial case 'from' device vs plain 'to' resource:
          std::string error_msg = "'from' device cannot match a 'to' resource!";
          return std::make_tuple(false, error_msg);
        } else if (from_spec_.is_resource()) {
          // Process 'from' resource vs plain 'to'  resource:
          std::tuple<bool,std::string> matched_result = from_spec_.match(to_spec_);
          return matched_result;
        }
      } else {
        // ********************
        // *   To == device   *
        // ********************
        if (relpath_.is_empty()) {
          if (from_spec_.is_resource()) {
            // Eliminate trivial case 'from' resource vs plain 'to' device:
            std::string error_msg = "'from' resource cannot match a 'to' device!";
            return std::make_tuple(false, error_msg);
          } else {
            // Process 'from' device vs plain 'to'  device:
            std::tuple<bool,std::string> matched_result = from_spec_.match(to_spec_);
            return matched_result;
          }
        } else {
          // Check of the 'to' device has a model ID:
          if (!to_spec_.has_device_model_id()) {
            std::string error_msg
              = std::string("'to' device specs does not have an explicit device model ID to be used with relative path '")
              + relpath_.to_string() + "'!";
            return std::make_tuple(false, error_msg);
          }
          // Check if the 'to' model ID is known from de device manager:
          if (!_cc_.devices->has_device_model(to_spec_.device_model_id)) {
            std::string error_msg
              = std::string("'to' device specs with explicit device model ID '")
              + to_spec_.device_model_id + "' is registered in the device manager!";
            return std::make_tuple(false, error_msg);       
          }
          // Fetch the device model from the device manager:
          const vire::device::base_device_model & to_dev_model
            = _cc_.devices->get_device_model(to_spec_.device_model_id);
          const vire::device::base_device_model * current_to_dev_model = &to_dev_model;
          const vire::device::base_device_model * found_to_dev_model = nullptr;
          const vire::device::base_port_model   * found_to_port_model = nullptr;
          
          // Scan the relative path with respect to the 'to' device model:
          for (std::size_t i = 0; i < relpath_.get_length(); i++) {
            bool at_leaf = false;
            // bool current_to_dev_is_datapoint = false;
            // if (dynamic_cast<const vire::device::base_datapoint_model *>(current_to_dev_model) != nullptr) {
            //   current_to_dev_is_datapoint = true;
            // }
            if ((i + 1) < relpath_.get_length()) {
              // We tag the last segment of the relative path:
              at_leaf = true;
            }
            // Last segment of the relative path may be a method port:
            bool could_be_a_port = at_leaf;
            const vire::device::logical_device & current_to_dev_log = current_to_dev_model->get_logical();
            std::string seg_name = relpath_.get_segments()[i];
            // Note: method ID shortcuts are not allowed by the path syntax...
            // if (at_leaf && current_to_dev_is_datapoint) {
            //   // Shortcuts
            //   if (seg_name == "?") {
            //     seg_name = vire::device::base_datapoint_model::read_method_name();
            //   } else if (seg_name == "=") {
            //     seg_name = vire::device::base_datapoint_model::write_method_name();
            //   }
            // }

            if (could_be_a_port) {
              // Search for a port instance first: 
              const std::map<std::string, std::string> & to_dev_log_port_names = current_to_dev_log.get_port_names();
              if (to_dev_log_port_names.count(seg_name) == 1) {
                /// Found a port (resource) with proper daughter name: 
                const std::string & port_label = to_dev_log_port_names.find(seg_name)->second;
                if (!current_to_dev_model->has_port(port_label)) {
                  std::string error_msg
                    = std::string("'to' device specs of model '")
                    + current_to_dev_model->get_name()
                    + "' has no port with label '"
                    + port_label + "'! This is a bug!";
                  return std::make_tuple(false, error_msg);            
                }
                const vire::device::physical_port & to_port_phys
                  = current_to_dev_model->get_port(port_label);
                const vire::device::logical_port & to_port_log = to_port_phys.get_logical();
                const vire::device::base_port_model & to_port_model = to_port_log.get_model();
                found_to_port_model = &to_port_model;
                break;
              }
            }
            // Search for an embedded daughter device: 
            const std::map<std::string, std::string> & to_dev_log_daughter_names = current_to_dev_log.get_daughter_names();
            if (to_dev_log_daughter_names.count(seg_name) == 0) {
              std::string error_msg
                = std::string("'to' device specs of model '")
                + current_to_dev_model->get_name()
                + "' has no embedded device instance with name '"
                + seg_name + "'!";
              return std::make_tuple(false, error_msg);            
            }
            const std::string & daughter_label = to_dev_log_daughter_names.find(seg_name)->second;
            if (!current_to_dev_model->has_embedded_device(daughter_label)) {
              std::string error_msg
                = std::string("'to' device specs of model '")
                + current_to_dev_model->get_name()
                + "' has no embedded device with label '"
                + daughter_label + "'! This is a bug!";
              return std::make_tuple(false, error_msg);            
            }
            /// Found a device with proper daughter name: 
            const vire::device::physical_device & to_dev_phys
              = current_to_dev_model->get_embedded_device(daughter_label);
            const vire::device::logical_device & to_dev_log = to_dev_phys.get_logical();
            current_to_dev_model = &to_dev_log.get_model();
            if (at_leaf) {
              found_to_dev_model = current_to_dev_model;
              break;
            }
          } // for relpath

          if (found_to_port_model) {

            // We found a link to port model:
           
            if (from_spec_.is_device()) {
              // Eliminate trivial case 'from' device vs 'to' sub-resource:
              std::string error_msg = "'from' device cannot match a 'to' resource!";
              return std::make_tuple(false, error_msg);
            }
           
            if (found_to_port_model->get_type() != vire::device::PORT_METHOD) {
              std::string error_msg
                = std::string("'to' device specs of model '")
                + current_to_dev_model->get_name()
                + "' has a non-method port '"
                + found_to_port_model->get_name() + "'!";
              return std::make_tuple(false, error_msg);            
            }
            const vire::device::base_method_port_model * meth_port_model
              = dynamic_cast<const vire::device::base_method_port_model *>(found_to_port_model);
            DT_THROW_IF(meth_port_model = nullptr,
                        std::logic_error,
                        "Cannot cast a method port model from mount relative path '" << relpath_ << "'!");
            // const vire::device::logical_port & to_port_log = meth_port_model->get_logical();
            const ::datatools::introspection::method & to_meth = meth_port_model->get_method();

            // if (from_spec_.has_resource_model_id() && meth_port_model.get_name() != from_spec_.resource_model_id) {
            //   Should we check the method port model if (string)???
            // }
           
            if (from_spec_.has_resource_category()) {
              if (from_spec_.resource_category != vire::resource::RESOURCE_CATEGORY_METHOD) {
                std::string error_msg
                  = std::string("'from' resource specs is not of method category!");
                return std::make_tuple(false, error_msg);            
              }
            }
            
            if (from_spec_.has_resource_access()) {
              if (to_meth.is_constness() && from_spec_.resource_access != vire::resource::ACCESS_READ_ONLY) {
                std::string error_msg
                  = std::string("'from' method resource specs does not match the method constness!");
                return std::make_tuple(false, error_msg);            
              }
            }
            
            if (to_meth.is_constness()) {
              if (!to_meth.has_unique_returned()) {
                std::string error_msg
                  = std::string("Unsupported case: 'to' method does not have an unique returned value!");
                return std::make_tuple(false, error_msg);            
              }
              const ::datatools::introspection::argument & to_meth_returned = to_meth.get_unique_returned();
              if (!to_meth_returned.is_output()) {
                std::string error_msg
                  = std::string("Bug! 'to' method returned value is not tagged as 'output'!");
                return std::make_tuple(false, error_msg);            
              }
              if (to_meth_returned.has_data_description()) {
                const ::datatools::introspection::data_description & dd = to_meth_returned.get_data_description();
                if (dd.get_layout() != ::datatools::introspection::DATA_LAYOUT_SCALAR) {
                  std::string error_msg
                    = std::string("Unsupported case: 'to' method does not have a scalar returned value!");
                }
                 
                if (dd.has_type() 
                    && from_spec_.has_resource_datatype_id()
                    && from_spec_.resource_datatype_id != dd.get_type()) {
                  std::string error_msg
                    = std::string("'from' method resource specs does not match the datatype of the method's returned value ('")
                    + ::datatools::introspection::to_string(from_spec_.resource_datatype_id) + "' vs '"
                    + ::datatools::introspection::to_string(dd.get_type()) + "')!";
                  return std::make_tuple(false, error_msg);                       
                }
                
                if (!dd.is_dimensionless()
                    && from_spec_.has_resource_unit_dimension()
                    && dd.get_unit_dimension_label() != from_spec_.resource_unit_dimension) {
                  std::string error_msg
                    = std::string("'from' method resource specs does not match the unit dimension of the method's returned value ('")
                    + dd.get_unit_dimension_label() + "' vs '"
                    + from_spec_.resource_unit_dimension + "')!";
                  return std::make_tuple(false, error_msg);                       
                }
                  
              }              
            } else {
              if (!to_meth.has_input_arguments()) {
                std::string error_msg
                  = std::string("Unsupported case: 'to' method does not have input arguments!");
                return std::make_tuple(false, error_msg);            
              }
              const ::datatools::introspection::argument & to_meth_first_arg = to_meth.get_argument_by_rank(0);
              if (!to_meth_first_arg.is_input()) {
                std::string error_msg
                  = std::string("Bug! 'to' method first input argument value is not tagged as 'input'!");
                return std::make_tuple(false, error_msg);            
              }
              if (to_meth_first_arg.has_data_description()) {
                const ::datatools::introspection::data_description & dd = to_meth_first_arg.get_data_description();
                if (dd.get_layout() != ::datatools::introspection::DATA_LAYOUT_SCALAR) {
                  std::string error_msg
                    = std::string("Unsupported case: 'to' method does not have a scalar first argument!");
                }
                if (dd.has_type() 
                    && from_spec_.has_resource_datatype_id()
                    && from_spec_.resource_datatype_id != dd.get_type()) {
                  std::string error_msg
                    = std::string("'from' method resource specs does not match the datatype of the method's first argument ('")
                    + ::datatools::introspection::to_string(from_spec_.resource_datatype_id) + "' vs '"
                    + ::datatools::introspection::to_string(dd.get_type()) + "')!";
                  return std::make_tuple(false, error_msg);                       
                }
                
                if (!dd.is_dimensionless()
                    && from_spec_.has_resource_unit_dimension()
                    && dd.get_unit_dimension_label() != from_spec_.resource_unit_dimension) {
                  std::string error_msg
                    = std::string("'from' method resource specs does not match the unit dimension of the method's first argument ('")
                    + from_spec_.resource_unit_dimension + "' vs '"
                    + dd.get_unit_dimension_label() + "')!";
                  return std::make_tuple(false, error_msg);                       
                }  
              }              
            }
            
            return std::make_tuple(true, "");
          } else {
           
            // We found a link to a device model:
           
            if (from_spec_.is_resource()) {
              // Eliminate trivial case 'from' resource vs 'to' sub-device:
              std::string error_msg = "'from' resource cannot match a 'to' sub-device!";
              return std::make_tuple(false, error_msg);
            }

            // Check device model IDs:
            if (from_spec_.has_device_model_id()
                && from_spec_.device_model_id != found_to_dev_model->get_name()) {
              std::string error_msg
                = std::string("'from' device and 'to' sub-device are not devices of the same model ('")
                + from_spec_.device_model_id + "' vs '"
                + found_to_dev_model->get_name() + "')!";          
              return std::make_tuple(false, error_msg);
            }
            
            // Check device types:
            if (from_spec_.has_device_type()
                && found_to_dev_model->has_type()
                && from_spec_.device_type_id != vire::device::TYPE_GENERIC
                && found_to_dev_model->get_type() != vire::device::TYPE_GENERIC
                && (from_spec_.device_type_id != found_to_dev_model->get_type())) {
              std::string error_msg
                = std::string("'from' device and 'to' sub-device are not devices of the same type ('")
                + vire::device::get_device_type_label(from_spec_.device_type_id) + "' vs '"
                + vire::device::get_device_type_label(found_to_dev_model->get_type()) + "')!";          
              return std::make_tuple(false, error_msg);
            } // device types
            
            return std::make_tuple(true, "");
          } // relpath
        }
        
      } // to == device
      
      return std::make_tuple(false, "No match.");
    }
    
    const uc_resource_mounting_table &
    base_use_case::get_distributable_mounting_table() const
    {
      return _distributable_mounting_table_;
    }
    
    void base_use_case::build_functional_requirements()
    {
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_DISTRIBUTABLE_MOUNT_SPECS,
                  std::logic_error,
                  "Invalid construction stage!");

      _build_functional_requirements_(_functional_requirements_);

      _construct_stage_ = CONSTRUCT_FUNCTIONAL_SPECS;
      return;
    }

    const uc_functional_resource_specifications &
    base_use_case::get_functional_requirements() const
    {
      return _functional_requirements_;
    }
 
    void base_use_case::add_relative_functional(const std::string & key_,
                                                const vire::utility::relative_path & rp_)
    {
      return;
    }

    void base_use_case::add_absolute_functional(const std::string & key_,
                                                const vire::utility::path &)
    {
      return;
    }

    // virtual
    void base_use_case::_build_functional_requirements_(uc_functional_resource_specifications & fund_res_specs_)
    {
      return;
    }

    void base_use_case::_check_functional_requirements_()
    {

      
      return;
    }

    /* */
    
    void base_use_case::build_scope_resources()
    {
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_DISTRIBUTABLE_MOUNT_SPECS,
                  std::logic_error,
                  "Invalid construction stage!");

      // if (!_scope_resources_ptr_) {
      //   _scope_resources_ptr_ = std::make_shared<uc_scope_resources>();
      // }
      
      // _compute_list_of_distributables_();

      // // Callback to compute:
      // _build_list_of_functional_resources_(_functional_resource_requirements_);
      
      // // Callback to compute:
      // _build_list_of_functional_resources_(*_scope_resources_ptr_);
      
      _construct_stage_ = CONSTRUCT_RELATIVE_SCOPE;
      return;
    }
 
    // virtual
    // void base_use_case::_build_list_of_functional_resources_(uc_scope_resources &)
    // {
    //   return;
    // }
    
    base_use_case::base_use_case()
    {
      return;
    }

    base_use_case::~base_use_case()
    {
      return;
    }

    bool base_use_case::has_rc() const
    {
      return _rc_.get() != nullptr;
    }
                        
    const running::run_control & base_use_case::get_rc() const
    {
      DT_THROW_IF(!has_rc() , std::logic_error, "No run control!")
      return *_rc_;
    }

    void base_use_case::set_rc(const std::shared_ptr<running::run_control> & rc_)
    {
      DT_THROW_IF(!is_run_mode(), std::logic_error, "Run mode is not activated!");
      DT_THROW_IF(has_rc(), std::logic_error, "Run control is already set!");
      _rc_ = rc_;
      return;
    }

    bool base_use_case::is_dry_run_mode() const
    {
      return _run_mode_ == RUN_MODE_DRYRUN;
    }

    bool base_use_case::is_run_mode() const
    {
      return _run_mode_ == RUN_MODE_RUN;
    }
 
    bool base_use_case::has_resource_constraints() const
    {
      return _resource_constraints_.get() != nullptr;
    }
    
    const uc_resource_constraints & base_use_case::get_resource_constraints() const
    {
      DT_THROW_IF(!has_resource_constraints(), std::logic_error, "No resource constraints!");
      return *_resource_constraints_.get();
    }

    bool base_use_case::has_time_constraints() const
    {
      return _time_constraints_.get() != nullptr;
    }

    const uc_time_constraints & base_use_case::get_time_constraints() const
    {
      DT_THROW_IF(!has_time_constraints(), std::logic_error, "No time constraints!");
      return *_time_constraints_.get();
    }

    // virtual
    std::shared_ptr<uc_resource_constraints> base_use_case::_build_resource_constraints_() const
    {
      return std::make_shared<uc_resource_constraints>();
    }

    // virtual
    std::shared_ptr<uc_time_constraints> base_use_case::_build_time_constraints_() const
    {
      return std::make_shared<uc_time_constraints>();
    }

    void base_use_case::build_composition()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_CONFIG_COMPO,
                  std::logic_error,
                  "Composition description is not set!");
     
      const uc_composition_description & comp_desc = this->get_composition_description();
      // Compute the list of daughters (ordered):
      std::vector<std::string> names;
      comp_desc.build_daughter_names(names);
      for (const std::string & dname : names) {
        const uc_composition_description::daughter_entry & de
          = comp_desc.get_daughter(dname);
        uc_construction_context_type child_cc;
        child_cc.path = _cc_.path.make_child(dname); 
        child_cc.model_id = de.model_id;
        child_cc.description = de.description;
        child_cc.factory = _cc_.factory;
        child_cc.devices = _cc_.devices;
        child_cc.resources = _cc_.resources;
        DT_LOG_DEBUG(get_logging_priority(),
                     "Instantiating daughter use case '" << dname << "'...");
        base_use_case::composition_entry_type ce;
        ce.name        = dname;
        ce.description = de.description;
        ce.serial_rank = de.serial_rank;
        ce.from_setup  = de.from_setup;
        // XXX: Dry run
        if (this->is_dry_run_mode()) {
          ce.instance = _cc_.factory->create_dry(child_cc);
        } else {
          DT_THROW(std::logic_error, "No support for 'run' mode!");
        }
        this->_composition_[ce.name] = ce;     
      }
      _construct_stage_ = CONSTRUCT_COMPOSED;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
    
    void base_use_case::post_composition_configure_setup()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_COMPOSED,
                  std::logic_error,
                  "Scope resources constraints is not set!");
      
      _at_post_composition_configure_setup_(_configuration_);
      
      _construct_stage_ = CONSTRUCT_CONFIG_SETUP_POST_COMPO;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
                        
    void base_use_case::post_composition_deconfigure_setup()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // DT_THROW_IF(_construct_stage_ != CONSTRUCT_SCOPED,
      //             std::logic_error,
      //             "Scoped resources constraints is not set!");

      // // _at_post_composition_deconfigure_setup_();
      
      // _construct_stage_ = CONSTRUCT_POST_COMPO_CONFIG_SETUP;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
 
    // virtual
    void base_use_case::_at_post_composition_configure_setup_(const datatools::properties & config_)
    {
      return;
    }
   
    // virtual
    void base_use_case::_at_post_composition_deconfigure_setup_()
    {
      return;
    }

    const datatools::properties & base_use_case::get_configuration() const
    {
      return _configuration_;
    }

    void base_use_case::deconfigure_setup()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (is_run_mode()) {
        _rc_.reset();
      }
      
      // User hook:
      _at_deconfigure_setup_();

      _configuration_.reset();
 
      this->datatools::enriched_base::reset();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void base_use_case::configure_setup(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_READY,
                  std::logic_error,
                  "Ready construction stage is not set!");

      this->datatools::enriched_base::initialize(config_, false);
      if (is_run_mode()) {
        DT_THROW_IF(!has_rc(), std::logic_error, "No run control is set!");
        _rc_->set_run_stage(running::RUN_STAGE_READY);
      }

      _configuration_ = config_;

      // User hook:
      _at_configure_setup_(_configuration_);
      
      _construct_stage_ = CONSTRUCT_CONFIG_SETUP;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void base_use_case::_at_configure_setup_(const datatools::properties & config_)
    {
      
      return;
    }

    void base_use_case::_at_deconfigure_setup_()
    {
      
      return;
    }
     
      // _resource_constraints_.reset();
      // _time_constraints_.reset();
      // _mount_points_.clear();
      // _composition_.clear();
 
    void base_use_case::deconfigure_composition()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_CONFIG_COMPO,
                  std::logic_error,
                  "Composition description stage is not set!");

      _composition_description_.reset();

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    // virtual
    void base_use_case::_build_composition_description_(uc_composition_description & type_comp_desc_) const
    {
      return;
    }

    void base_use_case::_merge_composition_description_setup_(const uc_composition_description & setup_comp_desc_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (setup_comp_desc_.has_daughters()) {
        DT_THROW_IF(!setup_comp_desc_.has_scheduling(),
                    std::logic_error,
                    "Use case setup composition has no defined scheduling!");
        DT_LOG_DEBUG(get_logging_priority(), "Merging the use case setup composition with the type composition...");
        DT_THROW_IF(_composition_description_.can_merge(setup_comp_desc_.get_scheduling()),
                    std::logic_error,
                    "Cannot merge use case setup composition with the type composition!");
        bool from_setup = true;
        _composition_description_.merge(setup_comp_desc_, from_setup);
        _composition_description_.lock();
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void base_use_case::configure_composition(const uc_composition_description & setup_comp_desc_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(_construct_stage_ != CONSTRUCT_CONFIG_SETUP,
                  std::logic_error,
                  "Setup construction stage is not set!");
     
      // User hook:
      _build_composition_description_(_composition_description_);

      // System call:
      _merge_composition_description_setup_(setup_comp_desc_);

      _construct_stage_ = CONSTRUCT_CONFIG_COMPO;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
     
    const uc_composition_description & base_use_case::get_composition_description() const
    {
      return _composition_description_;
    }
  
    // void base_use_case::_basic_finalize_()
    // {
    //   DT_LOG_TRACE_ENTERING(get_logging_priority());
    //   if (is_run_mode()) {
    //     _rc_.reset();
    //   }
    //   _resource_constraints_.reset();
    //   _time_constraints_.reset();
    //   _mount_points_.clear();
    //   _composition_.clear();
    //   _composition_description_.reset();

    //   this->datatools::enriched_base::reset();
    //   DT_LOG_TRACE_EXITING(get_logging_priority());
    //   return;
    // }

    /* Actions */

    running::run_stage_completion base_use_case::run_prepare()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!_rc_->is_run_stage(running::RUN_STAGE_READY),
                  std::logic_error,
                  "Use case is not ready for running!");
      running::run_stage_completion completion;
      DT_LOG_TRACE(get_logging_priority(), "Entering try block...");
      try {
        _rc_->set_run_stage(running::RUN_STAGE_SYSTEM_PREPARING);
        completion.run_stage = _rc_->get_run_stage();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        _at_run_prepare_();
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        _rc_->set_run_stage(running::RUN_STAGE_SYSTEM_PREPARED);
      } catch (uc_run_system_error & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "vire::cmsserver::uc_run_system_error";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }
   
    running::run_stage_completion base_use_case::run_up()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(_rc_->get_run_depth() == running::RUN_DEPTH_SYSTEM,
                  std::logic_error,
                  "Use case does not allow this running path at runnning depth '"
                  << run_depth_label(_rc_->get_run_depth()) << "'!");
      DT_THROW_IF(!_rc_->is_run_stage(running::RUN_STAGE_SYSTEM_PREPARED),
                    std::logic_error,
                  "Use case does not allow this running path at runnning depth '"
                  << run_depth_label(_rc_->get_run_depth()) << "'!");
      running::run_stage_completion completion;
      try {
        _rc_->set_run_stage(running::RUN_STAGE_FUNCTIONAL_UP_RUNNING);
        completion.run_stage = _rc_->get_run_stage();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        _at_run_up_();
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        _rc_->set_run_stage(running::RUN_STAGE_FUNCTIONAL_UP_DONE);
      } catch (uc_run_work_error & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "vire::cmsserver::uc_work_error";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }
    running::run_stage_completion base_use_case::run_terminate()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (_rc_->get_run_depth() == running::RUN_DEPTH_SYSTEM) {
        DT_THROW_IF(!_rc_->is_run_stage(running::RUN_STAGE_SYSTEM_PREPARED),
                    std::logic_error,
                    "Use case does not allow this running path at runnning depth '"
                    << run_depth_label(_rc_->get_run_depth()) << "'!");
      } else {
         DT_THROW_IF(!_rc_->is_run_stage(running::RUN_STAGE_FUNCTIONAL_DOWN_DONE),
                    std::logic_error,
                    "Use case does not allow this running path at runnning depth '"
                    << run_depth_label(_rc_->get_run_depth()) << "'!");
      }
      running::run_stage_completion completion;
      try {
        _rc_->set_run_stage(running::RUN_STAGE_SYSTEM_TERMINATING);
        completion.run_stage = _rc_->get_run_stage();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        _at_run_terminate_();
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        _rc_->set_run_stage(running::RUN_STAGE_SYSTEM_TERMINATED);
       } catch (uc_run_system_error & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "vire::cmsserver::uc_run_system_error";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }

    running::run_stage_completion base_use_case::run_work()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(_rc_->get_run_depth() != running::RUN_DEPTH_WORK,
                  std::logic_error,
                  "Use case does not allow this running path at runnning depth '"
                  << run_depth_label(_rc_->get_run_depth()) << "'!");
      DT_THROW_IF(!_rc_->is_run_stage(running::RUN_STAGE_FUNCTIONAL_UP_DONE),
                  std::logic_error,
                  "Use case does not allow this running path at runnning depth '"
                  << run_depth_label(_rc_->get_run_depth()) << "'!");      
      running::run_stage_completion completion;
      try {
        _rc_->set_run_stage(running::RUN_STAGE_FUNCTIONAL_WORK_RUNNING);
        completion.run_stage = _rc_->get_run_stage();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        // LOOP STAT INIT/RESET
        DT_LOG_TRACE(get_logging_priority(), "Starting loop...");
        running::run_work_loop_status_type continue_flag = running::RUN_WORK_LOOP_CONTINUE;
        while (continue_flag == running::RUN_WORK_LOOP_CONTINUE
               && !_rc_->check_run_stop_requested()) {
          DT_LOG_TRACE(get_logging_priority(), "Loop...");
          _rc_->increment_run_work_loop_counter();

          // LOOP STAT LAST BEGIN
          //_run_work_begin_(),

          continue_flag = _at_run_work_loop_iteration_();
          
          //_run_work_end_(),
          // LOOP STAT LAST END
          // LOOP STAT UPDATE
        }
        // (LOOP STAT FINI)
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        _rc_->set_run_stage(running::RUN_STAGE_FUNCTIONAL_WORK_DONE);
      } catch (uc_run_work_error & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "vire::cmsserver::uc_work_error";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }

    running::run_stage_completion base_use_case::run_down()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (_rc_->get_run_depth() == running::RUN_DEPTH_AUTO) {
        DT_THROW_IF(!_rc_->is_run_stage(running::RUN_STAGE_FUNCTIONAL_UP_DONE),
                    std::logic_error,
                    "Use case does not allow this running path at runnning depth '"
                    << run_depth_label(_rc_->get_run_depth()) << "'!");
      } else if (_rc_->get_run_depth() == running::RUN_DEPTH_WORK) {
        DT_THROW_IF(!_rc_->is_run_stage(running::RUN_STAGE_FUNCTIONAL_WORK_DONE),
                    std::logic_error,
                    "Use case does not allow this running path at runnning depth '"
                    << run_depth_label(_rc_->get_run_depth()) << "'!");
      } else {
        DT_THROW(std::logic_error,
                 "Use case does not allow this running path at runnning depth '"
                 << run_depth_label(_rc_->get_run_depth()) << "'!");
      }
      running::run_stage_completion completion;
      try {
        DT_THROW_IF(!_rc_->is_run_stage(running::RUN_STAGE_FUNCTIONAL_WORK_DONE),
                    std::logic_error,
                    "Functional work is not done!");
        _rc_->set_run_stage(running::RUN_STAGE_FUNCTIONAL_DOWN_RUNNING);
        completion.run_stage = _rc_->get_run_stage();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        _at_run_down_();
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        _rc_->set_run_stage(running::RUN_STAGE_FUNCTIONAL_DOWN_DONE);
      } catch (uc_run_work_error & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "vire::cmsserver::uc_work_error";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }


    void base_use_case::_at_run_prepare_()
    {
      return;
    }

    void base_use_case::_at_run_terminate_()
    {
      return;
    }

    void base_use_case::_at_run_up_()
    {
      return;
    }

    void base_use_case::_at_run_down_()
    {
      return;
    }

    running::run_work_loop_status_type
    base_use_case::_at_run_work_loop_iteration_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_work_loop_status_type ret = running::RUN_WORK_LOOP_STOP;
      // Does nothing but ask for working loop stop...
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return ret;
    }

    // virtual
    void base_use_case::print_tree(std::ostream & out_,
                                   const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      datatools::enriched_base::tree_dump(out_, popts.title, popts.indent, true);
 
      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Construction stage : '" << to_string(_construct_stage_) << "'"
           << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Run mode : '" << run_mode_to_label(_run_mode_) << "'"
           << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Configuration : ";
      if (_configuration_.empty()) {
        out_ << "<empty>";
      }
      out_ << std::endl;
      if (!_configuration_.empty()) {
        std::ostringstream indent2ss;
        indent2ss << popts.indent  << datatools::i_tree_dumpable::skip_tag;
        _configuration_.tree_dump(out_, "", indent2ss.str());
      }
    
      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Composition description : ";
      if (_composition_description_.is_empty()) {
        out_ << "<empty>";
      }
      out_ << std::endl;
      if (!_composition_description_.is_empty()) {
        boost::property_tree::ptree popts2;
        std::ostringstream indentss2;
        indentss2 << popts.indent << skip_tag;
        popts2.put(datatools::i_tree_dumpable::base_print_options::indent_key(),
                   indentss2.str());
        _composition_description_.print_tree(out_, popts2);
      }
   
   
      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Composition : ";
      if (_composition_.size() == 0) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (_composition_.size()) {
        std::size_t count = 0;
        for (composition_dict_type::const_iterator i = _composition_.begin();
             i != _composition_.end();
             i++) {
          const composition_entry_type & daughter_entry = i->second;
          out_ << popts.indent << datatools::i_tree_dumpable::skip_tag;
          if (++count == _composition_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Daughter use case : '" << i->first << "'";
          if (i->second.serial_rank >= 0) {
            out_ << " (serial rank #" << i->second.serial_rank << ')';
          }
          out_ << std::endl;
        }
      }
      
      /*
      if (! _resource_requirements_.is_empty()) {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional resource requirements  : ";
        out_ << std::endl;
        {
          boost::property_tree::ptree popts2;
          popts2.put(datatools::i_tree_dumpable::base_print_options::indent_key(),
                     popts.indent + datatools::i_tree_dumpable::tags::skip_item());
          _resource_requirements_.print_tree(out_, popts2);
        }

        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Resource/device mount points : ";
        out_ << std::endl;
        size_t count = 0;
        for (const auto fmp : _mount_points_) {
          out_ << popts.indent << datatools::i_tree_dumpable::skip_tag;
          if (++count == _mount_points_.size()) {
            out_ << datatools::i_tree_dumpable::last_tag; 
          } else {
            out_ << datatools::i_tree_dumpable::tag; 
          }
          out_ << "Mount point '" << fmp.first << "'"
               << " with path '" << fmp.second << "'"
               << std::endl;
        }
      }
      */
      
      if (has_rc() ) {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Run stage    : '" << running::run_stage_label(_rc_->get_run_stage()) << "'"
             << std::endl;
      }

      if (! popts.inherit) {
        out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
             << "End.";
        out_ << std::endl;
      }

      return;
    }
    
  } // namespace cmsserver

} // namespace vire
