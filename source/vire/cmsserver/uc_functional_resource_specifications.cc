//! \file vire/cmsserver/uc_functional_resource_specifications.cc
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
#include <vire/cmsserver/uc_functional_resource_specifications.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/properties.h>

namespace vire {

  namespace cmsserver {
    
    uc_functional_resource_description::uc_functional_resource_description()
    {
      return;
    }

    // explicit
    uc_functional_resource_description::uc_functional_resource_description(const uc_resource_mount_link & mlink_)
    {
      set_mount_link(mlink_);
      return;
    }

    // explicit
    uc_functional_resource_description::uc_functional_resource_description(const vire::utility::path & path_)
    {
      set_absolute_path(path_);
      return;
    }

    // virtual
    uc_functional_resource_description::~uc_functional_resource_description()
    {
      return;
    }

    bool uc_functional_resource_description::is_complete() const
    {
      return is_absolute() || is_relative();
    }

    bool uc_functional_resource_description::is_absolute() const
    {
      return this->has_absolute_path();
    }

    bool uc_functional_resource_description::is_relative() const
    {
      return this->has_mount_link();
    }

    bool uc_functional_resource_description::has_absolute_path() const
    {
      return _absolute_path_.is_valid();
    }
     
    void uc_functional_resource_description::set_absolute_path(const vire::utility::path & abspath_)
    {
      DT_THROW_IF(!abspath_.is_valid(),
                  std::logic_error,
                  "Not a valid absolute path!");
      _absolute_path_ = abspath_;
      return;
    }

    const vire::utility::path & uc_functional_resource_description::get_absolute_path() const
    {
      return _absolute_path_;
    }
    
    bool uc_functional_resource_description::has_mount_link() const
    {
      return _mount_link_.is_complete();
    }
    
    void uc_functional_resource_description::set_mount_link(const uc_resource_mount_link & mlink_)
    {
      DT_THROW_IF(!mlink_.is_complete(), std::logic_error,
                  "Cannot set a incomplete mount link!")
      DT_THROW_IF(!mlink_.get_from_id().is_functional(), std::logic_error,
                  "Cannot set a mount link from a non-functional resource port '" << mlink_.to_string() << "'!")
      _mount_link_ = mlink_;
      return;
    }
 
    const uc_resource_mount_link & uc_functional_resource_description::get_mount_link() const
    {
      return _mount_link_;
    }
  
    void uc_functional_resource_description::configure(const datatools::properties & config_)
    {
      if (config_.has_key("absolute_path")) {
        std::string ap_repr = config_.fetch_string("absolute_path");
        vire::utility::path ap;
        DT_THROW_IF(!ap.from_string(ap_repr), std::logic_error, "Invalid format for absolute path '" << ap_repr << "'!");
        this->set_absolute_path(ap);
      } else {
        if (config_.has_key("mount_link")) {
          std::string mlink_repr = config_.fetch_string("mount_link");
          uc_resource_mount_link mlink;      
          DT_THROW_IF(!mlink.from_string(mlink_repr),
                      std::logic_error,
                      "Invalid format for functional resource mount link '" << mlink_repr << "'!");
          this->set_mount_link(mlink);
        }
      }
      return;
    }
 
    void uc_functional_resource_description::print_tree(std::ostream & out_,
                                                        const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      if (is_relative()) {
        out_ << popts.indent << tag
             << "Mount link : '" << _mount_link_.to_string() << "'"
             << std::endl;
      } else if (is_absolute()) {
        out_ << popts.indent << tag
             << "Absolute path : '" << _absolute_path_.to_string() << "'" << std::endl;          
      }
      
      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Completion: " << std::boolalpha << is_complete() << std::endl;          
      

      return;
    }

    uc_functional_resource_specifications:: uc_functional_resource_specifications()
    {
      return;
    }

    // virtual
    uc_functional_resource_specifications::~uc_functional_resource_specifications()
    {
      return;
    }
        
    void uc_functional_resource_specifications::reset()
    {
      _locked_ = false;
      clear();
      return;
    }

    bool uc_functional_resource_specifications::is_empty() const
    {
      return _specs_.size() == 0;
    }

    std::size_t uc_functional_resource_specifications::size() const
    {
      return _specs_.size();
    }

    bool uc_functional_resource_specifications::has(const std::string & key_) const
    {
      return _specs_.count(key_) > 0;
    }

    void uc_functional_resource_specifications::build_keys(std::set<std::string> & keys_) const
    {
      keys_.clear();
      for (auto p : _specs_) {
        keys_.insert(p.first);
      }
      return;
    }

    std::set<std::string> uc_functional_resource_specifications::keys() const
    {
      std::set<std::string> keys;
      build_keys(keys);
      return keys;
    }
       
    bool uc_functional_resource_specifications::is_absolute(const std::string & key_) const
    {
      spec_dict_type::const_iterator found = _specs_.find(key_);
      DT_THROW_IF(found == _specs_.end(), std::logic_error, "No functional resource with key '" << key_ << "'!");
      return found->second->is_absolute();
    }

    void uc_functional_resource_specifications::add_relative_resource(const std::string & key_,
                                                                      const uc_resource_mount_link & mlink_)
    {
      uc_functional_resource_description e;
      e.set_mount_link(mlink_);
      _add_(key_, e);
      return;
    }

    void uc_functional_resource_specifications::add_absolute_resource(const std::string & key_,
                                                                      const vire::utility::path & path_)
    {
      uc_functional_resource_description e;
      e.set_absolute_path(path_);
      _add_(key_, e);
      return;      
    }

    void uc_functional_resource_specifications::_add_(const std::string & key_,
                                                      const uc_functional_resource_description & frd_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Locked specifications!");
      static const uint32_t nvflags = datatools::NV_NO_COLON | datatools::NV_NO_DOT | datatools::NV_NO_HYPHEN | datatools::NV_LEADING_DIGIT;
      DT_THROW_IF(!datatools::name_validation(key_, nvflags),
                  std::logic_error,
                  "Functional resource key '" << key_ << "' is not valid!");
      DT_THROW_IF(has(key_), std::logic_error, "Functional resource with key '" << key_ << "' already exists!");
      _specs_[key_] = std::make_shared<uc_functional_resource_description>(frd_);
      return;
    }

    /// Return the record associated to as existing functional resource description
    const uc_functional_resource_description & uc_functional_resource_specifications::get_description(const std::string & key_) const
    {
      spec_dict_type::const_iterator found = _specs_.find(key_);
      DT_THROW_IF(found == _specs_.end(), std::logic_error, "No functional resource with key '" << key_ << "'!");
      return *found->second.get();
    }
 
    void uc_functional_resource_specifications::clear()
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Locked specifications!");
      _specs_.clear();
      return;
    }

    /*
    void uc_functional_resource_specifications::load(const datatools::properties & config_)
    {
      std::set<std::string> keys;
      if (config_.has_key("keys")) {
        config_.fetch("keys", keys);
      }
    
      return;
    }
    */
    
    bool uc_functional_resource_specifications::is_locked() const
    {
      return _locked_;
    }

    void uc_functional_resource_specifications::lock()
    {
      _locked_ = true;
      return;
    }
    
    void uc_functional_resource_specifications::print_tree(std::ostream & out_,
                                                           const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      out_ << popts.indent << i_tree_dumpable::tag
           << "Number of functional resource descriptions : "
           << _specs_.size()
           << std::endl;
     
      out_ << popts.indent << i_tree_dumpable::inherit_tag(popts.inherit)
           << "Locked : "
           << std::boolalpha
           << this->is_locked()
           << std::endl;
      
      return;
    }

  } // namespace cmsserver

} // namespace vire
