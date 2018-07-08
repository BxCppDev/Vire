//! \file vire/cmsserver/sc_info.cc
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
#include <vire/cmsserver/sc_info.h>

// Standard Library:
// #include <chrono>

#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/time/utils.h>
#include <vire/cms/connection_request.h>
#include <vire/cms/connection_success.h>
#include <vire/cms/connection_failure.h>
#include <vire/cms/disconnection_request.h>
#include <vire/cms/disconnection_success.h>
#include <vire/cms/disconnection_failure.h>
#include <vire/utility/path.h>
#include <vire/resource/resource.h>
#include <vire/com/manager.h>
#include <vire/com/actor.h>
#include <vire/com/i_service_client_plug.h>

namespace vire {

  namespace cmsserver {
 
    sc_info_signal_emitter::sc_info_signal_emitter(sc_info & sc_info_)
      : _subcontractor_info_(sc_info_)
    {
      return;
    }

    const sc_info & sc_info_signal_emitter::get_subcontractor_info() const
    {
      return _subcontractor_info_;
    }
      
    sc_info & sc_info_signal_emitter::grab_subcontractor_info() const
    {
      return _subcontractor_info_;
    }

    void sc_info_signal_emitter::emit_connection_change()
    {
      emit connection_changed();
    }

    void sc_info_signal_emitter::emit_device_status_change(std::string device_path_)
    {
      emit device_status_changed(device_path_);
    }
 
    sc_info_signal_emitter & sc_info::_grab_emitter_()
    {
      if (!_emitter_) {
        _emitter_.reset(new sc_info_signal_emitter(*this));
      }
      return *_emitter_.get();
    }

    const sc_info_signal_emitter & sc_info::get_emitter() const
    {
      sc_info * mutable_this = const_cast<sc_info*>(this);
      return mutable_this->_grab_emitter_();
    }
   
    sc_info_signal_emitter & sc_info::grab_emitter()
    {
      return _grab_emitter_();
    }
 
    float sc_info::default_connection_timeout()
    {
      static const float _t(2.0 * CLHEP::second);
      return _t;
    }

    sc_info::sc_info()
    {
      return;
    }

    sc_info::~sc_info()
    {
      return;
    }

    bool sc_info::is_auto_connect() const
    {
      return _auto_connect_;
    }

    void sc_info::set_auto_connect(const bool flag_)
    {
      _auto_connect_ = flag_;
      return;
    }

    bool sc_info::has_max_connection_attempts() const
    {
      return _max_connection_attempts_ > 0;
    }
    
    std::size_t sc_info::get_max_connection_attempts() const
    {
      return _max_connection_attempts_;
    }
    
    void sc_info::set_max_connection_attempts(const std::size_t max_)
    {
      _max_connection_attempts_ = max_;
      return;
    }
    
    void sc_info::set_com_manager(vire::com::manager & com_)
    {
      _com_ = &com_;
      return;
    }

    void sc_info::set_device_manager(const vire::device::manager & devices_)
    {
      _devices_ = &devices_;
      return;
    }
      
    void sc_info::set_resource_manager(const vire::resource::manager & resources_)
    {
      _resources_ = &resources_;
      return;
    }
  
    bool sc_info::is_connected() const
    {
      return _connected_;
    }
    
    void sc_info::set_connected(const bool connected_)
    {
      bool changed = false;
      if (_connected_ != connected_) {
        changed = true;
      }
      if (changed) {
        _connected_ = connected_;
        if (_connected_) _on_connect_();
        else _on_disconnect_();
      }
      return;
    }

    void sc_info::build_mounted_devices(std::set<std::string> & names_) const
    {
      names_.clear();
      for (const auto & p : _mounted_devices_) {
        names_.insert(p.first);
      }
      return;
    }

    void sc_info::add_mounted_device(const std::string & device_path_,
                                     const std::string & selection_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Subcontractor '" << get_name() << "' is initialized and locked!");
      const vire::device::instance_tree & itree = _devices_->get_tree();
      DT_THROW_IF(!itree.has_instance(device_path_)
                  && ! itree.get_instance(device_path_).is_device(),
                  std::logic_error,
                  "Device manager '" << _devices_->get_setup_label()
                  << "' has no device instance with path '" << device_path_ << "'!");
      device_info devInfo;
      if (selection_.empty()) {
        devInfo.selection = "(.*)";
      } else {
        devInfo.selection = selection_;
      }
      devInfo.status.set_timestamp(vire::time::now_utc());
      _mounted_devices_[device_path_] = devInfo;

      // std::cerr << " **************************\n";
      // std::cerr << " Device path : " << device_path_ << "\n";
      // std::cerr << " Selection   : " << devInfo.selection << "\n";
      // std::cerr << " **************************\n" << std::endl;
      
      return;
    }

    bool sc_info::has_mounted_device(const std::string & device_path_)
    {
      return _mounted_devices_.count(device_path_) > 0;
    }

    void sc_info::remove_mounted_device(const std::string & device_path_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Subcontractor '" << get_name() << "' is initialized and locked!");
      DT_THROW_IF(!has_mounted_device(device_path_),
                  std::logic_error,
                  "No mounted device with path '" << device_path_ << "' in subcontractor '" << get_name() << "'!");
      return;
    }

    bool sc_info::is_initialized() const
    {
      return _initialized_;
    }
      
    void sc_info::initialize_simple()
    {
      initialize(datatools::empty_config());
      return;
    }
      
    void sc_info::initialize(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      this->datatools::enriched_base::initialize(config_, false);
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Subcontractor '" << get_name() << "' is already initialized!");

      DT_THROW_IF(!has_name(),
                  std::logic_error,
                  "Subcontractor has no name!");
      
      DT_THROW_IF(_devices_ == nullptr,
                  std::logic_error,
                  "Subcontractor has no device manager service!");
      
      DT_THROW_IF(_resources_ == nullptr,
                  std::logic_error,
                  "Subcontractor has no resource manager service!");

      if (config_.has_key("mounted_devices")) {
        std::set<std::string> mdev_labels;
        config_.fetch("mounted_devices", mdev_labels);
        for (const std::string & mdev_label : mdev_labels) {
          std::string mdev_path;
          std::string mdev_selection;
          {
            std::ostringstream mdev_path_key_ss;
            mdev_path_key_ss << "mounted_devices." << mdev_label << ".path";
            DT_THROW_IF(!config_.has_key(mdev_path_key_ss.str()),
                        std::logic_error,
                        "Missing path of mounted device labelled '" << mdev_label << "'!");
            mdev_path = config_.fetch_string(mdev_path_key_ss.str());
            vire::utility::path testDevPath;
            DT_THROW_IF(!testDevPath.from_string(mdev_path),
                        std::logic_error,
                        "Invalid device path syntax '" << mdev_path << "'!");
            DT_THROW_IF(testDevPath.get_setup() != _devices_->get_setup_label(),
                        std::logic_error,
                        "Invalid setup '" << testDevPath.get_setup() << "' device path!");
          }
          
          {
            std::ostringstream mdev_selection_key_ss;
            mdev_selection_key_ss << "mounted_devices." << mdev_label << ".selection";
            if (config_.has_key(mdev_selection_key_ss.str())) {
              mdev_selection = config_.fetch_string(mdev_selection_key_ss.str());
            }            
          }
          
          add_mounted_device(mdev_path, mdev_selection);
        }
      }

      if (config_.has_key("auto_connect")) {
        bool auto_connect = config_.fetch_boolean("auto_connect");
        set_auto_connect(auto_connect);
      }
 
      if (config_.has_key("max_connection_attempts")) {
        std::size_t max_connect_attempts = config_.fetch_positive_integer("max_connection_attempts");
        set_max_connection_attempts(max_connect_attempts);
      }
      
      if (config_.has_key("subcontractor_login")) {
        _sc_login_ = config_.fetch_string("subcontractor_login");
      }
      
      if (config_.has_key("subcontractor_password")) {
        _sc_password_ = config_.fetch_string("subcontractor_password");
      }
 
      _at_init_();
      
      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
    
    void sc_info::_at_init_()
    {
      std::string sc_name = get_name();
      std::string sc_sys_domain_name
        = vire::com::domain_builder::build_cms_subcontractor_system_name(_com_->get_domain_maker().get_domain_name_prefix(),
                                                                         sc_name);
      if (!_com_->has_domain(sc_sys_domain_name)) {
        vire::com::domain & sc_sys_domain = _com_->create_domain(sc_sys_domain_name,
                                                                 vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM,
                                                                 _com_->get_default_transport_type_id(),
                                                                 _com_->get_default_encoding_type_id()); 
        _com_->get_domain_maker().build_subcontractor_system_domain(sc_sys_domain, sc_name);
      }
      
      if (_svr_login_.empty()) {
        _svr_login_ = "vireserver";
      }
      if (_svr_password_.empty()) {
        _svr_password_ = "vireserver";
      }
      
      if (_sc_login_.empty()) {
        _sc_login_ = get_name();
      }
      if (_sc_password_.empty()) {
        _sc_password_ = _sc_login_;
      }
      _com_->create_actor(_svr_login_,
                          _svr_password_,
                          vire::com::ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM,
                          get_name());
      _com_->create_actor(_sc_login_,
                          _sc_password_,
                          vire::com::ACTOR_CATEGORY_SUBCONTRACTOR,
                          get_name());
      return;
    }
    
    void sc_info::_at_reset_()
    {
      // Clear server actors:
      if (_com_->has_actor(_sc_login_)) {
        _com_->remove_actor(_sc_login_);
      }
      if (_com_->has_actor(_svr_login_)) {
        _com_->remove_actor(_svr_login_);
      }
      {
        std::string sc_name = get_name();
        std::string sc_sys_domain_name
          = vire::com::domain_builder::build_cms_subcontractor_system_name(_com_->get_domain_maker().get_domain_name_prefix(),
                                                                           sc_name);
        if (_com_->has_domain(sc_sys_domain_name)) {
          _com_->remove_domain(sc_sys_domain_name);
        }
      }
      return;
    }

    void sc_info::reset()
    {
     DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(),
                  std::logic_error,
                  "Subcontractor '" << get_name() << "' is not initialized!");
      if (is_connected()) {
        disconnect();
      }
      _connected_ = false;

      _at_reset_();

      _initialized_ = false;
      _mounted_devices_.clear();
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    bool sc_info::has_connection_start_time() const
    {
      return _connection_start_time_ > vire::time::system_epoch();
    }
                        
    const vire::time::system_time_point & sc_info::get_connection_start_time() const
    {
       return _connection_start_time_;
    }

    bool sc_info::can_reconnect() const
    {
      return has_max_connection_attempts()
        && (_number_of_failed_connection_attempts_ < get_max_connection_attempts());
    }

    std::size_t sc_info::get_number_of_failed_connection_attempts() const
    {
      return _number_of_failed_connection_attempts_;
    }

    const std::set<std::string> & sc_info::get_resource_path() const
    {
      sc_info * mutable_this = const_cast<sc_info*>(this);
      if (!_resource_paths_) {
        DT_LOG_DEBUG(get_logging_priority(),
                     "Compute the list of resources associated to subcontractor '"
                     << get_name() << "'...");
        mutable_this->_resource_paths_.reset(new std::set<std::string>);
        std::set<int32_t> resource_ids;
        _resources_->build_set_of_resource_ids_from_responsible(get_name(), resource_ids);
        for (auto id : resource_ids) {
          const vire::resource::resource & res = _resources_->get_resource_by_id(id);
          _resource_paths_->insert(res.get_path());
        }
      }
      return *_resource_paths_.get();
    }

    void sc_info::connect()
    {
      bool connection_success = false;
      
      // Perform the connection ops:
      auto connReqPtr = std::make_shared<vire::cms::connection_request>();
      const vire::utility::instance_identifier & setupId = _devices_->get_setup_id();
      connReqPtr->set_setup_id(setupId);
      for (const std::string & resPath : this->get_resource_path()) {
         connReqPtr->add_requested_resource(resPath);
      }

      DT_LOG_DEBUG(get_logging_priority(),
                   "List of requested resources for subcontractor '" << get_name() << "':");
      if (datatools::logger::is_debug(get_logging_priority())) {
        for (const auto & res_path : connReqPtr->get_requested_resources()) {
          std::cerr << " --> " << res_path << std::endl;
        }
      }

      // Connection protocol:
      if (_com_->has_actor(_svr_login_)) {
        const vire::com::actor & serverActor = _com_->get_actor(_svr_login_);
        std::shared_ptr<vire::com::i_service_client_plug> clientPlugPtr =
          std::dynamic_pointer_cast<vire::com::i_service_client_plug>(serverActor.get_plug("subcontractor.service.client"));
        std::string address = vire::com::system_connection_key();
        // address = "";
        vire::utility::const_payload_ptr_type connRespPtr;
        double timeout = default_connection_timeout();
        timeout = -1.0; // No timeout
        clientPlugPtr->set_logging(datatools::logger::PRIO_DEBUG);
        vire::com::rpc_status status = clientPlugPtr->send_receive(address, connReqPtr, connRespPtr, timeout);
        if (status != vire::com::RPC_STATUS_SUCCESS) {
          if (status == vire::com::RPC_STATUS_TIMEOUT) {
            std::cerr << "TEST>>> CONNECTION TIMEOUT!" << std::endl;
          } else {
            std::cerr << "TEST>>> CONNECTION FAILURE!" << std::endl;
          }
        } else {
          connection_success = false;
          connRespPtr->tree_dump(std::cerr, "RESPONSE:", "TEST>>> ");
      
          {
            std::shared_ptr<const vire::cms::connection_success> connSuccessPtr
              = std::dynamic_pointer_cast<const vire::cms::connection_success>(connRespPtr);
            if (connSuccessPtr) {
              std::cerr << "******** Found vire::cms::connection_success" << std::endl;
              const vire::cms::connection_success & connSuccess = *connSuccessPtr;
              connSuccess.tree_dump(std::cerr, "Connection success:", "[debug] ");
              connection_success = true;
            } else {
              std::cerr << "******** Could not find vire::cms::connection_success" << std::endl;
            }
          }
          
          if (!connection_success) {
            std::shared_ptr<const vire::cms::connection_failure> connFailurePtr
              = std::dynamic_pointer_cast<const vire::cms::connection_failure>(connFailurePtr);
            if (connFailurePtr) {
              std::cerr << "******** Found vire::cms::connection_failure" << std::endl;
              const vire::cms::connection_failure & connFailure = *connFailurePtr;
              connFailure.tree_dump(std::cerr, "Connection failure:", "[debug] ");
              connection_success = false;
            } else {
              std::cerr << "******** Could not find vire::cms::connection_failure" << std::endl;
            }
          }
          
        }
      } else {
        std::cerr << "****  --> no actor " << _svr_login_ << std::endl;
      }
       
      if (connection_success) {
        _connection_start_time_ = std::chrono::system_clock::now();
        set_connected(true);
        _number_of_failed_connection_attempts_ = 0;
        DT_LOG_DEBUG(get_logging_priority(),
                     "Subcontractor '" << get_name() << "' is connected!");
      } else {
        _number_of_failed_connection_attempts_++;
      }
      return;
    }
    
    void sc_info::disconnect()
    {
      bool disconnection_success = false;

      // Perform the connection ops:
      auto disconnReqPtr = std::make_shared<vire::cms::disconnection_request>();
      const vire::utility::instance_identifier & setupId = _devices_->get_setup_id();
      disconnReqPtr->set_setup_id(setupId);

      bool do_disconnect = true;
      //do_disconnect = false; // Hack to skip disconnection protocol
      // Disconnection protocol:
      if (do_disconnect && _com_->has_actor(_svr_login_)) {
        const vire::com::actor & serverActor = _com_->get_actor(_svr_login_);
        std::shared_ptr<vire::com::i_service_client_plug> clientPlugPtr =
          std::dynamic_pointer_cast<vire::com::i_service_client_plug>(serverActor.get_plug("subcontractor.service.client"));
        std::string address = vire::com::system_connection_key();
        // address = "";
        vire::utility::const_payload_ptr_type disconnRespPtr;
        double timeout = default_connection_timeout();
        timeout = -1.0; // No timeout
        clientPlugPtr->set_logging(datatools::logger::PRIO_DEBUG);
        vire::com::rpc_status status = clientPlugPtr->send_receive(address, disconnReqPtr, disconnRespPtr, timeout);
        if (status != vire::com::RPC_STATUS_SUCCESS) {
          if (status == vire::com::RPC_STATUS_TIMEOUT) {
            std::cerr << "TEST>>> DISCONNECTION TIMEOUT!" << std::endl;
          } else {
            std::cerr << "TEST>>> DISCONNECTION FAILURE!" << std::endl;
          }
        } else {
          disconnection_success = false;
          disconnRespPtr->tree_dump(std::cerr, "RESPONSE:", "TEST>>> ");
      
          {
            std::shared_ptr<const vire::cms::disconnection_success> disconnSuccessPtr
              = std::dynamic_pointer_cast<const vire::cms::disconnection_success>(disconnRespPtr);
            if (disconnSuccessPtr) {
              std::cerr << "******** Found vire::cms::disconnection_success" << std::endl;
              const vire::cms::disconnection_success & disconnSuccess = *disconnSuccessPtr;
              disconnSuccess.tree_dump(std::cerr, "Disconnection success:", "[debug] ");
              disconnection_success = true;
            } else {
              std::cerr << "******** Could not find vire::cms::disconnection_success" << std::endl;
            }
          }
          
          if (!disconnection_success) {
            std::shared_ptr<const vire::cms::disconnection_failure> disconnFailurePtr
              = std::dynamic_pointer_cast<const vire::cms::disconnection_failure>(disconnFailurePtr);
            if (disconnFailurePtr) {
              std::cerr << "******** Found vire::cms::disconnection_failure" << std::endl;
              const vire::cms::disconnection_failure & disconnFailure = *disconnFailurePtr;
              disconnFailure.tree_dump(std::cerr, "Disconnection failure:", "[debug] ");
              disconnection_success = false;
            } else {
              std::cerr << "******** Could not find vire::cms::disconnection_failure" << std::endl;
            }
          }
          
        }
      } else {
        std::cerr << "****  --> no explicit disconnection was done" << std::endl;
      }
  
      if (disconnection_success) {
        _last_disconnection_time_ = std::chrono::system_clock::now();
        set_connected(false);
        _connection_start_time_ = vire::time::system_epoch();
        DT_LOG_DEBUG(get_logging_priority(),
                     "Subcontractor '" << get_name() << "' is disconnected!");
      }
      return;
    }

    void sc_info::_on_connect_()
    {
      _grab_emitter_().emit_connection_change();
      return;
    }
    
    void sc_info::_on_disconnect_()
    {
      _grab_emitter_().emit_connection_change();    
      return;
    }

    const sc_info::device_info & sc_info::get_mounted_device_info(const std::string & device_path_) const
    {
      std::map<std::string, device_info>::const_iterator found = _mounted_devices_.find(device_path_);
      DT_THROW_IF(found == _mounted_devices_.end(),
                  std::logic_error,
                  "No mounted device with path '" << device_path_ << "'!");
      return found->second;
    }
 
    sc_info::device_info & sc_info::grab_mounted_device_info(const std::string & device_path_)
    {
      std::map<std::string, device_info>::iterator found = _mounted_devices_.find(device_path_);
      DT_THROW_IF(found == _mounted_devices_.end(),
                  std::logic_error,
                  "No mounted device with path '" << device_path_ << "'!");
      return found->second;
    }
 
    void sc_info::print_tree(std::ostream & out_,
                             const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      datatools::enriched_base::tree_dump(out_, popts.title, popts.indent, true);

      out_ << popts.indent << tag
           << "Device manager    : ";
      if (_devices_) {
        out_ << '@' << _devices_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << tag
           << "Resource manager  : ";
      if (_resources_) {
        out_ << '@' << _resources_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << tag
           << "Auto connect      : " << std::boolalpha << is_auto_connect() << std::endl;

      out_ << popts.indent << tag
           << "Max connection    : ";
      if (has_max_connection_attempts()) {
        out_ << get_max_connection_attempts();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << tag
           << "Mounted devices   : " << _mounted_devices_.size() << std::endl;

      if (_mounted_devices_.size()) {
        std::size_t count = 0;
        for (const auto & p : _mounted_devices_) {
          const std::string & dev_path = p.first;
          const device_info & dev_info = p.second;
          out_ <<  popts.indent << skip_tag;
          if (++count == _mounted_devices_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Device: '" << dev_path << "' : " << dev_info.status;
          out_ << std::endl;
        }
      }

      out_ << popts.indent << tag
           << "Connected   : " << std::boolalpha << is_connected() << std::endl;

      // out_ << popts.indent << tag
      //      << "Connection time   : " << _connection_start_time_ << std::endl;

      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Initialized : " << std::boolalpha << is_initialized() << std::endl;

      return;
    }

  } // namespace cmsserver

} // namespace vire
