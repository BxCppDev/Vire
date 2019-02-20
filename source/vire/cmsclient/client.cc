// /vire/cmsclient/client.cc
//
// Copyright (c) 2017-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2017-2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/cmsclient/client.h>

// Third party:
// - Bayeux:
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/com/manager.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>
#include <vire/cmsclient/setup_infos_tui.h>
#include <vire/cmsclient/negotiation_service.h>
// #include <vire/cmsclient/session_manager.h>

namespace vire {

  namespace cmsclient {

    struct client::pimpl_type
    {
      vire::com::manager               * com       = nullptr;
      const vire::device::manager      * devices   = nullptr;
      const vire::resource::manager    * resources = nullptr;
      vire::cmsclient::negotiation_service * nego = nullptr;       
      // vire::cmsclient::session_manager * session = nullptr;       
    };
    
    // static
    std::string client::nego_service_name()
    {
      static const std::string _n("nego");
      return _n;
    }
    
    // static
    std::string client::com_service_name()
    {
      static const std::string _n("com");
      return _n;
    }

    // static
    std::string client::log_service_name()
    {
      static const std::string _n("log");
      return _n;
    }

    // static
    std::string client::session_service_name()
    {
      static const std::string _n("session");
      return _n;
    }

    // static
    std::string client::devices_service_name()
    {
      static const std::string _n("devices");
      return _n;
    }

    // static
    std::string client::resources_service_name()
    {
      static const std::string _n("resources");
      return _n;
    }

    client::client()
    {
      _pimpl_.reset(new pimpl_type);
      return;
    }

    client::~client()
    {
      if (is_initialized()) {
        reset();
      }
      _pimpl_.reset();
      return;
    }

    bool client::has_ui_mode() const
    {
      return _ui_mode_ != UI_UNDEF;
    }

    void client::set_ui_mode(const ui_mode_type ui_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Client is already initialized!");
      _ui_mode_ = ui_;
      return;
    }

    client::ui_mode_type client::get_ui_mode() const
    {
      return _ui_mode_;
    }

    bool client::is_interactive() const
    {
      return _ui_mode_ == UI_TUI || _ui_mode_ == UI_GUI;
    }

    bool client::is_batch() const
    {
      return _ui_mode_ == UI_BATCH;
    }

    bool client::has_setup_infos() const
    {
      return _setup_infos_.is_valid();
    }

    void client::set_setup_infos(const setup_infos & si_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Client is already initialized!");
      _setup_infos_ = si_;
      return;
    }

    const setup_infos & client::get_setup_infos() const
    {
      return _setup_infos_;
    }

    setup_infos & client::grab_setup_infos()
    {
      return _setup_infos_;
    }

    datatools::service_manager & client::grab_services()
    {
      return _services_;
    }

    const datatools::service_manager & client::get_services() const
    {
      return _services_;
    }

    bool client::is_initialized() const
    {
      return _initialized_;
    }

    void client::initialize(const datatools::multi_properties & mconfig_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error, "Client is already initialized!");

      _mconfig_ = mconfig_;

      _at_init_();

      _start_system_services_();

      // _start_business_services_();

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void client::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error, "Client is not initialized!");
      _initialized_ = false;

      //_stop_business_services_();

      _stop_system_services_();

      _at_reset_();

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    bool client::in_setup() const
    {
      return _in_setup_;
    }

    void client::setup_enter()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Client is not initialized!");
      if ((has_setup_infos() && !get_setup_infos().is_valid())
          || !has_setup_infos()) {
        if (is_interactive()) {
          _setup_infos_.unlock();
          if (_ui_mode_ == UI_TUI || _ui_mode_ == UI_GUI) {
            setup_infos_tui setupInfosTui;
            if (! setupInfosTui.run(_setup_infos_)) {
            }
          }
          _setup_infos_.lock();
          if (datatools::logger::is_notice(get_logging_priority())) {
            _setup_infos_.tree_dump(std::clog, "Setup infos: ");
          }
        }
      }
      _in_setup_ = true;
      return;
    }

    void client::setup_quit()
    {
      DT_THROW_IF(!in_setup(), std::logic_error, "Client is not in-setup status!");
      _in_setup_ = false;

      // ...

      return;
    }

    void client::print_tree(std::ostream & out_,
                            const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      out_ << popts.indent << tag
           << "Configuration : " << std::endl;
      {
        std::ostringstream indent2;
        indent2 << popts.indent << skip_tag;
        _mconfig_.tree_dump(out_, "", indent2.str());
      }

      out_ << popts.indent << tag
           << "UI mode : " << _ui_mode_ << std::endl;

      out_ << popts.indent << tag
           << "Setup infos : " << std::endl;
      {
        std::ostringstream indent2;
        indent2 << popts.indent << skip_tag;
        _setup_infos_.tree_dump(out_, "", indent2.str());
      }

      out_ << popts.indent << tag
           << "Services : " << std::endl;
      {
        std::ostringstream indent2;
        indent2 << popts.indent << skip_tag;
        _services_.tree_dump(out_, "", indent2.str());
      }

      out_ << popts.indent << tag
           << "Initialized : "
           << std::boolalpha << _initialized_ << std::endl;

      out_ << popts.indent << inherit_tag(popts.inherit)
           << "In setup : "
           << std::boolalpha << _in_setup_ << std::endl;

      return;
    }

    void client::_init_main_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      datatools::enriched_base::initialize(config_, false);

      if (!has_ui_mode()) {
        if (config_.has_key("ui_mode")) {
          std::string ui_repr = config_.fetch_string("ui_mode");
          if (ui_repr == "batch") {
            set_ui_mode(UI_BATCH);
          } else if (ui_repr == "tui") {
            set_ui_mode(UI_TUI);
          } else if (ui_repr == "gui") {
            set_ui_mode(UI_GUI);
          } else {
            DT_THROW(std::logic_error, "Unsupported UI mode '" << ui_repr << "'!");
          }
        }
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void client::_reset_main_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      _setup_infos_.reset();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void client::_at_init_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      // Fetch general client parameters:
      datatools::properties main_config;
      if (_mconfig_.has_section("main")) {
        main_config = _mconfig_.get_section("main");
        _init_main_(main_config);
      }

      if (!has_setup_infos()) {
        // Fetch setup informations:
        datatools::properties setup_config;
        if (_mconfig_.has_section("setup")) {
          setup_config = _mconfig_.get_section("setup");
          _setup_infos_.initialize(setup_config);
        }
      }

      if (!_setup_infos_.has_gate_client_user_login()) {
        DT_THROW(std::logic_error, "No login is provided for client gate access!");
      }
      
      if (!_setup_infos_.has_gate_client_user_password()) {
        DT_THROW(std::logic_error, "No password is provided for client gate access!");
      }

      _services_.set_name("CMSClientServices");
      _services_.set_description("CMS client service bus manager");
      _services_.set_allow_dynamic_services(true);
      _services_.set_force_initialization_at_load(true);
      _services_.initialize();

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void client::_at_reset_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      _services_.reset();
      _setup_infos_.reset();
      _reset_main_();
      _mconfig_.reset();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    /*
    void client::_init_com_()
    {
      if (_pimpl_->com) {
        if (!_setup_infos_.has_gate_login()) {
          DT_THROW(std::logic_error, "No login is provided for client gate access!");
        }
        if (!_setup_infos_.has_gate_password()) {
          DT_THROW(std::logic_error, "No password is provided for client gate access!");
        }
        // Create actor profile:
        vire::com::actor_profile & client_gate_actor_profile =
          _pimpl_->com->create_actor_profile(get_setup_infos().get_gate_login(),
                                             get_setup_infos().get_gate_password(),
                                             vire::com::ACTOR_CATEGORY_CLIENT_GATE);
        _gate_actor_ = 
          client_gate_actor_profile.create_actor("connection");

        _pimpl_->com->create_actor(_setup_infos_.get_gate_login(),
                                   _setup_infos_.get_gate_password(),
                                   vire::com::ACTOR_CATEGORY_CLIENT_GATE);
      }
      return;
    }
    */

    /*
    void client::_terminate_com_()
    {
      if (_pimpl_->com) {
        if (_pimpl_->com->has_actor(_setup_infos_.get_gate_login())) {
          _pimpl_->com->remove_actor(_setup_infos_.get_gate_login());
        }
      }
      return;
    }
    */

    void client::_start_system_services_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      
      // Com manager:
      DT_LOG_DEBUG(get_logging_priority(), "Com manager...");
      datatools::properties com_config;
      if (_mconfig_.has_section(com_service_name())) {
        com_config = _mconfig_.get_section(com_service_name());
      }
      {
        vire::com::manager & com_handle =
          dynamic_cast<vire::com::manager&>(_services_.load_no_init(com_service_name(),
                                                                    "vire::com::manager"));
        com_handle.set_app_category(vire::cms::application::CATEGORY_CLIENT);
        com_handle.set_domain_name_prefix(get_setup_infos().get_domain_name_prefix());
        com_handle.set_default_transport_driver_type_id(get_setup_infos().get_transport_driver_type_id());
        com_handle.set_default_encoding_driver_type_id(get_setup_infos().get_encoding_driver_type_id());
        // com_handle.set_resource_service_name(resource_service_name());
      }
      _services_.configure_no_init(com_service_name(), com_config);
      DT_LOG_DEBUG(get_logging_priority(), "Com service is configured.");
      vire::com::manager & com = _services_.grab<vire::com::manager>(com_service_name());
      DT_LOG_DEBUG(get_logging_priority(), "Com service is initialized.");
      if (datatools::logger::is_trace(get_logging_priority())) {
        com.tree_dump(std::cerr, "Com service:", "[trace] ");
      }
      _pimpl_->com = &com;
      _services_.sync();
      
      // Negotiation service:
      DT_LOG_DEBUG(get_logging_priority(), "Negotiation service...");
      datatools::properties nego_config;
      if (_mconfig_.has_section(nego_service_name())) {
        nego_config = _mconfig_.get_section(nego_service_name());
      }
      {
        vire::cmsclient::negotiation_service & nego_handle =
          dynamic_cast<vire::cmsclient::negotiation_service&>(_services_.load_no_init(nego_service_name(),
                                                                                      "vire::cmsclient::negotiation_service"));
        // XXX
        nego_handle.set_logging_priority(datatools::logger::PRIO_DEBUG);
        nego_handle.set_client(*this);
        nego_handle.set_gate_user_login(get_setup_infos().get_gate_client_user_login());
        nego_handle.set_gate_user_password(get_setup_infos().get_gate_client_user_password());
      }
      _services_.configure_no_init(nego_service_name(), nego_config);
      DT_LOG_DEBUG(get_logging_priority(), "Negotiation service is configured.");
      vire::cmsclient::negotiation_service & nego =
        _services_.grab<vire::cmsclient::negotiation_service>(nego_service_name());
      DT_LOG_DEBUG(get_logging_priority(), "Negotiation service is initialized.");
      if (datatools::logger::is_trace(get_logging_priority())) {
        nego.tree_dump(std::cerr, "Negotiation service:", "[trace] ");
      }
      _pimpl_->nego = &nego;
      _services_.sync();

      // // Session manager:
      // DT_LOG_DEBUG(get_logging_priority(), "Session manager...");
      // datatools::properties session_config;
      // if (_mconfig_.has_section(session_service_name())) {
      //   session_config = _mconfig_.get_section(session_service_name());
      // }
      // vire::cmsclient::session_manager & session =
      //   dynamic_cast<vire::cmsclient::session_manager&>(_services_.load_no_init(session_service_name(),
      //                                                                           "vire::cmsclient::session_manager"));
      // session.set_logging(get_logging_priority());
      // session.set_client(*this);
      // session.initialize(session_config,
      //                    const_cast<datatools::service_dict_type&>(_services_.get_bus_of_services()));
      // {
      //   const datatools::service_dict_type & services_dict = _services_.get_bus_of_services();
      //   DT_LOG_DEBUG(get_logging_priority(), "Session dict size = " << services_dict.size());
      //   for (const auto & p : services_dict) {
      //     DT_LOG_DEBUG(get_logging_priority(), "Key='" << p.first << "'");
      //   }
      // }
      // _pimpl_->session = &session;
      // _services_.sync();

      if (datatools::logger::is_trace(get_logging_priority())) {
        _services_.tree_dump(std::cerr, "Services:", "[trace] ");
      }

      {
        const datatools::service_dict_type & services_dict = _services_.get_bus_of_services();
        DT_LOG_DEBUG(get_logging_priority(), "Session dict size = " << services_dict.size());
        for (const auto & p : services_dict) {
          DT_LOG_DEBUG(get_logging_priority(), "Key='" << p.first << "'");
        }
      }

      DT_LOG_DEBUG(get_logging_priority(), "session service is setup.");
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void client::_stop_system_services_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      // if (_services_.is_a<vire::cmsclient::session_manager>(session_service_name())) {
      //   _pimpl_->session = nullptr;
      //   _services_.drop(session_service_name());
      // }

      if (_services_.is_a<vire::cmsclient::negotiation_service>(nego_service_name())) {
        _pimpl_->nego = nullptr;
        _services_.drop(nego_service_name());
      }

      if (_services_.is_a<vire::com::manager>(com_service_name())) {
        _pimpl_->com = nullptr;
        _services_.drop(com_service_name());
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void client::_start_business_services_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // Devices manager:
      datatools::properties devices_config;
      if (_mconfig_.has_section("devices")) {
        devices_config = _mconfig_.get_section("devices");
      }
      _services_.load(devices_service_name(),
                      "vire::device::manager",
                      devices_config);
      _pimpl_->devices = &_services_.get<vire::device::manager>(devices_service_name());
      _services_.sync();

      // Resources manager:
      datatools::properties resources_config;
      if (_mconfig_.has_section("resources")) {
        resources_config = _mconfig_.get_section("resources");
      }
      _services_.load(resources_service_name(),
                      "vire::resource::manager",
                      resources_config);
      _pimpl_->resources = &_services_.get<vire::resource::manager>(resources_service_name());
      _services_.sync();

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void client::_stop_business_services_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      std::vector<std::string> list_of_services;
      _services_.build_list_of_services(list_of_services);
      std::set<std::string> list_of_business_services;
      for (const auto & service_name : list_of_services) {
        if (service_name == session_service_name()) continue;
        if (service_name == com_service_name()) continue;
        if (service_name == resources_service_name()) continue;
        if (service_name == devices_service_name()) continue;
        list_of_business_services.insert(service_name);
      }

      while (list_of_business_services.size()) {
        for (const auto & service_name : list_of_business_services) {
          if (_services_.can_drop(service_name)) {
            _services_.drop(service_name);
          }
        }
      }

      if (_services_.is_a<vire::resource::manager>(resources_service_name())) {
        _pimpl_->resources = nullptr;
        _services_.drop(resources_service_name());
      }

      if (_services_.is_a<vire::device::manager>(devices_service_name())) {
        _pimpl_->devices = nullptr;
        _services_.drop(devices_service_name());
      }

      return;
      DT_LOG_TRACE_EXITING(get_logging_priority());
    }

  } // namespace cmsclient

} // namespace vire
