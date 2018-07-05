//! \file vire/cmsserver/uc_parametrised_resource_specifications.cc
//
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/uc_parametrised_resource_specifications.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/multi_properties.h>

namespace vire {

  namespace cmsserver {

    uc_parametrised_resource_specifications::resource_spec_entry::resource_spec_entry()
    {
      return;
    }

    // virtual
    uc_parametrised_resource_specifications::resource_spec_entry::~resource_spec_entry()
    {
      return;
    }

    bool uc_parametrised_resource_specifications::resource_spec_entry::is_complete() const
    {
      return is_resource() || is_device();
    }
 
    bool uc_parametrised_resource_specifications::resource_spec_entry::is_resource() const
    {
      return category == CATEGORY_RESOURCE;
    }
                                
    bool uc_parametrised_resource_specifications::resource_spec_entry::is_device() const
    {
      return category == CATEGORY_DEVICE;
    }

    bool uc_parametrised_resource_specifications::resource_spec_entry::has_device_type() const
    {
      return device_type_id != vire::device::TYPE_UNDEFINED;
    }

    bool uc_parametrised_resource_specifications::resource_spec_entry::has_device_model_id() const
    {
      return !device_model_id.empty();
    }

    bool uc_parametrised_resource_specifications::resource_spec_entry::has_resource_name() const
    {
      return !resource_name.empty();
    }

    bool uc_parametrised_resource_specifications::resource_spec_entry::has_resource_category() const
    {
      return resource_category != vire::resource::RESOURCE_CATEGORY_INVALID;
    }

    bool uc_parametrised_resource_specifications::resource_spec_entry::has_resource_access() const
    {
      return resource_access != vire::resource::ACCESS_INVALID;
    }

    bool uc_parametrised_resource_specifications::resource_spec_entry::has_resource_datatype_id() const
    {
      return resource_datatype_id != datatools::introspection::DATA_TYPE_INVALID;
    }

    bool uc_parametrised_resource_specifications::resource_spec_entry::has_resource_unit_dimension() const
    {
      return !resource_unit_dimension.empty();
    }

    std::tuple<bool,std::string>
    uc_parametrised_resource_specifications::resource_spec_entry::match(const resource_spec_entry & other_) const
    {
      if (this->is_device()) {
        
        if (!other_.is_device()) {
          return std::make_tuple(false, "Device cannot match to a non-device!");
        }
        
        // Check device model IDs:
        if (other_.has_device_model_id()
            && this->has_device_model_id()
            && (other_.device_model_id != this->device_model_id)) {
          std::string error_msg
            = std::string("Entries are not devices of the same model ('")
            + this->device_model_id + "' vs '"
            + other_.device_model_id + "')!";          
          return std::make_tuple(false, error_msg);
        } // model IDs
      
        // Check device types:
        if (other_.has_device_type()
            && this->has_device_type()
            && this->device_type_id != vire::device::TYPE_GENERIC
            && other_.device_type_id != vire::device::TYPE_GENERIC
            && (other_.device_type_id != this->device_type_id)) {
          std::string error_msg
            = std::string("Entries are not devices of the same type id ('")
            + vire::device::get_device_type_label(this->device_type_id) + "' vs '"
            + vire::device::get_device_type_label(other_.device_type_id) + "'!";          
          return std::make_tuple(false, error_msg);
        } // device types
        
      } else if (this->is_resource()) {
        
        if (!other_.is_resource()) {
          return std::make_tuple(false, "Resource cannot match to a non-resource!");
        }

        // Check resource names:
        if (other_.has_resource_name()
            && this->has_resource_name()
            && (other_.resource_name != this->resource_name)) {
          std::string error_msg
            = std::string("Entries are not resources of the same name ('")
            + this->resource_name + "' vs '"
            + other_.resource_name + "')!";          
          return std::make_tuple(false, error_msg);
        } // names
 
        // Check resource category:
        if (other_.has_resource_category()
            && this->has_resource_category()
            && (other_.resource_category != this->resource_category)) {
          std::string error_msg
            = std::string("Entries are not resources of the same category ('")
            + vire::resource::resource_category_to_label(this->resource_category) + "' vs '"
            + vire::resource::resource_category_to_label(other_.resource_category) + "'))!";          
          return std::make_tuple(false, error_msg);
        } // category
 
        // Check resource access:
        if (other_.has_resource_access()
            && this->has_resource_access()
            && (other_.resource_access != this->resource_access)) {
          std::string error_msg
            = std::string("Entries are not resources of the same access ('")
            + vire::resource::access_to_label(this->resource_access) + "' vs '"
            + vire::resource::access_to_label(other_.resource_access) + "')!";          
          return std::make_tuple(false, error_msg);
        } // access
        
        // Check if both resource data types are compatible:
        if (other_.has_resource_datatype_id()
            && this->has_resource_datatype_id()) {
          
          if (other_.resource_datatype_id != this->resource_datatype_id) {
            std::string error_msg
              = std::string("Entries are not resources with the same data types ('")
              + datatools::introspection::to_string(this->resource_datatype_id) + "' vs '"
              + datatools::introspection::to_string(other_.resource_datatype_id) + "')!"; 
            return std::make_tuple(false, error_msg);
          }
          
          // Check if both resource unit dimension are compatible:
          if (other_.has_resource_unit_dimension()
              && this->has_resource_unit_dimension()
              && (other_.resource_unit_dimension != this->resource_unit_dimension)) {
            std::string error_msg
              = std::string("Entries are not resources with the same unit dimensions ('")
              + this->resource_unit_dimension + "' vs '"
              + other_.resource_unit_dimension + "')!";
            return std::make_tuple(false, error_msg);
          }
        } // datatype
        
      } else {
        return std::make_tuple(false, "Undefined specification category!");
      }
      
      return std::make_tuple(true, "");
    }

    void uc_parametrised_resource_specifications::resource_spec_entry::print_tree(std::ostream & out_,
                                                                                  const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }
     
      out_ << popts.indent << last_tag
           << "Category : ";
      if (category == CATEGORY_DEVICE) {
        out_ << "'device'";
      } else if (category == CATEGORY_RESOURCE) {
        out_ << "'resource'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      if (category == CATEGORY_DEVICE) {
        out_ << popts.indent << last_skip_tag << tag
             << "Device model ID : '" << device_model_id << "'" << std::endl;   
        out_ << popts.indent << last_skip_tag << last_tag
             << "Device type ID : '" << vire::device::get_device_type_label(device_type_id) << "'" << std::endl;
      }
      
      if (category == CATEGORY_RESOURCE) {
        out_ << popts.indent << last_skip_tag << tag
             << "Resource name : '" << resource_name << "'" << std::endl;
         out_ << popts.indent << last_skip_tag << tag
             << "Resource category : '" << vire::resource::resource_category_to_label(resource_category) << "'" << std::endl;
        out_ << popts.indent << last_skip_tag << tag
             << "Resource access : '" << vire::resource::access_to_label(resource_access) << "'" << std::endl;  
        out_ << popts.indent << last_skip_tag << tag
             << "Resource data type : '" << datatools::introspection::to_string(resource_datatype_id) << "'" << std::endl;     
        out_ << popts.indent << last_skip_tag << last_tag
             << "Resource unit dimension : '" << resource_unit_dimension << "'" << std::endl;          
      }
      
      return;
    }

    uc_parametrised_resource_specifications::uc_parametrised_resource_specifications()
    {
      return;
    }

    uc_parametrised_resource_specifications::~uc_parametrised_resource_specifications()
    {
      return;
    }
    
    void uc_parametrised_resource_specifications::reset()
    {
      _locked_ = false;
      clear();
      return;
    }
 
    bool uc_parametrised_resource_specifications::is_empty() const
    {
      return _specs_.size() == 0;
    }

    std::size_t uc_parametrised_resource_specifications::size() const
    {
      return _specs_.size();
    }

    bool uc_parametrised_resource_specifications::has(const std::string & key_) const
    {
      return _specs_.count(key_);
    }

    void uc_parametrised_resource_specifications::build_keys(std::set<std::string> & keys_) const
    {
      keys_.clear();
      for (auto p : _specs_) {
        keys_.insert(p.first);
      }
      return;
    }
    
    std::set<std::string> uc_parametrised_resource_specifications::keys() const
    {
      std::set<std::string> lps;
      build_keys(lps);
      return lps;
    }
    
    bool uc_parametrised_resource_specifications::is_resource(const std::string & key_) const
    {
      spec_dict_type::const_iterator found = _specs_.find(key_);
      DT_THROW_IF(found == _specs_.end(), std::logic_error, "No entry with local path '" << key_ << "'!");
      return found->second->category == CATEGORY_RESOURCE;
    }
      
    bool uc_parametrised_resource_specifications::is_device(const std::string & key_) const
    {
      spec_dict_type::const_iterator found = _specs_.find(key_);
      DT_THROW_IF(found == _specs_.end(), std::logic_error, "No entry with local path '" << key_ << "'!");
      return found->second->category == CATEGORY_DEVICE;
    }
    
    void uc_parametrised_resource_specifications::_add_(const std::string & key_,
                                                        const resource_spec_entry & e_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Locked specifications!");
      static const uint32_t nvflags = datatools::NV_NO_COLON | datatools::NV_NO_DOT | datatools::NV_NO_HYPHEN | datatools::NV_LEADING_DIGIT;
      DT_THROW_IF(!datatools::name_validation(key_, nvflags),
                  std::logic_error,
                  "Resource specification local path '" << key_ << "' is not valid!");
      DT_THROW_IF(has(key_), std::logic_error, "Entry with local path '" << key_ << "' already exists!");
      DT_THROW_IF(e_.category == CATEGORY_UNDEF, std::logic_error, "Entry with local path '" << key_ << "' has no valid category!");
      _specs_[key_] = std::make_shared<resource_spec_entry>(e_);
      return;
    }
    
    void uc_parametrised_resource_specifications::add_device(const std::string & key_,
                                                             const std::string & device_model_id_,
                                                             const vire::device::device_type device_type_id_)
    {
      resource_spec_entry e;
      e.category = CATEGORY_DEVICE;
      if (!device_model_id_.empty()) {
        e.device_model_id = device_model_id_;
      }
      if (device_type_id_ != vire::device::TYPE_UNDEFINED) {
        e.device_type_id = device_type_id_;
      }
      _add_(key_, e);
      return;
    }

    void uc_parametrised_resource_specifications::add_resource(const std::string & key_,
                                                               const vire::resource::resource_category_type resource_category_,
                                                               const vire::resource::access_type resource_access_,
                                                               const datatools::introspection::data_type resource_datatype_id_,
                                                               const std::string & resource_unit_dimension_)
    {
      resource_spec_entry e;
      e.category = CATEGORY_RESOURCE;
      e.resource_category = resource_category_;
      e.resource_access = resource_access_;
      e.resource_datatype_id = resource_datatype_id_;
      if (!resource_unit_dimension_.empty()) {
        DT_THROW_IF(!datatools::introspection::is_real(e.resource_datatype_id),
                    std::logic_error,
                    "Explicit unit dimension '" << resource_unit_dimension_
                    << "' is only supported for real data types in resource requirement '" << key_ << "'!");
        e.resource_unit_dimension = resource_unit_dimension_;
      }
      _add_(key_, e);
      return;
    }

    void uc_parametrised_resource_specifications::remove(const std::string & key_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Locked specifications!");
      DT_THROW_IF(!has(key_), std::logic_error, "Entry with local path '" << key_ << "' does not exist!");
      _specs_.erase(key_);
      return;
    }

    const uc_parametrised_resource_specifications::resource_spec_entry &
    uc_parametrised_resource_specifications::get_specification(const std::string & key_) const
    {
      spec_dict_type::const_iterator found = _specs_.find(key_);
      DT_THROW_IF(found == _specs_.end(), std::logic_error, "No entry with local path '" << key_ << "'!");
      return *found->second.get();
    }

    const uc_parametrised_resource_specifications::spec_ptr_type &
    uc_parametrised_resource_specifications::get_specification_ptr(const std::string & key_) const
    {
      spec_dict_type::const_iterator found = _specs_.find(key_);
      DT_THROW_IF(found == _specs_.end(), std::logic_error, "No entry with local path '" << key_ << "'!");
      return found->second;
    }
 
    void uc_parametrised_resource_specifications::clear()
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Locked specifications!");
      _specs_.clear();
      return;
    }

    void uc_parametrised_resource_specifications::load(const datatools::properties & config_)
    {
      std::set<std::string> keys;
      if (config_.has_key("keys")) {
        config_.fetch("keys", keys);
      }

      for (const auto & key : keys) {
        resource_spec_entry entry;
        
        {
          std::string category_repr;
          std::ostringstream category_key_ss;
          category_key_ss << key << ".category";
          if (config_.has_key(category_key_ss.str())) {
            category_repr = config_.fetch_string(category_key_ss.str());
            if (category_repr == "device") {
              entry.category = CATEGORY_DEVICE;
            } else if (category_repr == "resource") {
              entry.category = CATEGORY_RESOURCE;
            } else {
              DT_THROW(std::logic_error, "Invalid resource specification category '" << category_repr << "' for requirement '" << key << "'!");
            }
          }
        }
        DT_THROW_IF(entry.category == CATEGORY_UNDEF,
                    std::logic_error,
                    "Missing resource specification category for requirement '" << key << "'!");

        // Device requirement:
        if (entry.category == CATEGORY_DEVICE) {
          {
            std::string device_type_repr;
            std::ostringstream device_type_key_ss;
            device_type_key_ss << key << ".device_type";
            if (config_.has_key(device_type_key_ss.str())) {
              device_type_repr = config_.fetch_string(device_type_key_ss.str());
              vire::device::device_type devId = vire::device::get_device_type(device_type_repr);
              DT_THROW_IF(devId == vire::device::TYPE_UNDEFINED,
                          std::logic_error,
                          "Invalid specification device type '" << device_type_repr
                          << "' for device requirement '" << key << "'!");
              entry.device_type_id = devId;
            }
          }

          {
            std::string device_model_repr;
            std::ostringstream device_model_key_ss;
            device_model_key_ss << key << ".device_model";
            if (config_.has_key(device_model_key_ss.str())) {
              device_model_repr = config_.fetch_string(device_model_key_ss.str());
              entry.device_model_id = device_model_repr;
            }
          }

          // At least one constraint must be given on the device to be mounted:
          DT_THROW_IF(entry.device_type_id == vire::device::TYPE_UNDEFINED
                      && entry.device_model_id.empty(),
                      std::logic_error,
                      "Missing device type/model for device requirement '" << key << "'!");
        }
        
        // Resource requirement:
        if (entry.category == CATEGORY_RESOURCE) {
          {
            std::string resource_cat_repr;
            std::ostringstream resource_cat_key_ss;
            resource_cat_key_ss << key << ".resource_category";
            if (config_.has_key(resource_cat_key_ss.str())) {
              resource_cat_repr = config_.fetch_string(resource_cat_key_ss.str());
              vire::resource::resource_category_type resCat = vire::resource::label_to_resource_category(resource_cat_repr);
              DT_THROW_IF(resCat == vire::resource::RESOURCE_CATEGORY_INVALID,
                          std::logic_error,
                          "Invalid resource category '" << resource_cat_repr
                          << "' for resource requirement '" << key << "'!");
              entry.resource_category = resCat;
            }
          }
          
          {
            std::string resource_access_repr;
            std::ostringstream resource_access_key_ss;
            resource_access_key_ss << key << ".access";
            if (config_.has_key(resource_access_key_ss.str())) {
              resource_access_repr = config_.fetch_string(resource_access_key_ss.str());
              vire::resource::access_type resAcc = vire::resource::label_to_access(resource_access_repr);
              DT_THROW_IF(resAcc == vire::resource::ACCESS_INVALID,
                          std::logic_error,
                          "Invalid resource access '" << resource_access_repr
                          << "' for resource requirement '" << key << "'!");
              entry.resource_access = resAcc;
            }
          }
         
          {
            std::string resource_datatype_repr;
            std::ostringstream resource_datatype_key_ss;
            resource_datatype_key_ss << key << ".datatype";
            if (config_.has_key(resource_datatype_key_ss.str())) {
              resource_datatype_repr = config_.fetch_string(resource_datatype_key_ss.str());
              datatools::introspection::data_type resDT = datatools::introspection::DATA_TYPE_INVALID;
              DT_THROW_IF(!datatools::introspection::from_string(resource_datatype_repr, resDT),
                          std::logic_error,
                          "Invalid resource data type '" << resource_datatype_repr
                          << "' for resource requirement '" << key << "'!");
              // DT_THROW_IF(resDT == datatools::introspection::DATA_TYPE_INVALID,
              //             std::logic_error,
              //             "Invalid resource data type '" << resource_datatype_repr
              //             << "' for resource requirement '" << key << "'!");
              entry.resource_datatype_id = resDT;
            }
          }
        
          {
            std::string resource_unitdim_repr;
            std::ostringstream resource_unitdim_key_ss;
            resource_unitdim_key_ss << key << ".unit_dimension";
            if (config_.has_key(resource_unitdim_key_ss.str())) {
              resource_unitdim_repr = config_.fetch_string(resource_unitdim_key_ss.str());
              entry.resource_unit_dimension = resource_unitdim_repr;
            }       
          }
        }
        
        _add_(key, entry);
      }
      
      return;
    }

    bool uc_parametrised_resource_specifications::is_locked() const
    {
      return _locked_;
    }

    void uc_parametrised_resource_specifications::lock()
    {
      _locked_ = true;
      return;
    }
    
    void uc_parametrised_resource_specifications::print_tree(std::ostream & out_,
                                                             const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      out_ << popts.indent << i_tree_dumpable::tag
           << "Number of resource specification entries : "
           << _specs_.size()
           << std::endl;

      if (!is_empty()) {
        out_ << popts.indent << i_tree_dumpable::tag
             << "Resource specifications : " << std::endl;
        size_t count = 0;
        for(const auto e : _specs_) {
          out_ << popts.indent << i_tree_dumpable::skip_tag;
          std::ostringstream indentss2;
          indentss2 << popts.indent << i_tree_dumpable::skip_tag;
          if (++count == _specs_.size()) {
            out_ << i_tree_dumpable::last_tag;
            indentss2 << i_tree_dumpable::last_skip_tag;
          } else {
            out_ << i_tree_dumpable::tag;
            indentss2 << i_tree_dumpable::skip_tag;
          }
          category_type cat = e.second->category;
          out_ << "Local path='" << e.first << "'" << " : ";
          out_ << std::endl;
          {
            boost::property_tree::ptree popts2;
            popts2.put(datatools::i_tree_dumpable::base_print_options::indent_key(), indentss2.str());
            e.second->print_tree(out_, popts2);
          }
        }
      }
      
      out_ << popts.indent << i_tree_dumpable::inherit_tag(popts.inherit)
           << "Locked : "
           << std::boolalpha
           << this->is_locked()
           << std::endl;
      
      return;
    }

  } // namespace cmsserver

} // namespace vire
