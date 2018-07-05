// ihs_handler.cc
//
// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of datatools.
//
// datatools is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option any later version.
//
// datatools is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with datatools. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/inspector/ihs_handler.h>

// Third Party:
// - Boost:
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/ui/utils.h>

// Vire library:
#include <vire/utility/path.h>
#include <vire/device/instance_info.h>

namespace vire {

  namespace inspector {

    bool ihs_handler::ui_is_root_path(const std::string & ui_path_) const
    {
      return ui_path_ == datatools::ui::path::root_path();
    }

    std::string ihs_handler::ui_parent_path(const std::string & ui_path_) const
    {
      return datatools::ui::path::remove_trailing_sep(datatools::ui::path::parent_path(ui_path_));
    }

    std::string ihs_handler::instance_to_ui_path(const std::string & dev_path_) const
    {
      std::string ui_path;
      ui_path = boost::replace_first_copy(dev_path_,
                                         _top_path_,
                                         datatools::ui::path::root_path());
      if (boost::starts_with(ui_path, "//")) {
        ui_path = ui_path.substr(1);
      }
      return ui_path;
    }

    std::string ihs_handler::ui_to_instance_path(const std::string & ui_path_) const
    {
      std::ostringstream dev_path_out;
      dev_path_out << _top_path_;
      if (!ui_is_root_path(ui_path_)) {
        dev_path_out << ui_path_;
      }
      return dev_path_out.str();
    }

    std::string ihs_handler::ui_leaf_path(const std::string & ui_path_) const
    {
      return datatools::ui::path::basename(ui_path_);
    }

    ihs_handler::ihs_handler()
    {
      _initialized_ = false;
      _logging_ = datatools::logger::PRIO_FATAL;
      _device_manager_ = nullptr;
      _tree_ = nullptr;
      return;
    }

    ihs_handler::~ihs_handler()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    datatools::logger::priority ihs_handler::get_logging() const
    {
      return _logging_;
    }

    void ihs_handler::set_logging(datatools::logger::priority p_)
    {
      _logging_ = p_;
      return;
    }

    bool ihs_handler::has_device_manager() const
    {
      return _device_manager_ != nullptr;
    }

    void ihs_handler::set_device_manager(const vire::device::manager & dev_mgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Device IHS handler is already initialized!");
      _device_manager_ = &dev_mgr_;
      return;
    }

    const vire::device::manager & ihs_handler::get_device_manager() const
    {
      DT_THROW_IF(!has_device_manager(), std::logic_error,
                  "Device IHS handler has no device manager!");
      return *_device_manager_;
    }

    const datatools::ui::ihs & ihs_handler::get_ihs() const
    {
      return *_ihs_;
    }

    datatools::ui::ihs & ihs_handler::grab_ihs()
    {
      return *_ihs_;
    }

    bool ihs_handler::is_initialized() const
    {
      return _initialized_;
    }

    void ihs_handler::initialize()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Device IHS handler is already initialized!");
      DT_THROW_IF(!has_device_manager(), std::logic_error,
                  "Device manager is not set!");
      DT_THROW_IF(!_device_manager_->is_initialized(), std::logic_error,
                  "Device manager is not initialized!");
      _init_();
      _construct_device_descriptors();
      _construct_ihs();

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void ihs_handler::reset()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Device IHS handler is not initialized!");
      _initialized_ = false;

      // Destroy the IHS:
      _destroy_ihs();
      _dev_interfaces_.clear();
      _dev_descriptors_.clear();
      _tree_ = nullptr;
      _device_manager_ = nullptr;
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void ihs_handler::_destroy_ihs()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      _ihs_.reset();
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void ihs_handler::_init_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      _tree_ = &_device_manager_->get_tree();
      _top_path_ = _tree_->get_parent_path();
      DT_LOG_DEBUG(_logging_, "Top level device path= '" << _top_path_ << "'");
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    // void ihs_handler::_construct_logical_device_descriptors(const std::string & path_,
    //                                                         const vire::device::logical_device & log_dev_)
    // {
    //   DT_LOG_TRACE_ENTERING(_logging_);
    //   /*
    //   const vire::device::logical_device::daughters_dict_type & daughters = log_dev_.get_daughters();
    //   std::vector<std::string> daughter_names;
    //   log_dev_.build_daughters_labels(daughter_names);
    //   for (const auto child : daughters) {
    //     const std::string & name = child.first;
    //     const vire::device::physical_device & child_phys = log_dev_.get_daughter(name);
    //   }
    //   */
    //   DT_LOG_TRACE_EXITING(_logging_);
    //   return;
    // }

    void ihs_handler::_construct_device_descriptors()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      const std::string & top_device_full_path = _tree_->get_parent_path();
      DT_LOG_DEBUG(_logging_, "Top device full path : '" << top_device_full_path << "'");
      const vire::device::instance_info & top_instance = _tree_->get_instance(_tree_->get_parent_path());
      DT_THROW_IF(!top_instance.has_logical(),
                  std::logic_error,
                  "Top device instance path is not associated to a logical device!");
      const vire::device::logical_device & top_dev_log = top_instance.get_logical_device();
      DT_LOG_DEBUG(_logging_, "Top logical device : '" << top_dev_log.get_name() << "'");
      std::string top_device_ui_path = instance_to_ui_path(top_device_full_path);
      DT_LOG_DEBUG(_logging_, "Top device UI path : '" << top_device_ui_path << "'");
      // device_descriptor_ptr ddescp(new device_desc);
      // ddescp->set_minfo(top_minfo);
      // _dev_descriptors_[dev_ui_path] = ddescp;
      // for (const auto ddesc : _dev_descriptors_) {
      //   std::cerr << "Device descriptor: " << *ddesc.second.get() << std::endl;
      // }
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void ihs_handler::_construct_ihs()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      _ihs_.reset(new datatools::ui::ihs);

      datatools::ui::ihs & the_ihs = *_ihs_;
      the_ihs.set_name("devihs");
      the_ihs.set_display_name("devIHS");
      the_ihs.set_terse_description("The device interface hierarchical system");
      the_ihs.set_scheme(_device_manager_->get_setup_label());

      const vire::device::instance_tree::instance_info_dict_type & instances
        = _tree_->get_instances();
      int path_counter = 0;
      for (const auto & p : instances) {
        const std::string & the_path = p.first;
        _plug_device_interface(the_path);
      }

      DT_LOG_DEBUG(_logging_, "IHS after construction:");
      if (datatools::logger::is_debug(_logging_)) {
        _ihs_->tree_dump(std::cerr, "", "[debug] ");
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void ihs_handler::_plug_device_interface(const std::string & path_)
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      // DT_LOG_DEBUG(_logging_, "Entering : path_ = '" << path_ << "'");
      std::string device_path = path_;
      bool device = true;
      if (_tree_->has_instance(device_path)) {
        const vire::device::instance_info & iinfo = _tree_->get_instance(device_path);
        if (!iinfo.is_device()) {
          device = false;
        }
      }
      // DT_LOG_DEBUG(_logging_, "device_path = '" << device_path << "'");

      std::string ui_path = instance_to_ui_path(device_path);
      if (_ihs_->exists(ui_path)) {
        return;
      }

      //DT_LOG_DEBUG(_logging_, "ui_path = '" << ui_path << "'");
      std::string ui_mother_path = ui_parent_path(ui_path);
      //DT_LOG_DEBUG(_logging_, "ui_mother_path = '" << ui_mother_path << "'");
      std::string mother_path = ui_to_instance_path(ui_mother_path);
      //DT_LOG_DEBUG(_logging_, "mother_path = '" << mother_path << "'");
      if (!device) {
        //DT_LOG_DEBUG(_logging_, "Exiting: Not a device.");
        return;
      }

      /*
      if (_dev_interfaces_.find(ui_parent_path) == _dev_interfaces_.end()) {
        DT_LOG_DEBUG(_logging_, "Plug-ing parent interface : '" << ui_parent_path << "'...");
        // _plug_device_interface(parent_path);
        _plug_device_interface(ui_parent_path);
        DT_LOG_DEBUG(_logging_, "Plugged parent interface : '" << ui_parent_path << "'.");
      }
      */
      /*
      if (device
          && mapped
          && _dev_interfaces_.find(ui_path) == _dev_interfaces_.end()) {
        DT_LOG_DEBUG(_logging_, "Creating a new device command interface...'");
         // Create a new device command interface:
        vire::device::mapping_info & mutable_minfo = const_cast<vire::device::mapping_info &>(*minfo);
        std::string leaf_device_name = datatools::ui::path::basename(ui_path);
        DT_LOG_DEBUG(_logging_, "leaf_device_name = '" << leaf_device_name << "'");
        dev_interface_handler dev_ih(new device_command_interface(mutable_minfo,
                                                                  leaf_device_name,
                                                                  "Device interface"));
        _dev_interfaces_[ui_path] = dev_ih;
        DT_LOG_DEBUG(_logging_, "Added interface for = '" << leaf_device_name << "'.");
      }
      device_command_interface & dev_com_inter
        = *_dev_interfaces_.find(ui_path)->second.get();
      dev_com_inter.initialize_simple();
      datatools::ui::ihs & the_ihs = *_ihs_;
      */
      std::string ui_leaf_name = ui_leaf_path(ui_path);
      _ihs_->add_interface(ui_mother_path, ui_leaf_name);

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

  } // namespace inspector

} // namespace vire
