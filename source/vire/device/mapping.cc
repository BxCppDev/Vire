//! \file vire/device/mapping.cc
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/mapping.h>

// Third Party:
// - Boost:
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/algorithm/string.hpp>
// - Bayeux/geomtools:
#include <geomtools/mapping_utils.h>

// This project:
#include <vire/device/manager.h>
#include <vire/device/mapping_utils.h>
#include <vire/device/mapping_info.h>
#include <vire/device/physical_device.h>
#include <vire/device/logical_device.h>
#include <vire/device/logical_port.h>
#include <vire/device/physical_port.h>
#include <vire/device/single_instance.h>
#include <vire/utility/path.h>

namespace vire {

  namespace device {

    // Tags for accessing the corresponding indices of mapping_set :
    struct mapping_tag_id {};
    struct mapping_tag_path {};

    //! Multi index container of resources
    typedef boost::multi_index_container<
      mapping_info,
      boost::multi_index::indexed_by<

        // The first key is the unique resource identifier (RID):
        boost::multi_index::ordered_unique<
          boost::multi_index::tag<mapping_tag_id>,
          boost::multi_index::const_mem_fun<mapping_info,
                                            const geomtools::geom_id &,
                                            &mapping_info::get_mapping_id
                                            >
          >,

        // The second key is the unique resource path:
        boost::multi_index::ordered_unique<
          boost::multi_index::tag<mapping_tag_path>,
          boost::multi_index::const_mem_fun<instance_info,
                                            const std::string &,
                                            &instance_info::get_path
                                            >
          >
        >
      > mapping_set;

    typedef mapping_set::index<mapping_tag_id>::type   mapping_set_by_id;
    typedef mapping_set::index<mapping_tag_path>::type mapping_set_by_path;

    struct mapping::work {
      mapping_set dict;
    };

    // static
    unsigned int mapping::depth_no_limit()
    {
      static const unsigned int _value = 0;
      return _value;
    }

    // static
    const std::string & mapping::default_setup_category()
    {
      static const std::string _cat("setup");
      return _cat;
    }

    void mapping::_set_defaults()
    {
      // Configuration:
      _manager_ = nullptr;
      _top_level_mapping_ = false;
      _max_depth_ = depth_no_limit();
      _port_mapping_ = true;
      _link_mapping_ = false;

      // Working/internal data:
      _top_level_logical_ = nullptr;
      _mapping_manager_ = nullptr;
      _mapping_depth_ = 0;
      return;
    }

    mapping::mapping()
    {
      _logging_priority_ = datatools::logger::PRIO_FATAL;
      _initialized_ = false;
      _set_defaults();
      return;
    }

    mapping::~mapping()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    datatools::logger::priority mapping::get_logging_priority() const
    {
      return _logging_priority_;
    }

    void mapping::set_logging_priority(datatools::logger::priority l_)
    {
      _logging_priority_ = l_;
      return;
    }

    bool mapping::has_manager() const
    {
      return _manager_ != nullptr;
    }

    void mapping::set_manager(const manager & mgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      _manager_ = &mgr_;
      return;
    }

    bool mapping::is_top_level_mapping() const
    {
      return _top_level_mapping_;
    }

    void mapping::set_top_level_mapping(bool tlm_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      _top_level_mapping_ = tlm_;
      return;
    }

    const manager & mapping::get_manager() const
    {
      return *_manager_;
    }

    bool mapping::has_mapping_manager() const
    {
      return _mapping_manager_ != nullptr;
    }

    void mapping::set_mapping_manager(const geomtools::id_mgr & mapping_mgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      _mapping_manager_ = &mapping_mgr_;
      return;
    }

    const geomtools::id_mgr & mapping::get_mapping_manager() const
    {
      return *_mapping_manager_;
    }

    bool mapping::has_max_depth() const
    {
      return _max_depth_ > depth_no_limit();
    }

    void mapping::set_max_depth(unsigned int max_depth_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      _max_depth_ = max_depth_;
      return;
    }

    unsigned int mapping::get_max_depth() const
    {
      return _max_depth_;
    }

    void mapping::set_port_mapping(bool pm_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      _port_mapping_ = pm_;
      return;
    }

    bool mapping::is_port_mapping() const
    {
      return _port_mapping_;
    }

    void mapping::set_link_mapping(bool lm_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      _link_mapping_ = lm_;
      return;
    }

    bool mapping::is_link_mapping() const
    {
      return _link_mapping_;
    }

    bool mapping::is_mode_only() const
    {
      return _only_categories_.size();
    }

    bool mapping::is_mode_excluded() const
    {
      return _excluded_categories_.size();
    }

    void mapping::add_only(const std::string & category_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      DT_THROW_IF(is_mode_excluded(), std::logic_error, "Mapping uses 'excluded' mode !");
      _only_categories_.push_back(category_);
      return;
    }

    void mapping::add_excluded(const std::string & category_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      DT_THROW_IF(is_mode_only(), std::logic_error, "Mapping uses 'only' mode !");
      _excluded_categories_.push_back(category_);
      return;
    }

    void mapping::set_top_level_device_model_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      _top_level_device_model_name_ = name_;
      return;
    }

    const std::string & mapping::get_top_level_device_model_name() const
    {
      return _top_level_device_model_name_;
    }

    void mapping::set_top_level_device_instance_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      _top_level_device_instance_name_ = name_;
      return;
    }

    const std::string & mapping::get_top_level_device_instance_name() const
    {
      return _top_level_device_instance_name_;
    }

    bool mapping::is_initialized() const
    {
      return _initialized_;
    }

    void mapping::_configure(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");

      // Parse logging parameteres:
      datatools::logger::priority p = datatools::logger::extract_logging_configuration(config_);
      set_logging_priority(p);

      if (config_.has_key("top_level_device_model_name")) {
        set_top_level_device_model_name(config_.fetch_string("top_level_device_model_name"));
      }

      if (_top_level_device_model_name_.empty()) {
        _top_level_device_model_name_ = _manager_->get_top_level_device_model_name();
      }

      if (config_.has_key("top_level_device_instance_name")) {
        set_top_level_device_instance_name(config_.fetch_string("top_level_device_instance_name"));
      }

      if (_top_level_device_instance_name_.empty()) {
        _top_level_device_instance_name_ = _manager_->get_setup_label() + vire::utility::path::setup_separator();
      }

      if (config_.has_key("max_depth")) {
        int md = config_.fetch_positive_integer("max_depth");
        set_max_depth((unsigned int) md);
      }

      if (config_.has_key("top_level_mapping")) {
        _top_level_mapping_ = config_.fetch_boolean("top_level_mapping");
      }

      if (config_.has_key("port_mapping")) {
        set_port_mapping(config_.fetch_boolean("port_mapping"));
      }

      if (config_.has_key("link_mapping")) {
        set_link_mapping(config_.fetch_boolean("link_mapping"));
      }

      if (_excluded_categories_.size() == 0) {
        if (config_.has_key("only_categories")) {
          std::vector<std::string> onlys;
          config_.fetch("only_categories", onlys);
          for (int i = 0; i < onlys.size(); i++) {
            add_only(onlys[i]);
          }
        }
      }

      if (_only_categories_.size() == 0) {
        if (config_.has_key("excluded_categories")) {
          std::vector<std::string> excluded;
          config_.fetch("excluded_categories", excluded);
          for (int i = 0; i < excluded.size(); i++) {
            add_excluded(excluded[i]);
          }
        }
      }

      return;
    }

    void mapping::initialize(const datatools::properties & config_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      DT_THROW_IF(is_initialized(), std::logic_error, "Mapping is already initialized !");
      DT_THROW_IF(! has_manager(), std::logic_error, "Missing device model manager !");
      DT_THROW_IF(! _manager_->is_initialized(), std::logic_error,
                  "Device manager is not initialized !");
      if (! has_mapping_manager()) {
        set_mapping_manager(_manager_->get_mapping_manager());
      }

      _configure(config_);

      _work_.reset(new work);

      _top_build();

      _initialized_ = true;
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void mapping::reset()
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      DT_THROW_IF(!is_initialized(), std::logic_error, "Mapping is not initialized !");
      _initialized_ = false;

      _work_->dict.erase(_work_->dict.get<mapping_tag_id>().begin(),
                         _work_->dict.get<mapping_tag_id>().end());
      _work_.reset();
      _mapping_depth_ = 0;
      _mapping_manager_ = nullptr;
      _top_level_logical_ = nullptr;
      _top_level_device_model_name_.clear();
      _top_level_device_instance_name_.clear();
      _only_categories_.clear();
      _excluded_categories_.clear();
      _set_defaults();

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    // bool mapping::validate_id(const geomtools::geom_id & id_) const
    // {
    //   return ;
    // }

    // bool mapping::validate_path(const std::string & path_) const
    // {
    //   return ;
    // }

    std::size_t mapping::get_number_of_entries() const
    {
      return _work_->dict.size();
    }

    void mapping::build_paths(std::set<std::string> & paths_, uint32_t flags_) const
    {
      for (mapping_set_by_path::const_iterator ipath
             = _work_->dict.get<mapping_tag_path>().begin();
           ipath != _work_->dict.get<mapping_tag_path>().end();
           ipath++) {
        const mapping_info & minfo = *ipath;
        bool insert_it = false;
        insert_it = minfo.is_device() || minfo.is_port();
        if (insert_it) {
          paths_.insert(minfo.get_path());
        }
      }
      return;
    }

    bool mapping::has_top_path() const
    {
      geomtools::geom_id top_gid(0, 0);
      mapping_set_by_id::const_iterator found
        = _work_->dict.get<mapping_tag_id>().find(top_gid);
      return found != _work_->dict.get<mapping_tag_id>().end();
    }

    std::string mapping::get_top_path() const
    {
      geomtools::geom_id top_gid(0, 0);
      mapping_set_by_id::const_iterator found
        = _work_->dict.get<mapping_tag_id>().find(top_gid);
      DT_THROW_IF(found == _work_->dict.get<mapping_tag_id>().end(),
                  std::logic_error,
                  "Mapping has no top level path!");
      return found->get_path();
    }

    bool mapping::has_mapping_info(const geomtools::geom_id & id_) const
    {
      mapping_set_by_id::const_iterator found = _work_->dict.get<mapping_tag_id>().find(id_);
      return found != _work_->dict.get<mapping_tag_id>().end();
    }

    bool mapping::has_mapping_info(const std::string & path_) const
    {
      mapping_set_by_path::const_iterator found = _work_->dict.get<mapping_tag_path>().find(path_);
      return found != _work_->dict.get<mapping_tag_path>().end();
    }

    mapping_info & mapping::_grab_mapping_info_(const geomtools::geom_id & id_)
    {
      mapping_set_by_id::const_iterator found = _work_->dict.get<mapping_tag_id>().find(id_);
      DT_THROW_IF(found == _work_->dict.get<mapping_tag_id>().end(),
                  std::logic_error,
                  "No GID=" << id_ << " has been found in this mapping!");
      return const_cast<mapping_info &>(*found);
    }

    const mapping_info & mapping::get_mapping_info(const geomtools::geom_id & id_) const
    {
      mapping_set_by_id::const_iterator found = _work_->dict.get<mapping_tag_id>().find(id_);
      DT_THROW_IF(found == _work_->dict.get<mapping_tag_id>().end(),
                  std::logic_error,
                  "No GID=" << id_ << " has been found in this mapping!");
      return *found;
    }

    const mapping_info & mapping::get_mapping_info(const std::string & path_) const
    {
      mapping_set_by_path::const_iterator found = _work_->dict.get<mapping_tag_path>().find(path_);
      DT_THROW_IF(found == _work_->dict.get<mapping_tag_path>().end(),
                  std::logic_error,
                  "No path='" << path_ << "' has been found in this mapping!");
      return *found;
    }

    const logical_device & mapping::get_top_level_logical() const
    {
      DT_THROW_IF(_top_level_logical_ == nullptr,
                  std::logic_error,
                  "Cannot find the top-level device model with name '" << _top_level_device_model_name_
                  << "' in the virtual device models manager !");
      return *_top_level_logical_;
    }

    void mapping::_top_build()
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      std::string setup_device_model_name = _top_level_device_model_name_;
      // if (setup_device_model_name.empty()) {
      //   setup_device_model_name = _top_level_device_model_name_;
      // }
      // if (setup_device_model_name.empty()) {
      //   setup_device_model_name = _manager_->get_top_level_device_model_name();
      // }
      // set_top_level_device_model_name(setup_device_model_name);

      if (_top_level_logical_ == nullptr) {
        DT_THROW_IF(!_manager_->has_device_model(setup_device_model_name),
                    std::logic_error,
                    "Cannot find the top-level device model with name '" << setup_device_model_name
                    << "' in the virtual device models manager !");
        _top_level_logical_ = &(_manager_->get_device_model(setup_device_model_name).get_logical());
        DT_LOG_TRACE(get_logging_priority(), "Top level logical @ " << _top_level_logical_);
      }

      _build();

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void mapping::print_tree(std::ostream & out_,
                             const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);
      bool print_ids_list = options_.get<bool>("list_ids", false);

      if (!popts.title.empty()) out_ << popts.indent << popts.title << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Logging priority : '"
           << datatools::logger::get_priority_label(get_logging_priority())
           << "'" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Max depth : [" << _max_depth_ << "]" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Manager : [@" << _manager_ << "]" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Top-level mapping : " << _top_level_mapping_ << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Top-level device model name : '" << _top_level_device_model_name_ << "'" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Only categories : ";
      if (_only_categories_.size()) {
        out_ << "<none>";
      } else {
        out_ << "[" << _only_categories_.size() << "]";
      }
      out_ << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Excluded categories : ";
      if (_excluded_categories_.size()) {
        out_ << "<none>";
      } else {
        out_ << "[" << _excluded_categories_.size() << "]";
      }
      out_ << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Port mapping : '" << _port_mapping_ << "'" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Link mapping : '" << _link_mapping_ << "'" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Top-level logical : ";
      if (_top_level_logical_) {
        out_ << "'" << _top_level_logical_->get_name() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Mapping ID manager : [@" << _mapping_manager_ << "]" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Device information dictionary : ";
      if (is_initialized()) {
        out_ << "[" << _work_->dict.size() << "]";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      if (print_ids_list) {
        if (is_initialized()) {
          for (mapping_set_by_id::const_iterator i = _work_->dict.get<mapping_tag_id>().begin();
               i != _work_->dict.get<mapping_tag_id>().end();
               i++) {
            mapping_set_by_id::const_iterator j = i;
            j++;
            out_ << popts.indent << i_tree_dumpable::skip_tag;
            if (j == _work_->dict.get<mapping_tag_id>().end()) {
              out_ << i_tree_dumpable::last_tag;
            } else {
              out_ << i_tree_dumpable::tag;
            }
            const mapping_info & mi = *i;
            out_ << "ID=" << mi.get_mapping_id() << " with path '" << mi.get_path() << "'";
            if (mi.is_device()) {
              out_ << " (as logical device named '" << mi.get_logical_device().get_name() << "')";
            } else if (mi.is_port()) {
              out_ << " (as logical port named '" << mi.get_logical_port().get_name() << "')";
            }
            out_ << std::endl;
          }
        }
      }

      out_ << popts.indent << i_tree_dumpable::inherit_tag(popts.inherit)
           << "Initialized    : "
           << this->is_initialized()
           << std::endl;

      return;
    }

    void mapping::_build()
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      std::string setup_cat_name = mapping::default_setup_category();
      // _mapping_manager_->tree_dump(std::cerr, "Mapping manager: ", "DEVEL: ");
      DT_THROW_IF(! _mapping_manager_->has_category_info(setup_cat_name),
                  std::logic_error,
                  "Unknown top-level setup category '" << setup_cat_name << "' !");
      const geomtools::id_mgr::category_info & setup_cat_info
        = _mapping_manager_->get_category_info(setup_cat_name);
      // Dynamic LIFO collection of handles to mapping_info objects:
      info_stack_type ancestors;
      // Top level setup ID:
      geomtools::geom_id setup_root_id;
      setup_cat_info.create(setup_root_id);
      setup_root_id.set_address(0);
      DT_LOG_DEBUG(_logging_priority_, "Setup root ID = " << setup_root_id << ' '
                   << (setup_root_id.is_valid() ? "[Valid]": "[Invalid]"));
      std::string setup_root_path = _top_level_device_instance_name_;
      // _manager_->get_setup_label() + vire::utility::path::setup_separator();
      DT_LOG_DEBUG(_logging_priority_, "Setup root path = '" << setup_root_path << "'");
      mapping_info setup_root_info;
      setup_root_info.set_mapping_id(setup_root_id);
      setup_root_info.set_path(setup_root_path);
      setup_root_info.set_logical_device(*_top_level_logical_);

      // Add setup mapping info :
      if (_top_level_mapping_)  {
        _work_->dict.insert(setup_root_info);
      }
      ancestors.stack.push_back(&setup_root_info);
      ancestors.path.push_back(setup_root_path);
      _build_embedded_devices_mapping(*_top_level_logical_, ancestors);
      /*
        bool build_mode_strict_mothership = true;
        if (build_mode_strict_mothership) {
        _build_embedded_devices_mapping(*_top_level_logical_, setup_id);
        }
      */

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void mapping::_build_device_mapping(const logical_device & mother_log_,
                                        info_stack_type & ancestors_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      ++_mapping_depth_;

      if (is_port_mapping()) {
        // Build mapping records for all mapped ports in the mother logical device:
        _build_ports_mapping(mother_log_, ancestors_);
      }

      // Build mapping records for all embedded devices in the mother logical device:
      _build_embedded_devices_mapping(mother_log_, ancestors_);

      if (is_link_mapping()) {
        // Build mapping records for all mapped links in the mother logical device:
        //_build_link_mapping(mother_log_, ancestors_);
      }

      --_mapping_depth_;
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void mapping::_build_ports_mapping(const logical_device & mother_log_,
                                       info_stack_type & ancestors_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      /*
        DT_THROW_IF(!mother_log_.has_model(), std::logic_error, "Port '" << port_name << "' has no port model!");
        const base_device_model & mother_device_model = mother_log_.get_model();
        const std::type_info & tmdm = typeid(mother_device_model);
        const std::type_info & tbmpm = typeid(vire::device::base_method_port_model);
        bool is_datapoint = false;
        if (tmdm == tbmpm) {
        is_datapoint = true;
        }
      */
      // Map the ports of the logical device:
      for (logical_device::ports_dict_type::const_iterator i
             = mother_log_.get_ports().begin();
           i != mother_log_.get_ports().end();
           i++) {
        const std::string & port_name = i->first;
        const physical_port & port_phys = *(i->second);
        const logical_port & port_logical = port_phys.get_logical();
        // DT_THROW_IF(!port_logical.has_model(), std::logic_error, "Port '" << port_name << "' has no port model!");
        // const base_port_model & port_model = port_logical.get_model();

        std::string port_mapping_info;
        if (geomtools::mapping_utils::has_labelled_info(mother_log_.get_auxiliaries(),
                                                        "port",
                                                        port_name)) {
          geomtools::mapping_utils::fetch_labelled_info(mother_log_.get_auxiliaries(),
                                                        "port",
                                                        port_name,
                                                        port_mapping_info);
          DT_LOG_TRACE(get_logging_priority(),
                       "Found port ID mapping info for physical port '" << port_name << "' : '" << port_mapping_info << "'.");
        } else {
          DT_LOG_TRACE(get_logging_priority(),
                       "No port ID mapping info for physical port '" << port_name << "'");
          continue;
        }
        DT_THROW_IF(!port_phys.has_instance(), std::logic_error, "Missing instance in port '" << port_name << "'!");
        const i_instance & phys_instance = port_phys.get_instance();
        DT_LOG_TRACE(get_logging_priority(), "Number of items: " << phys_instance.get_number_of_items());

        // Loop on replicated ports:
        for (size_t item = 0; item < phys_instance.get_number_of_items(); item++) {
          DT_LOG_TRACE (get_logging_priority(), "-> item #" << item << ": ");
          // Get the current item instance in the mother slotting system:
          slot item_slot;
          phys_instance.fetch_slot(item, item_slot);
          {
            DT_LOG_TRACE(get_logging_priority(), "-> child slot : " << item_slot);
          }

          DT_LOG_NOTICE(get_logging_priority(),
                        "Found port mapping category information for port with label '"
                        << port_name << "'.");
          geomtools::geom_id port_id;
          // Compute the vector of sub-addresses:
          std::vector<uint32_t> port_indexes;
          item_slot.fetch_coordinates(port_indexes, true);
          // if (port_indexes.size() == 0) {
          //   std::cerr << "DEVEL: " << " ******* Port index vector is empty" << std::endl;
          // }
          const geomtools::geom_id & mother_id = ancestors_.stack.back()->get_mapping_id();
          const std::string & mother_path = ancestors_.path.back();
          _mapping_manager_->compute_id_from_info(port_id,
                                                  mother_id,
                                                  port_mapping_info,
                                                  port_indexes);
          DT_LOG_TRACE(get_logging_priority(), "port_id=" << port_id);
          if (! _mapping_manager_->validate_id(port_id)) {
            DT_LOG_TRACE(get_logging_priority(), "port_id   =" << port_id << " is NOT validated by the ID manager!");
          } else {
            DT_LOG_TRACE(get_logging_priority(), "port_id   = " << port_id << " IS validated by the ID manager...");
            DT_LOG_TRACE(get_logging_priority(), "mother_id = " << mother_id << ".");
            // std::ostringstream port_path_oss;
            // port_path_oss << mother_path;
            // if (! boost::ends_with(mother_path, vire::utility::path::setup_separator())) {
            //   port_path_oss << vire::utility::path::path_separator();
            // }
            // port_path_oss << port_name;
            // for (int iaddr = 0; iaddr < (int) port_indexes.size(); iaddr++) {
            //   port_path_oss << '_' << port_indexes[iaddr];
            // }
            std::string port_path;
            // = port_path_oss.str();
            vire::utility::path::build_child_with_indexes(mother_path, port_name, port_indexes, port_path);
            DT_LOG_TRACE(get_logging_priority(), "port_path = '" << port_path << "'...");
            mapping_info port_mi;
            port_mi.set_mapping_id(port_id);
            port_mi.set_path(port_path);
            if (mother_id.is_valid()) {
              port_mi.set_parent_device_id(mother_id);
            }
            port_mi.set_logical_port(port_logical);
            bool add_it = true;
            if (is_mode_only() || is_mode_excluded()) {
              // get the category associated to the port ID:
              const std::string & category
                = _mapping_manager_->get_category_info(port_id.get_type()).get_category();
              DT_LOG_TRACE(get_logging_priority(),
                           "Category = '" << category << "'(from ID=" << port_id << ")");
              if (is_mode_only()) {
                // the list contains only the categories to be registered:
                std::vector<std::string>::const_iterator found =
                  std::find(_only_categories_.begin(),
                            _only_categories_.end(),
                            category);
                if (found == _only_categories_.end()) {
                  add_it = false;
                }
              } else if (is_mode_excluded()) {
                // the list contains only the categories to be excluded:
                std::vector<std::string>::const_iterator found
                  = std::find(_excluded_categories_.begin(),
                              _excluded_categories_.end(),
                              category);
                if (found != _excluded_categories_.end())  {
                  add_it = false;
                }
              }
            }
            if (add_it) {
              DT_LOG_TRACE(get_logging_priority(), "Adding port ID=" << port_id << "...");
              _work_->dict.insert(port_mi);
            }
          } // port id is valid
        } // for instantiated item in the physical port
      } // for ports

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void mapping::_build_embedded_devices_mapping(const logical_device & mother_log_,
                                                  info_stack_type & ancestors_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");

      // Map the daughter embedded devices of the logical device:
      for (logical_device::daughters_dict_type::const_iterator i
             = mother_log_.get_daughters().begin();
           i != mother_log_.get_daughters().end();
           i++) {
        const std::string & embedded_name = i->first;
        const physical_device & embedded_phys = *(i->second);
        DT_LOG_TRACE(get_logging_priority(),
                     "Embedded physical device '" << embedded_name << "' with physical name "
                     << "'" << embedded_phys.get_name() << "'");

        std::string embedded_mapping_info;
        if (datatools::logger::is_debug(get_logging_priority())) {
          DT_LOG_DEBUG(get_logging_priority(), "Auxiliaries for the mother logical '" << mother_log_.get_name() << "': ");
          mother_log_.get_auxiliaries().tree_dump(std::cerr, "", "[debug] ");
        }
        if (geomtools::mapping_utils::has_labelled_info(mother_log_.get_auxiliaries(),
                                                        "device",
                                                        embedded_name)) {
          geomtools::mapping_utils::fetch_labelled_info(mother_log_.get_auxiliaries(),
                                                        "device",
                                                        embedded_name,
                                                        embedded_mapping_info);
          DT_LOG_TRACE(get_logging_priority(),
                       "Found device ID mapping info for embedded physical device '"
                       << embedded_name << "' : '" << embedded_mapping_info << "'.");
        } else {
          DT_LOG_TRACE(get_logging_priority(),
                       "No device ID mapping info for embedded physical device '"
                       << embedded_name << "'");
          //continue; // XXX
        }
        const logical_device & embedded_logical = embedded_phys.get_logical();
        DT_THROW_IF(!embedded_phys.has_instance(), std::logic_error,
                    "Missing instance in embedded device '" << embedded_name << "'!");
        const i_instance & phys_instance = embedded_phys.get_instance();
        DT_LOG_TRACE(get_logging_priority(), "Number of items: " << phys_instance.get_number_of_items());

        // Loop on replicated embedded devices:
        for (size_t item = 0; item < phys_instance.get_number_of_items(); item++) {
          DT_LOG_TRACE(get_logging_priority(), "-> embedded item #" << item << ": ");
          // Get the current item instance in the mother slotting system:
          slot item_slot;
          phys_instance.fetch_slot(item, item_slot);
          {
            DT_LOG_TRACE(get_logging_priority(), "-> embedded child slot : " << item_slot);
          }
          const geomtools::geom_id & mother_id = ancestors_.stack.back()->get_mapping_id();
          DT_LOG_TRACE(get_logging_priority(), "ancestors.top : mother_id = " << mother_id);
          const std::string & mother_path = ancestors_.path.back();
          geomtools::geom_id propagated_parent_id = mother_id;

          DT_LOG_NOTICE(get_logging_priority(),
                        "Found embedded device mapping category information for embedded device with label '"
                        << embedded_name << "'.");
          geomtools::geom_id embedded_id;
          // Compute the vector of sub-addresses:
          std::vector<uint32_t> embedded_indexes;
          item_slot.fetch_coordinates(embedded_indexes, true);
          DT_LOG_TRACE(get_logging_priority(), "-> embedded index: size=[" << embedded_indexes.size() << "]");
          for (int idx = 0; idx < (int) embedded_indexes.size(); idx++) {
            DT_LOG_TRACE(get_logging_priority(), "  -> embedded index: idx=[" << idx << "]");
          }
          // if (embedded_indexes.size() == 0) {
          //   std::cerr << "DEVEL: " << " ******* Embedded index vector is empty" << std::endl;
          // }
          DT_LOG_TRACE(get_logging_priority(),
                       "Embedded index number of coordinates = ["
                       << embedded_indexes.size() << "]");
          for (int i = 0; i < (int) embedded_indexes.size(); i++) {
            DT_LOG_TRACE(get_logging_priority(), " - embedded_indexes[" << i << "]=" << embedded_indexes[i]);
          }
          DT_LOG_TRACE(get_logging_priority(),
                       "Embedded mapping info = '"
                       << embedded_mapping_info << "'");
          if (! embedded_mapping_info.empty()) {
            _mapping_manager_->compute_id_from_info(embedded_id,
                                                    mother_id,
                                                    embedded_mapping_info,
                                                    embedded_indexes);
          }
          DT_LOG_TRACE(get_logging_priority(), "embedded_id=" << embedded_id);
          bool valid_embedded_id = false;
          if (_mapping_manager_->validate_id(embedded_id)) {
            valid_embedded_id = true;
          }
          if (!valid_embedded_id) {
            DT_LOG_TRACE(get_logging_priority(), "embedded_id=" << embedded_id << " is NOT validated by the ID manager!");
          } else {
            DT_LOG_TRACE(get_logging_priority(), "embedded_id = " << embedded_id << " IS validated by the ID manager...");
          }
          DT_LOG_TRACE(get_logging_priority(), "embedded mother_path   = '" << mother_path << "'");
          // std::ostringstream embedded_path_oss;
          // embedded_path_oss << mother_path;
          // if (! boost::ends_with(mother_path, vire::utility::path::setup_separator())) {
          //   embedded_path_oss << vire::utility::path::path_separator();
          // }
          // embedded_path_oss << embedded_name;
          // DT_LOG_TRACE(get_logging_priority(), "Append name '" << embedded_name << "' to mother path = '" << mother_path << "'...");
          // for (int iaddr = 0; iaddr < (int) embedded_indexes.size(); iaddr++) {
          //   DT_LOG_TRACE(get_logging_priority(), "Append index #" << iaddr << " index=" << embedded_indexes.size() << " = [" << embedded_indexes[iaddr] << "]...");
          //   embedded_path_oss << vire::utility::path::index_separator() << embedded_indexes[iaddr];
          // }
          std::string embedded_path;
          vire::utility::path::build_child_with_indexes(mother_path, embedded_name, embedded_indexes, embedded_path);
          DT_LOG_DEBUG(get_logging_priority(), "Attempt to add embedded device with path = '" << embedded_path << "'...");
          mapping_info embedded_mi;
          if (valid_embedded_id) {
            embedded_mi.set_mapping_id(embedded_id);
            embedded_mi.set_path(embedded_path);
            if (mother_id.is_valid()) {
              embedded_mi.set_parent_device_id(mother_id);
              mapping_info & mother_info = _grab_mapping_info_(mother_id);
              mother_info._add_daughter_device_id_(embedded_id);
            }
            embedded_mi.set_logical_device(embedded_logical);
          }
          bool add_it = true;
          if (valid_embedded_id) {
            if (is_mode_only() || is_mode_excluded()) {
              // get the category associated to the embedded ID:
              const std::string & category
                = _mapping_manager_->get_category_info(embedded_id.get_type()).get_category();
              DT_LOG_TRACE(get_logging_priority(),
                           "Category = '" << category << "'(from ID=" << embedded_id << ")");
              if (is_mode_only()) {
                // the list contains only the categories to be registered:
                std::vector<std::string>::const_iterator found =
                  std::find(_only_categories_.begin(),
                            _only_categories_.end(),
                            category);
                if (found == _only_categories_.end()) {
                  add_it = false;
                }
              } else if (is_mode_excluded()) {
                // the list contains only the categories to be excluded:
                std::vector<std::string>::const_iterator found
                  = std::find(_excluded_categories_.begin(),
                              _excluded_categories_.end(),
                              category);
                if (found != _excluded_categories_.end())  {
                  add_it = false;
                }
              }
            }
          }
          ancestors_.path.push_back(embedded_path);
          if (valid_embedded_id) {
            const mapping_info * ptr_mapping_info = &embedded_mi;
            if (add_it) {
              DT_LOG_TRACE(get_logging_priority(), "Adding embedded device ID = " << embedded_id << "...");
              std::pair<mapping_set::iterator,bool> insert_result = _work_->dict.insert(embedded_mi);
              DT_THROW_IF(insert_result.second != true, std::logic_error,
                          "Could not insert embedded mapping info for ID=" << embedded_id);
              ptr_mapping_info = &*insert_result.first;
            }
            ancestors_.stack.push_back(ptr_mapping_info);
            DT_LOG_TRACE(get_logging_priority(), "Push ancestor mapping info = " << ptr_mapping_info->get_path() << "...");
          }
          bool build_it = true;
          if (has_max_depth()) {
            if (_mapping_depth_ > _max_depth_) {
              build_it = false;
            }
          }
          if (build_it) {
            DT_LOG_TRACE(get_logging_priority(), "Built device mapping for '" << embedded_logical.get_name() << "'");
            DT_LOG_TRACE(get_logging_priority(), "Ancestors stack:");
            for (minfo_stack_type::const_iterator ianc = ancestors_.stack.begin();
                 ianc != ancestors_.stack.end();
                 ianc++) {
              DT_LOG_TRACE(get_logging_priority(), "  Ancestors' ID = " << (*ianc)->get_mapping_id() << " with path '" << (*ianc)->get_path() << "'");
            }
            for (path_stack_type::const_iterator ianc = ancestors_.path.begin();
                 ianc != ancestors_.path.end();
                 ianc++) {
              DT_LOG_TRACE(get_logging_priority(), "  Ancestors' path = '" << *ianc << "'");
            }
             _build_device_mapping(embedded_logical, ancestors_);
            // _build_embedded_devices_mapping(embedded_logical, propagated_parent_id);
          } else {
            DT_LOG_TRACE(get_logging_priority(), "-> DO NOT TRAVERSE THE DEVICE TREE FURTHER.");
          }
          if (valid_embedded_id) {
            ancestors_.stack.pop_back();
          }
          ancestors_.path.pop_back();
        } // for instantiated item in the physical embedded device
      } // for embedded devices

      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

  } // end of namespace device

} // end of namespace vire
