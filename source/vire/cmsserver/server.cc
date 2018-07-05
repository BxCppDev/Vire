// /vire/cmsserver/server.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/server.h>

// Standard library:
#include <thread>
#include <chrono>

// Third party:
// - Boost:
#include <boost/tokenizer.hpp>
// - Bayeux:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/resource_files.h>
#include <vire/logging/simple_logging_service.h>
#include <vire/com/manager.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>
#include <vire/resource/resource.h>
#include <vire/user/manager.h>
#include <vire/auth/manager.h>
#include <vire/cmsserver/sc_manager.h>
#include <vire/cmsserver/sc_info.h>
// #include <vire/cmsserver/session_manager.h>
// #include <vire/cmsserver/gate.h>
// #include <vire/cmsserver/agenda.h>
#include <vire/utility/path.h>

namespace vire {

  namespace cmsserver {

    /* Qt signal emitter */

    server_signal_emitter::server_signal_emitter(server & server_)
      : _server_(server_)
    {
      return;
    }

    const server & server_signal_emitter::get_server() const
    {
      return _server_;
    }
      
    server & server_signal_emitter::grab_server() const
    {
      return _server_;
    }

    void server_signal_emitter::emit_stop()
    {
      emit sig_stop();
    }

    /* CMS server PIMPL */
    struct server::pimpl_type
    {
      pimpl_type()
      {
        return;
      }

      vire::logging::simple_logging_service * log = nullptr;
      vire::com::manager * com = nullptr;
      const vire::user::manager * users = nullptr;
      const vire::auth::manager * auth = nullptr;
      const vire::device::manager * devices = nullptr;
      const vire::resource::manager * resources = nullptr;
      sc_manager * sc = nullptr;

      std::map<std::string, std::thread> runners;
     
    };
    
    /* CMS server */

    server_signal_emitter & server::grab_emitter()
    {
      if (!_emitter_) {
        _emitter_.reset(new server_signal_emitter(*this));
      }
      return *_emitter_.get();
    }

    const server_signal_emitter & server::get_emitter() const
    {
      server * mutable_this = const_cast<server*>(this);
      return mutable_this->grab_emitter();
    }

    // static
    std::string server::log_service_name()
    {
      static const std::string _n("log");
      return _n;
    }

    // static
    std::string server::sc_service_name()
    {
      static const std::string _n("sc");
      return _n;
    }

    // static
    std::string server::devices_service_name()
    {
      static const std::string _n("devices");
      return _n;
    }

    // static
    std::string server::resources_service_name()
    {
      static const std::string _n("resources");
      return _n;
    }

    // static
    std::string server::users_service_name()
    {
      static const std::string _n("users");
      return _n;
    }

    // static
    std::string server::auth_service_name()
    {
      static const std::string _n("auth");
      return _n;
    }

    // static
    std::string server::com_service_name()
    {
      static const std::string _n("com");
      return _n;
    }

    // static
    std::string server::sessions_service_name()
    {
      static const std::string _n("sessions");
      return _n;
    }

    // static
    std::string server::agenda_service_name()
    {
      static const std::string _n("agenda");
      return _n;
    }

    // static
    std::string server::pubsub_service_name()
    {
      static const std::string _n("pubsub");
      return _n;
    }

    // static
    std::string server::control_service_name()
    {
      static const std::string _n("control");
      return _n;
    }

    // static
    std::string server::clients_service_name()
    {
      static const std::string _n("clients");
      return _n;
    }

    // static
    std::string server::gate_service_name()
    {
      static const std::string _n("gate");
      return _n;
    }

    server::server()
    {
      _rc_.set_status(vire::running::RUN_STATUS_UNDEF);
      _pimpl_.reset(new pimpl_type);
      return;
    }

    server::~server()
    {
      if (is_initialized()) {
        reset();
      }
      _pimpl_.reset();
      return;
    }

    datatools::logger::priority server::get_logging() const
    {
      return _logging_;
    }

    void server::set_logging(const datatools::logger::priority l_)
    {
      _logging_ = l_;
      return;
    }

    bool server::has_setup_id() const
    {
      return _setup_id_.is_valid();
    }

    const ::vire::utility::instance_identifier & server::get_setup_id() const
    {
      return _setup_id_;
    }

    void server::set_setup_id(const ::vire::utility::instance_identifier & setup_id_)
    {
      _setup_id_ = setup_id_;
      return;
    }

    bool server::has_cms_top_path() const
    {
      return !_cms_top_path_.empty();
    }

    void server::set_cms_top_path(const std::string & path_)
    {
      std::string setup;
      std::vector<std::string> segments;
      DT_THROW_IF(!vire::utility::path::extract(path_, setup, segments),
                  std::logic_error,
                  "Invalid CMS top path '" << path_ << "'!");
      if (!has_setup_id()) {
        vire::utility::instance_identifier sid;
        DT_THROW_IF(!sid.from_string(setup), std::logic_error,
                    "Invalid setup ID '" << setup << "'!");
        set_setup_id(sid);
      } else {
        DT_THROW_IF(_setup_id_.get_name() != setup,
                    std::logic_error,
                    "Unmatching CMS top path scheme  '"
                    << setup << "' and setup ID '" << _setup_id_.get_name() << "'!");
      }
      _cms_top_path_ = path_;
      return;
    }

    const std::string & server::get_cms_top_path() const
    {
      return _cms_top_path_;
    }

    const std::string & server::get_dll_load_default_path() const
    {
      return _dll_load_default_path_;
    }

    void server::set_dll_load_default_path(const std::string & p_)
    {
      _dll_load_default_path_ = p_;
      return;
    }

    void server::add_dll_load_directive(const std::string & dlr_)
    {
      _dll_load_directives_.push_back(dlr_);
      return;
    }

    std::vector<std::string> server::get_dll_load_directives() const
    {
      return _dll_load_directives_;
    }

    datatools::service_manager & server::grab_services()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Server is not initialized!");
      return *_services_.get();
    }

    const datatools::service_manager & server::get_services() const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Server is not initialized!");
      return *_services_.get();
    }

    bool server::is_initialized() const
    {
      return _initialized_;
    }

    void server::initialize(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(is_initialized(), std::logic_error, "Server is already initialized!");

      _at_init_(config_);

      DT_LOG_NOTICE(_logging_, "Starting pre system services...");
      _start_pre_system_services();

      DT_LOG_NOTICE(_logging_, "Starting business services...");
      _start_business_services();

      DT_LOG_NOTICE(_logging_, "Starting post system services...");
      // _start_post_system_services();

      {
        const datatools::service_dict_type & services_dict = _services_->get_bus_of_services();
        DT_LOG_TRACE(_logging_, "Service dict size = " << services_dict.size());
        for (const auto & p : services_dict) {
          DT_LOG_TRACE(_logging_, "Key='" << p.first << "'");
        }
      }

      _initialized_ = true;
      _rc_.set_status(vire::running::RUN_STATUS_READY);

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::reset()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(!is_initialized(), std::logic_error, "Server is not initialized!");

      {
        if (_rc_.is_running()) {
          DT_LOG_NOTICE(_logging_, "Forcing run stop...");
          _rc_.request_stop();
        }
      }
 
      _initialized_ = false;

      DT_LOG_NOTICE(_logging_, "Stopping post system services...");
      _stop_post_system_services();

      DT_LOG_NOTICE(_logging_, "Stopping business services...");
      _stop_business_services();

      DT_LOG_NOTICE(_logging_, "Stopping pre system services...");
      _stop_pre_system_services();

      _at_reset_();

      _reset_core_();
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::tree_dump(std::ostream & out_,
                           const std::string & title_,
                           const std::string & indent_,
                           bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Logging : '" << datatools::logger::get_priority_label(_logging_) << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Setup ID : '" << _setup_id_.to_string() << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "DLL load default path : '" << _dll_load_default_path_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "DLL load directives : " << _dll_load_directives_.size() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Services configuration : " << std::endl;
      {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
        _mconfig_.tree_dump(out_, "", indent2.str());
      }

      if (is_initialized()) {
        out_ << indent_ << datatools::i_tree_dumpable::tag
             << "Services : " << std::endl;
        {
          std::ostringstream indent2;
          indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
          _services_->tree_dump(out_, "", indent2.str());
        }
      }

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Initialized : "
           << std::boolalpha << _initialized_ << std::endl;

      return;
    }

    void server::_reset_dlls_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

     _dll_loader_.reset();
     _dll_load_directives_.clear();
     _dll_load_default_path_.clear();

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_init_dlls_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      if (_dll_load_default_path_.empty()) {
        std::string dll_defaut_path;
        if (config_.has_key("dll.load_defaut_path")) {
          std::string p = config_.fetch_string("dll.load_defaut_path");
          set_dll_load_default_path(p);
        }
      }
      if (_dll_load_default_path_.empty()) {
        // const vire::detail::vire_library & vrlib = vire::detail::vire_library::const_instance();
        // const datatools::service_manager & vrLibServices = vrlib.get_services();
        // datatools::kernel & krnl = datatools::kernel::instance();
        // const datatools::library_info & lib_info_reg = krnl.get_library_info_register();
        // if (lib_info_reg.has("vire")) {

        // }
        _dll_load_default_path_ = vire::get_plugin_lib_dir();
      }

      if (config_.has_key("dll.load")) {
        std::vector<std::string> dll_load_items;
        config_.fetch("dll.load", dll_load_items);
        for (auto item : dll_load_items) {
          add_dll_load_directive(item);
        }
      }

      _dll_loader_.reset(new datatools::library_loader);

      for (const std::string & dll_load_rule : _dll_load_directives_) {
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer_type;
        boost::char_separator<char> sep("@");
        tokenizer_type tokens(dll_load_rule, sep);
        std::string dll_name;
        std::string dll_path;
        for(tokenizer_type::iterator it = tokens.begin(); it != tokens.end(); ++it) {
          if (dll_name.empty()) dll_name = *it;
          else if (dll_path.empty()) dll_path = *it;
        }
        if (dll_path.empty() && !_dll_load_default_path_.empty()) {
          dll_path = _dll_load_default_path_;
        }
        DT_LOG_NOTICE(_logging_, "Loading DLL '" << dll_name << "' from '" << dll_path << "'...");
        int error = _dll_loader_->load(dll_name, dll_path);
        if (error != EXIT_SUCCESS) {
          DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS, "Loading DLL '" << dll_name << "' failed!");
        } else {
          if (_dll_loader_->is_loaded(dll_name)) {
            DT_LOG_NOTICE(_logging_, "DLL '" << dll_name << "' is loaded.");
          }
        }
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_init_core_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      if (!has_setup_id()) {
        // Fetch requested setup ID:
        if (config_.has_key("setup_id")) {
          vire::utility::instance_identifier sid;
          const std::string & sid_repr = config_.fetch_string("setup_id");
          DT_THROW_IF(!sid.from_string(sid_repr), std::logic_error,
                      "Invalid setup ID '" << sid_repr<< "'!");
          set_setup_id(sid);
        }
      }

      DT_THROW_IF (!has_setup_id(), std::logic_error, "Missing setup ID!");

      if (!has_cms_top_path()) {
        // Fetch requested CMS top path:
        if (config_.has_key("cms_top_path")) {
          const std::string & mp = config_.fetch_string("cms_top_path");
          set_cms_top_path(mp);
        }
      }

      if (!has_cms_top_path()) {
        std::ostringstream cms_top_path_out;
        cms_top_path_out << vire::utility::path::build(_setup_id_.get_name(),
                                                       "",
                                                       "CMS");
        DT_LOG_DEBUG(_logging_, "Use a default CMS top path...");
        set_cms_top_path(cms_top_path_out.str());
      }

      DT_LOG_NOTICE(_logging_, "Loading services configuration...");

      // Fetch configuration files for pre system services:
      {
        std::vector<std::string> services_pre_system_config_paths;
        if (config_.has_key("services.pre_system")) {
          config_.fetch("services.pre_system", services_pre_system_config_paths);
        }
        uint32_t read_options = datatools::multi_properties::config::RESOLVE_PATH;
        for (const std::string & path : services_pre_system_config_paths) {
          _mconfig_.read(path, read_options);
        }
      }

      // Fetch configuration files for business services:
      {
        std::vector<std::string> services_business_config_paths;
        if (config_.has_key("services.business")) {
          config_.fetch("services.business", services_business_config_paths);
        }
        uint32_t read_options = datatools::multi_properties::config::RESOLVE_PATH;
        for (const std::string & path : services_business_config_paths) {
          _mconfig_.read(path, read_options);
        }
      }

      // Fetch configuration files for post system services:
      {
        std::vector<std::string> services_post_system_config_paths;
        if (config_.has_key("services.post_system")) {
          config_.fetch("services.post_system", services_post_system_config_paths);
        }
        uint32_t read_options = datatools::multi_properties::config::RESOLVE_PATH;
        for (const std::string & path : services_post_system_config_paths) {
          _mconfig_.read(path, read_options);
        }
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_reset_core_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      _mconfig_.clear();
      _setup_id_.reset();

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_at_init_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      // Fetch logging priority:
      datatools::logger::priority prio =
        datatools::logger::extract_logging_configuration(config_,
                                                         _logging_,
                                                         false);

      // 
      _init_dlls_(config_);
      _init_core_(config_);

      // Initialize the service manager:
      DT_LOG_NOTICE(_logging_, "Initializing the service manager...");
      datatools::logger::priority services_logging = datatools::logger::PRIO_FATAL;
      if (config_.has_key("service_manager.logging.priority")) {
        datatools::properties service_manager_config;
        config_.export_and_rename_starting_with(service_manager_config, "service_manager.", "");
        services_logging
          = datatools::logger::extract_logging_configuration(service_manager_config,
                                                             services_logging,
                                                             false);
      }
      _services_.reset(new datatools::service_manager());
      _services_->set_name("CMSServerServices");
      _services_->set_logging_priority(services_logging);
      _services_->set_description("CMS server service manager");
      _services_->set_allow_dynamic_services(true);
      _services_->set_force_initialization_at_load(true);
      _services_->initialize();

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_at_reset_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      // Terminate the service manager:
      if (_services_ && _services_->is_initialized()) {
        DT_LOG_NOTICE(_logging_, "Terminating the service manager...");
        _services_->reset();
      }
      _services_.reset();

      _reset_core_();
      _reset_dlls_();

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_start_pre_system_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      // Log:
      DT_LOG_TRACE(_logging_, "Log...");
      datatools::properties log_config;
      if (_mconfig_.has_section("log")) {
        log_config = _mconfig_.get_section("log");
      }
      _services_->load(log_service_name(),
                      "vire::logging::simple_logging_service",
                      log_config);
      if (datatools::logger::is_trace(_logging_)) {
        _services_->tree_dump(std::cerr, "Services:", "[trace] ");
      }
      DT_LOG_TRACE(_logging_, "log service is setup.");
      vire::logging::simple_logging_service & log
        = _services_->grab<vire::logging::simple_logging_service>(log_service_name());
      if (datatools::logger::is_trace(_logging_)) {
        log.tree_dump(std::cerr, "Log service:", "[trace] ");
      }
      _services_->sync();
      _pimpl_->log = &_services_->grab<vire::logging::simple_logging_service>(log_service_name());

      // Com manager:
      DT_LOG_TRACE(_logging_, "Com manager...");
      datatools::properties com_config;
      if (_mconfig_.has_section("com")) {
        com_config = _mconfig_.get_section("com");
      }
      DT_LOG_DEBUG(get_logging(), "Loading 'com' service...");
      _services_->load(com_service_name(),
                      "vire::com::manager",
                      com_config);
      if (datatools::logger::is_trace(_logging_)) {
        _services_->tree_dump(std::cerr, "Services:", "[trace] ");
      }
      DT_LOG_TRACE(_logging_, "com service is setup.");
      vire::com::manager & com = _services_->grab<vire::com::manager>(com_service_name());
      // vire::com::actor server_actor("vire.cmsserver", vire::com::actor::CATEGORY_SERVER);
      // com.set_actor(server_actor);
      if (datatools::logger::is_trace(_logging_)) {
        com.tree_dump(std::cerr, "Com service:", "[trace] ");
      }
      _services_->sync();
      _pimpl_->com = &_services_->grab<vire::com::manager>(com_service_name());
      
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_start_post_system_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      // Session manager:
      /*
      DT_LOG_TRACE(_logging_, "Session manager...");
      datatools::properties session_config;
      if (_mconfig_.has_section("session")) {
        session_config = _mconfig_.get_section("session");
      }
      vire::cmsserver::session_manager & session =
        dynamic_cast<vire::cmsserver::session_manager&>(_services_->load_no_init(session_service_name(),
                                                                                "vire::cmsserver::session_manager"));
      session.set_logging(this->get_logging());
      session.set_server(*this);
      session.initialize(session_config,
                         const_cast<datatools::service_dict_type&>(_services_->get_bus_of_services()));

      {
        const datatools::service_dict_type & services_dict = _services_->get_bus_of_services();
        DT_LOG_TRACE(_logging_, "Session dict size = " << services_dict.size());
        for (const auto & p : services_dict) {
          DT_LOG_TRACE(_logging_, "Key='" << p.first << "'");
        }
      }
      DT_LOG_TRACE(_logging_, "session service is setup.");
      _services_->sync();
      */

      /*
      // RabbitMQ management service:
      DT_LOG_TRACE(_logging_, "RabbitMQ management service...");
      datatools::properties rabbitmq_config;
      if (_mconfig_.has_section("rabbitmq")) {
        rabbitmq_config = _mconfig_.get_section("rabbitmq");
      }
      std::string rabbitmq_service_name = "RabbitMgr";
      _services_->load(rabbitmq_service_name,
                      "vire::rabbitmq::manager_service",
                      rabbitmq_config);
      if (datatools::logger::is_trace(_logging_)) {
        _services_->tree_dump(std::cerr, "Services:", "[trace] ");
      }
      DT_LOG_TRACE(_logging_, "RabbitMQ management service is setup.");
      datatools::base_service & srvc = _services_->grab_service(rabbitmq_service_name);
      if (datatools::logger::is_trace(_logging_)) {
        srvc.tree_dump(std::cerr, "RabbitMQ management service:", "[trace] ");
      }
      // vire::rabbitmq::manager_service & rabbitmq = _services_.grab<vire::rabbitmq::manager_service>(rabbitmq_service_name);
      // if (datatools::logger::is_trace(_logging_)) {
      //   rabbitmq.tree_dump(std::cerr, "RabbitMQ management service:", "[trace] ");
      // }
      _services_->sync();
      */

      // // Agenda:
      // DT_LOG_TRACE(_logging_, "Agenda...");
      // datatools::properties agenda_config;
      // if (_mconfig_.has_section("agenda")) {
      //   agenda_config = _mconfig_.get_section("agenda");
      // }
      // vire::cmsserver::agenda & agenda =
      //   dynamic_cast<vire::cmsserver::agenda &>(_services_->load_no_init(agenda_service_name(),
      //                                                                   "vire::cmsserver::agenda"));
      // agenda.set_logging_priority(this->get_logging());
      // agenda.initialize(agenda_config,
      //                   const_cast<datatools::service_dict_type&>(_services_->get_bus_of_services()));

      // {
      //   const datatools::service_dict_type & services_dict = _services_->get_bus_of_services();
      //   DT_LOG_TRACE(_logging_, "Session service size = " << services_dict.size());
      //   for (const auto & p : services_dict) {
      //     DT_LOG_TRACE(_logging_, "Key='" << p.first << "'");
      //   }
      // }
       _services_->sync();

     //  // Gate:
     //  DT_LOG_TRACE(_logging_, "Gate...");
     //  datatools::properties gate_config;
     //  if (_mconfig_.has_section("gate")) {
     //    gate_config = _mconfig_.get_section("gate");
     //  }
     //  _services_->load(gate_service_name(),
     //                  "vire::cmsserver::gate",
     //                  gate_config);
     //  if (datatools::logger::is_trace(_logging_)) {
     //    _services_->tree_dump(std::cerr, "Services:", "[trace] ");
     //  }
     //  DT_LOG_TRACE(_logging_, "gate service is setup.");
     //  vire::cmsserver::gate & gate = _services_->grab<vire::cmsserver::gate>(gate_service_name());
     //  if (datatools::logger::is_trace(_logging_)) {
     //    gate.tree_dump(std::cerr, "Gate service:", "[trace] ");
     //  }
     //  _services_->sync();

     // _services_->tree_dump(std::cerr, "Vire CMS server's service manager:", "[trace] ");

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_stop_post_system_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      // if (_services_->has(gate_service_name())) {
      //   if (_services_->is_a<vire::cmsserver::gate>(gate_service_name())) {
      //     _services_->drop(gate_service_name());
      //   }
      // }

      // if (_services_->has(sessions_service_name())) {
      //        if (_services_->is_a<vire::cmsserver::session_manager>(sessions_service_name())) {
      //          _services_->drop(sessions_service_name());
      //        }
      // }

      // if (_services_->has(agenda_service_name())) {
      //   if (_services_->is_a<vire::cmsserver::agenda>(agenda_service_name())) {
      //     _services_->drop(agenda_service_name());
      //   }
      // }

      // std::string rabbitmq_service_name = "RabbitMgr";
      // if (_services_->has(rabbitmq_service_name)) {
      //   _services_->drop(rabbitmq_service_name);
      // }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_stop_pre_system_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      if (_services_->has(com_service_name())) {
        if (_services_->is_a<vire::com::manager>(com_service_name())) {
          _services_->drop(com_service_name());
        }
      }

      if (_services_->has(log_service_name())) {
        if (_services_->is_a<vire::logging::simple_logging_service>(log_service_name())) {
          _services_->drop(log_service_name());
        }
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_start_business_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      // Devices manager:
      datatools::properties devices_config;
      if (_mconfig_.has_section("devices")) {
        devices_config = _mconfig_.get_section("devices");
      }
      DT_LOG_DEBUG(get_logging(), "Loading 'devices' service...");
      _services_->load(devices_service_name(),
                      "vire::device::manager",
                      devices_config);
      _services_->sync();
      _pimpl_->devices = &_services_->get<vire::device::manager>(devices_service_name());

      // Resources manager:
      datatools::properties resources_config;
      if (_mconfig_.has_section("resources")) {
        resources_config = _mconfig_.get_section("resources");
      }
      DT_LOG_DEBUG(get_logging(), "Loading 'resources' service...");
      _services_->load(resources_service_name(),
                      "vire::resource::manager",
                      resources_config);
      _services_->sync();
      _pimpl_->resources = &_services_->get<vire::resource::manager>(resources_service_name());
     
      // vire::resource::manager & resources = _services_->grab<vire::resource::manager>("resources");
      // resources.tree_dump(std::cerr, "Resources:", "[debug] ");

      // Users manager:
      datatools::properties users_config;
      if (_mconfig_.has_section("users")) {
        users_config = _mconfig_.get_section("users");
      }
      DT_LOG_DEBUG(get_logging(), "Loading 'users' service...");
      _services_->load(users_service_name(),
                      "vire::user::manager",
                      users_config);
      _services_->sync();
      _pimpl_->users = &_services_->grab<vire::user::manager>(users_service_name());
 
      // Auth manager:
      // DT_LOG_DEBUG(datatools::logger::PRIO_ALWAYS, "Configuring 'auth' service...");
      datatools::properties auth_config;
      if (_mconfig_.has_section("auth")) {
        auth_config = _mconfig_.get_section("auth");
        // auth_config.tree_dump(std::cerr,"Auth service config: ", "[devel] ");
       }
      DT_LOG_DEBUG(get_logging(), "Loading 'auth' service...");
      _services_->load(auth_service_name(),
                      "vire::auth::manager",
                      auth_config);
      _services_->sync();
      _pimpl_->auth = &_services_->grab<vire::auth::manager>(auth_service_name());

      // Subcontractor manager:
      // DT_LOG_DEBUG(datatools::logger::PRIO_ALWAYS, "Configuring 'sc' (Subcontractor) service...");
      datatools::properties sc_config;
      if (_mconfig_.has_section("sc")) {
        sc_config = _mconfig_.get_section("sc");
        // sc_config.tree_dump(std::cerr,"Subcontractor service config: ", "[devel] ");
      }
      DT_LOG_DEBUG(get_logging(), "Loading 'sc' service...");
      _services_->load(sc_service_name(),
                      "vire::cmsserver::sc_manager",
                      sc_config);
      _services_->sync();
      _pimpl_->sc = &_services_->grab<vire::cmsserver::sc_manager>(sc_service_name());
      DT_LOG_DEBUG(get_logging(), "Service 'sc' logging is : "
                   << datatools::logger::get_priority_label(_pimpl_->sc->get_logging_priority()));

      // Compute the 'responsible' of resources:
      _compute_resource_responsible_();

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void server::_compute_resource_responsible_()
    {
      vire::resource::manager & resources = const_cast<vire::resource::manager &>(*_pimpl_->resources);
      const vire::cmsserver::sc_manager & sc = *_pimpl_->sc;
      std::set<std::string> sc_names;
      sc.build_subcontractor_names(sc_names);
      for (const auto & sc_name : sc_names) {
        const sc_info & scInfo = sc.get_subcontractor_info(sc_name);
        std::set<std::string> mdevices;
        scInfo.build_mounted_devices(mdevices);
        for (const auto & dev_name : mdevices) {
          std::set<int32_t> resource_ids;
          const vire::cmsserver::sc_info::device_info & devInfo = scInfo.get_mounted_device_info(dev_name);
          resources.build_set_of_resource_ids_from_path_regexp_within_device(dev_name,
                                                                             devInfo.selection,
                                                                             resource_ids);
                                                                             
          for (auto res_id : resource_ids) {
            const vire::resource::resource & res = resources.get_resource_by_id(res_id);
            const_cast<vire::resource::resource &>(res).set_responsible(sc_name);
          }
        }
      }
      return;
    }
    
    void server::_stop_business_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      /*
      std::vector<std::string> list_of_services;
      _services_->build_list_of_services(list_of_services);

      std::set<std::string> list_of_business_services;
      for (const auto & service_name : list_of_services) {
        if (service_name == sessions_service_name()) continue;
        if (service_name == com_service_name()) continue;
        if (service_name == resources_service_name()) continue;
        if (service_name == devices_service_name()) continue;
        list_of_business_services.insert(service_name);
      }

      while (list_of_business_services.size()) {
        for (const auto & service_name : list_of_business_services) {
          if (_services_->has(service_name)) {
            if (_services_->can_drop(service_name)) {
              _services_->drop(service_name);
            }
          }
        }
      }
      */

      if (_services_->has(sc_service_name())) {
        if (_services_->is_a<sc_manager>(sc_service_name())) {
          _services_->drop(sc_service_name());
        }
      }

      if (_services_->has(auth_service_name())) {
        if (_services_->is_a<vire::auth::manager>(auth_service_name())) {
          _services_->drop(auth_service_name());
        }
      }

      if (_services_->has(users_service_name())) {
        if (_services_->is_a<vire::user::manager>(users_service_name())) {
          _services_->drop(users_service_name());
        }
      }

      if (_services_->has(resources_service_name())) {
        if (_services_->is_a<vire::resource::manager>(resources_service_name())) {
          _services_->drop(resources_service_name());
        }
      }

      if (_services_->has(devices_service_name())) {
        if (_services_->is_a<vire::device::manager>(devices_service_name())) {
          _services_->drop(devices_service_name());
        }
      }

      DT_LOG_TRACE_EXITING(_logging_);
    }

    const vire::running::run_control & server::get_rc() const
    {
      return _rc_;
    }

    void server::stop()
    {
      _rc_.request_stop();
      return;
    }

    void server::_at_run_start_()
    {
     DT_LOG_DEBUG(_logging_, "Starting run...");
     _rc_.set_status(vire::running::RUN_STATUS_STARTING);

     if (_pimpl_->sc) {
       DT_LOG_DEBUG(_logging_, "Starting the 'sc' manager...");
       std::thread t(&sc_manager::run, _pimpl_->sc);
       t.detach();
       _pimpl_->runners["sc"] = std::move(t);
     }
     
     DT_LOG_DEBUG(_logging_, "Run is started.");
     return;
    }

    void server::_at_run_stop_()
    {
      DT_LOG_DEBUG(_logging_, "Stopping run...");
      _rc_.set_status(vire::running::RUN_STATUS_STOPPING);
      for (std::map<std::string, std::thread>::iterator thd_iter = _pimpl_->runners.begin();
           thd_iter != _pimpl_->runners.end();
           thd_iter++) {
        const std::string & key = thd_iter->first;
        if (key == "sc") {
          DT_LOG_DEBUG(_logging_, "Stopping the 'sc' manager...");
          _pimpl_->sc->stop();
          std::this_thread::sleep_for(std::chrono::seconds(2));
        }
      }
      DT_LOG_DEBUG(_logging_, "Run is stopped.");
      return;
    }

    void server::run()
    {
      DT_LOG_DEBUG(_logging_, "Running...");
      DT_THROW_IF(!is_initialized(), std::logic_error, "CMS server is not initialized!");
      DT_THROW_IF(!_rc_.is_ready(), std::logic_error,
                  "Subcontractor manager is not ready to run!");
      if (_rc_.is_stop_requested()) {
        _rc_.set_status(vire::running::RUN_STATUS_STOPPED);
        return;
      }
      _rc_.set_start_time(std::chrono::system_clock::now());
      _at_run_start_();
      _at_run_();
      _at_run_stop_();
      _rc_.set_stop_time(std::chrono::system_clock::now());
      _rc_.set_status(vire::running::RUN_STATUS_STOPPED);
      grab_emitter().emit_stop();
      return;
    }

    void server::_at_run_()
    {
      _rc_.set_status(vire::running::RUN_STATUS_RUNNING);

      DT_LOG_DEBUG(_logging_, "Entering run loop...");
      while (!_rc_.is_stop_requested()) {
        vire::time::system_time_point tp_now = std::chrono::system_clock::now();
        vire::time::system_time_point tp_elapsed_tick = tp_now + _rc_.get_sys_tick();
        _at_run_loop_();
        _rc_.increment_loop_counter();
        tp_now = std::chrono::system_clock::now();
        if (tp_now < tp_elapsed_tick) {
          std::this_thread::sleep_until(tp_elapsed_tick);
        }
      }
       return;
    }

    void server::_at_run_loop_()
    {
      return;
    }

  } // namespace cmsserver

} // namespace vire
