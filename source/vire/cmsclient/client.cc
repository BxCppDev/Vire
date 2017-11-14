// /vire/cmsclient/client.cc
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
#include <vire/cmsclient/client.h>

// Third party:
// - Bayeux:
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/com/manager.h>
#include <vire/cmsclient/session_manager.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace cmsclient {

    // static
    std::string client::com_service_name()
    {
      static const std::string _n("com");
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
      return;
    }

    client::~client()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    datatools::logger::priority client::get_logging() const
    {
      return _logging_;
    }

    void client::set_logging(const datatools::logger::priority l_)
    {
      _logging_ = l_;
      return;
    }

    bool client::has_setup_id() const
    {
      return _setup_id_.is_valid();
    }

    const ::vire::utility::instance_identifier & client::get_setup_id() const
    {
      return _setup_id_;
    }

    void client::set_setup_id(const ::vire::utility::instance_identifier & setup_id_)
    {
      _setup_id_ = setup_id_;
      return;
    }

    bool client::has_server_infos() const
    {
      return _server_infos_.is_valid();
    }

    void client::set_server_infos(const server_infos & si_)
    {
      _server_infos_ = si_;
      return;
    }

    const server_infos & client::get_server_infos() const
    {
      return _server_infos_;
    }

    server_infos & client::grab_server_infos()
    {
      return _server_infos_;
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
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(is_initialized(), std::logic_error, "Client is already initialized!");

      _mconfig_ = mconfig_;

      _at_init_();

      _start_system_services();

      _start_business_services();

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void client::reset()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(!is_initialized(), std::logic_error, "Client is not initialized!");
      _initialized_ = false;

      _stop_business_services();

      _stop_system_services();

      _at_reset_();

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void client::tree_dump(std::ostream & out_,
                           const std::string & title_,
                           const std::string & indent_,
                           bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Configuration : " << std::endl;
      {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
        _mconfig_.tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Setup ID : '" << _setup_id_.to_string() << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Server infos : " << std::endl;
      {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
        _server_infos_.tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Services : " << std::endl;
      {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::skip_tag;
        _services_.tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Initialized : "
           << std::boolalpha << _initialized_ << std::endl;

      return;
    }

    void client::_init_main_(const datatools::properties & config_)
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

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void client::_reset_main_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      _setup_id_.reset();
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void client::_at_init_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      _services_.set_name("CMSClientServices");
      _services_.set_description("CMS client service manager");
      _services_.set_allow_dynamic_services(true);
      _services_.set_force_initialization_at_load(true);
      _services_.initialize();

      // Fetch general client parameters:
      datatools::properties main_config;
      if (_mconfig_.has_section("main")) {
        main_config = _mconfig_.get_section("main");
        _init_main_(main_config);
      }

      if (!has_server_infos()) {
        // Fetch server informations:
        datatools::properties cmsserver_config;
        if (_mconfig_.has_section("cmsserver")) {
          cmsserver_config = _mconfig_.get_section("cmsserver");
          _server_infos_.initialize(cmsserver_config);
        }
      }

      // Checks: setup name/version (client vs server)
      if (has_setup_id() && has_server_infos()) {
        DT_THROW_IF(_setup_id_.get_name() != _server_infos_.get_setup_id().get_name(),
                    std::logic_error,
                    "Client setup ID '" << _setup_id_.to_string()
                    << "' does not match the server setup ID '"
                    << _server_infos_.get_setup_id().to_string() << "'!");
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void client::_at_reset_()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      _server_infos_.reset();
      _reset_main_();
      _services_.reset();
      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void client::_start_system_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      // Com manager:
      DT_LOG_TRACE(_logging_, "Com manager...");
      datatools::properties com_config;
      if (_mconfig_.has_section("com")) {
        com_config = _mconfig_.get_section("com");
      }
      _services_.load(com_service_name(),
                      "vire::com::manager",
                      com_config);
      if (datatools::logger::is_trace(_logging_)) {
        _services_.tree_dump(std::cerr, "Services:", "[trace] ");
      }
      DT_LOG_TRACE(_logging_, "com service is setup.");
      vire::com::manager & com = _services_.grab<vire::com::manager>(com_service_name());
      vire::com::actor client_actor("client", vire::com::actor::CATEGORY_CLIENT);
      if (datatools::logger::is_trace(_logging_)) {
        com.tree_dump(std::cerr, "Com service:", "[trace] ");
      }
      _services_.sync();

      {
        const datatools::service_dict_type & services_dict = _services_.get_bus_of_services();
        DT_LOG_TRACE(_logging_, "Session dict size = " << services_dict.size());
        for (const auto & p : services_dict) {
          DT_LOG_TRACE(_logging_, "Key='" << p.first << "'");
        }
      }

      // Session manager:
      DT_LOG_TRACE(_logging_, "Session manager...");
      datatools::properties session_config;
      if (_mconfig_.has_section("session")) {
        session_config = _mconfig_.get_section("session");
      }
      vire::cmsclient::session_manager & session =
        dynamic_cast<vire::cmsclient::session_manager&>(_services_.load_no_init(session_service_name(),
                                                                                "vire::cmsclient::session_manager"));
      session.set_logging(this->get_logging());
      session.set_client(*this);
      session.initialize(session_config,
                         const_cast<datatools::service_dict_type&>(_services_.get_bus_of_services()));

      {
        const datatools::service_dict_type & services_dict = _services_.get_bus_of_services();
        DT_LOG_TRACE(_logging_, "Session dict size = " << services_dict.size());
        for (const auto & p : services_dict) {
          DT_LOG_TRACE(_logging_, "Key='" << p.first << "'");
        }
      }
      _services_.sync();

      DT_LOG_TRACE(_logging_, "session service is setup.");

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void client::_stop_system_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);

      if (_services_.is_a<vire::cmsclient::session_manager>(session_service_name())) {
        _services_.drop(session_service_name());
      }

      if (_services_.is_a<vire::com::manager>(com_service_name())) {
        _services_.drop(com_service_name());
      }

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void client::_start_business_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      // Devices manager:
      datatools::properties devices_config;
      if (_mconfig_.has_section("devices")) {
        devices_config = _mconfig_.get_section("devices");
      }
      _services_.load(devices_service_name(),
                      "vire::device::manager",
                      devices_config);
      _services_.sync();

      // Resources manager:
      datatools::properties resources_config;
      if (_mconfig_.has_section("resources")) {
        resources_config = _mconfig_.get_section("resources");
      }
      _services_.load(resources_service_name(),
                      "vire::resource::manager",
                      resources_config);
      _services_.sync();

      DT_LOG_TRACE_EXITING(_logging_);
      return;
    }

    void client::_stop_business_services()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
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
        _services_.drop(resources_service_name());
      }

      if (_services_.is_a<vire::device::manager>(devices_service_name())) {
        _services_.drop(devices_service_name());
      }

      return;
      DT_LOG_TRACE_EXITING(_logging_);
    }

  } // namespace cmsclient

} // namespace vire
