// vire/cms/experiment_image_registry.cc - Resource image registry
//
// Copyright (c) 2015-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cms/experiment_image_registry.h>

// Third Party:
// - datatools:
#include <datatools/exception.h>
#include <datatools/logger.h>

// This project:
// #include <vire/device/manager.h>
// #include <vire/resource/manager.h>
#include <vire/cms/utils.h>
#include <vire/cms/resource_image.h>
#include <vire/cms/device_image.h>

namespace vire {

  namespace cms {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(experiment_image_registry, "vire::cms::experiment_image_registry");

    // static
    const std::string & experiment_image_registry::default_service_name()
    {
      static const std::string _n("expimage");
      return _n;
    }

    experiment_image_registry::experiment_image_registry(uint32_t flags_)
    {
      _initialized_ = false;
      _device_manager_ = nullptr;
      _resource_manager_ = nullptr;
      _set_defaults();
      return;
    }

    experiment_image_registry::~experiment_image_registry()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void experiment_image_registry::_set_defaults()
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _with_devices_ = false;
      _only_writable_resources_ = false;         
      return;
    }

    bool experiment_image_registry::get_with_devices() const
    {
      return _with_devices_;
    }

    void experiment_image_registry::set_with_devices(const bool wd_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Service is initialized!");
      _with_devices_ = wd_;
      return;
    }
    
    bool experiment_image_registry::get_only_writable_resources() const
    {
      return _only_writable_resources_;
    }

    void experiment_image_registry::set_only_writable_resources(const bool owr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Service is initialized!");
      _only_writable_resources_ = owr_;
      return;
    }
 
    bool experiment_image_registry::has(const std::string & path_) const
    {
      return _images_.count(path_) != 0;
    }

    bool experiment_image_registry::has_device(const std::string & path_) const
    {
      image_dict_type::const_iterator found = _images_.find(path_);
      if (found == _images_.end()) return false;
      return found->second->is_device();
    }

    bool experiment_image_registry::has_resource(const std::string & path_) const
    {
      image_dict_type::const_iterator found = _images_.find(path_);
      if (found == _images_.end()) return false;
      return found->second->is_resource();
    }

    const resource_image & experiment_image_registry::get_resource_image(const std::string & path_) const
    {
      experiment_image_registry * mutable_this = const_cast<experiment_image_registry *>(this);
      return mutable_this->grab_resource_image(path_);
    }

    resource_image & experiment_image_registry::grab_resource_image(const std::string & path_)
    {
      base_image & img = this->_grab_image_(path_);
      return dynamic_cast<resource_image&>(img);
    }

    const base_image & experiment_image_registry::get_image(const std::string & path_) const
    {
      experiment_image_registry * mutable_this = const_cast<experiment_image_registry *>(this);
      return mutable_this->_grab_image_(path_);
    }

    base_image & experiment_image_registry::grab_image(const std::string & path_)
    {
      return this->_grab_image_(path_);
    }

    base_image & experiment_image_registry::_grab_image_(const std::string & path_)
    {
      image_dict_type::const_iterator found = _images_.find(path_);
      DT_THROW_IF(found == _images_.end(), std::logic_error,
                  "No image at path '" << path_ << "'!");
      return *found->second;
    }

    bool experiment_image_registry::has_device_service_name() const
    {
      return !_device_service_name_.empty();
    }

    const std::string & experiment_image_registry::get_device_service_name() const
    {
      return _device_service_name_;
    }

    void experiment_image_registry::set_device_service_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Registry is already initialized!");
      _device_service_name_ = name_;
      return;
    }

    bool experiment_image_registry::has_resource_service_name() const
    {
      return !_resource_service_name_.empty();
    }

    const std::string & experiment_image_registry::get_resource_service_name() const
    {
      return _resource_service_name_;
    }

    void experiment_image_registry::set_resource_service_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Registry is already initialized!");
      _resource_service_name_ = name_;
      return;
    }

    bool experiment_image_registry::has_resource_manager() const
    {
      return _resource_manager_ != nullptr;
    }

    void experiment_image_registry::set_resource_manager(const vire::resource::manager & mgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Registry is already initialized!");
      _resource_manager_ = &mgr_;
      return;
    }

    const vire::resource::manager & experiment_image_registry::get_resource_manager() const
    {
      DT_THROW_IF(!has_resource_manager(), std::logic_error, "Registry has no resource manager!");

      return *_resource_manager_;
    }

    bool experiment_image_registry::has_device_manager() const
    {
      return _device_manager_ != nullptr;
    }

    void experiment_image_registry::set_device_manager(const vire::device::manager & mgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Registry is already initialized!");
      _device_manager_ = &mgr_;
      return;
    }

    const vire::device::manager & experiment_image_registry::get_device_manager() const
    {
      DT_THROW_IF(!has_device_manager(), std::logic_error, "Registry has no device manager!");

      return *_device_manager_;
    }

    bool experiment_image_registry::is_initialized() const
    {
      return _initialized_;
    }

    int experiment_image_registry::initialize(const datatools::properties & config_,
                                              datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error, "Registry is already initialized!");

      this->::datatools::base_service::common_initialize(config_);

      if (!has_device_manager()) {
        if (_device_service_name_.empty()) {
          std::string device_service_name;
          if (config_.has_key("devices_service")) {
            device_service_name = config_.fetch_string("devices_service");
          }
          if (!has_device_service_name()) {
            // Automatically pickup the first device manager from the service manager:
            datatools::find_service_name_with_id(service_dict_,
                                                 "vire::device::manager",
                                                 device_service_name);
          }
          set_device_service_name(device_service_name);
        }
        DT_THROW_IF(!has_device_service_name(),
                    std::logic_error,
                    "No device manager service name is set!");
        const vire::device::manager & device_mgr
          = datatools::get<vire::device::manager>(service_dict_, _device_service_name_);
        set_device_manager(device_mgr);
      }

      DT_THROW_IF(!has_device_manager(), std::logic_error, "No device manager is set!");

      if (!has_resource_manager()) {
        if (_resource_service_name_.empty()) {
          std::string resource_service_name;
          if (config_.has_key("resources_service")) {
            resource_service_name = config_.fetch_string("resources_service");
          }
          if (!has_resource_service_name()) {
            // Automatically pickup the first device manager from the service manager:
            datatools::find_service_name_with_id(service_dict_,
                                                 "vire::resource::manager",
                                                 resource_service_name);
          }
          set_resource_service_name(resource_service_name);
        }
        DT_THROW_IF(!has_resource_service_name(),
                    std::logic_error,
                    "No resource manager service name is set!");
        const vire::resource::manager & resource_mgr
          = datatools::get<vire::resource::manager>(service_dict_, _resource_service_name_);
        set_resource_manager(resource_mgr);
      }

      DT_THROW_IF(!has_resource_manager(), std::logic_error, "No resource manager is set!");

      if (config_.has_key("only_writable_resources")) {
        bool owr = config_.fetch_boolean("only_writable_resources");
        set_only_writable_resources(owr);
      }

      if (config_.has_key("with_devices")) {
        bool wd = config_.fetch_boolean("with_devices");
        set_with_devices(wd);
      }
      
      _initialized_ = true;
      return 0;
    }

    int experiment_image_registry::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Registry is not initialized!");
      _initialized_ = false;
      _images_.clear();
      _resource_manager_ = nullptr;
      _device_manager_ = nullptr;
      _device_service_name_.clear();
      _resource_service_name_.clear();
      _set_defaults();
      return 0;
    }
     
    // void experiment_image_registry::add_device_image(const std::string & path_)
    // {
    //   DT_THROW_IF(!is_initialized(), std::logic_error, "Image registry is not initialized!");

    //   return;
    // }
    
   std::tuple<bool, std::string> experiment_image_registry::add_resource_image(const std::string & path_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Image registry is not initialized!");
      if (! _resource_manager_->has_resource_by_path(path_)) {
        return std::make_tuple(false, "No resource '" + path_ + "'");
      }
      const vire::resource::resource & res = _resource_manager_->get_resource_by_path(path_);
      if (_only_writable_resources_ && res.get_access() == vire::utility::RW_READABLE) {
        return std::make_tuple(false, "Read only resource '" + path_ + "' is not accepted!");   
      }
      _images_[path_] = std::make_shared<resource_image>(res);
      return std::make_tuple(true, "");
    }

    const experiment_image_registry::image_dict_type & experiment_image_registry::get_images() const
    {
      return _images_;
    }

    /*
    void experiment_image_registry::_insert_device_image(const std::string & path_,
                                                         const vire::device::logical_device & log_)
    {
      {
        image dev_img(path_, log_);
        _images_[path_] = dev_img;
      }
      image & dimg = _images_.find(path_)->second;
      std::string exp;
      std::string parent_name;
      std::string leaf_name;
      DT_THROW_IF(!vire::utility::path::extract(path_, exp, parent_name, leaf_name),
                  std::logic_error,
                  "Invalid device path '" << path_ << "'!");
      dimg.set_leaf_name(leaf_name);
      std::string parent_path;
      vire::utility::path::build(exp, parent_name, "", parent_path);
      // = exp + vire::device::mapping::setup_path_sep() + parent_name;
      if (_images_.find(parent_path) == _images_.end()) {
        // If parent is not yet set:
        const vire::device::mapping & mpg = _device_manager_->get_mapping();
        const vire::device::mapping_info & minfo = mpg.get_mapping_info(parent_path);
        DT_THROW_IF(!minfo.is_device(),
                    std::logic_error,
                    "Mapped object '" << parent_path << "' is not a device!");
        DT_THROW_IF(!minfo.has_logical_device(),
                    std::logic_error,
                    "Mapped device '" << parent_path << "' has no logical device!");
        _insert_device_image(parent_path, minfo.get_logical_device());
      }
      dimg.set_parent(_images_.find(parent_path)->second, leaf_name);
      return;
    }
    */

    /*
    void experiment_image_registry::_insert_resource_image(const vire::resource::resource & r_)
    {
      {
        image res_img(r_);
        _images_[r_.get_path()] = res_img;
      }
      image & rimg = _images_.find(r_.get_path())->second;
      std::string exp;
      std::string parent_name;
      std::string leaf_name;
      DT_THROW_IF(!vire::utility::path::extract(r_.get_path(), exp, parent_name, leaf_name),
                  std::logic_error,
                  "Invalid resource path '" << path_ << "'!");
      // DT_THROW_IF(!split_path(r_.get_path(), exp, parent_name, leaf_name),
      //             std::logic_error,
      //             "Invalid resource path '" << r_.get_path() << "'!");
      rimg.set_leaf_name(leaf_name);
      // std::string parent_path = exp + vire::device::mapping::setup_path_sep() + parent_name;
      std::string parent_path;
      vire::utility::path::build(exp, parent_name, "", parent_path);
      if (_images_.find(parent_path) == _images_.end()) {
        // If parent is not yet set:
        const vire::device::mapping & mpg = _device_manager_->get_mapping();
        const vire::device::mapping_info & minfo = mpg.get_mapping_info(parent_path);
        DT_THROW_IF(!minfo.is_device(),
                    std::logic_error,
                    "Mapped object '" << parent_path << "' is not a device!");
        DT_THROW_IF(!minfo.has_logical_device(),
                    std::logic_error,
                    "Mapped device '" << parent_path << "' has no logical device!");
        _insert_device_image(parent_path, minfo.get_logical_device());
      }
      rimg.set_parent(_images_.find(parent_path)->second, leaf_name);
      return;
    }
    */
    
    // void experiment_image_registry::_build_images()
    // {
    //   const std::set<int32_t> ids = _resource_manager_->get_set_of_resource_ids() ;
    //   for (auto id : ids) {
    //     const vire::resource::resource & r = _resource_manager_->get_resource_by_id(id);
    //     _insert_resource_image(r);
    //   }
    //   return;
    // }

    void experiment_image_registry::print_tree(std::ostream & out_,
                                               const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      this->datatools::base_service::tree_dump(out_, popts.title, popts.indent, true);

      out_ << popts.indent << tag
           << "Device service name : '" << _device_service_name_ << "'" << std::endl;

      out_ << popts.indent << tag
           << "Device manager : [@" << _device_manager_ << ']' << std::endl;

      out_ << popts.indent << tag
           << "Resource service name : '" << _resource_service_name_ << "'" << std::endl;

      out_ << popts.indent << tag
           << "Resource manager : [@" << _resource_manager_ << ']' << std::endl;

      out_ << popts.indent << tag
           << "With devices : " << std::boolalpha << _with_devices_ << std::endl;

      out_ << popts.indent << tag
           << "Only writable resources : " << std::boolalpha << _only_writable_resources_ << std::endl;

      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Images : " << _images_.size() << std::endl;

      {
        std::size_t nimg = _images_.size();
        for (image_dict_type::const_iterator img_iter = _images_.begin();
             img_iter != _images_.end();
             img_iter++) {
          image_dict_type::const_iterator img_jter = img_iter;
          img_jter++;
          out_ << popts.indent << inherit_skip_tag(popts.inherit);
          if (img_jter == _images_.end()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << img_iter->first << " : " << img_iter->second->get_monitoring();
          out_ << std::endl;
        }
      }

      return;
    }

  } // namespace cms

} // namespace vire
