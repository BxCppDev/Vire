// /vire/cmsclient/session_manager.cc
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
#include <vire/cmsclient/session_manager.h>

// Third party:
// - Boost:

// Third party:
// - Bayeux:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/service_tools.h>

// This project:
#include <vire/com/manager.h>
#include <vire/com/actor.h>
#include <vire/com/domain.h>
#include <vire/com/domain_builder.h>
#include <vire/cmsclient/client.h>
#include <vire/cmsclient/negotiation.h>

namespace vire {

  namespace cmsclient {

    /* Auto-registration of this service class in a central service Db */
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(session_manager,
                                             "vire::cmsclient::session_manager")


    std::string session_manager::state_to_label(const fsm_state state_)
    {
      switch (state_) {
      case FSM_STATE_IDLE : return "initializing";
      case FSM_STATE_NEGOTIATING : return "negotiating";
      case FSM_STATE_CONNECTED : return "connected";
      case FSM_STATE_STOPPED : return "stopped";
      case FSM_STATE_ERROR : return "error";
      default: return "";
      }
    }

    session_manager::session_manager()
    {
      return;
    }

    session_manager::~session_manager()
    {
      return;
    }

    datatools::logger::priority session_manager::get_logging() const
    {
      return _logging_;
    }

    void session_manager::set_logging(const datatools::logger::priority l_)
    {
      _logging_ = l_;
      return;
    }

    bool session_manager::is_initialized() const
    {
      return _initialized_;
    }

    int session_manager::initialize(const datatools::properties & config_,
                                    datatools::service_dict_type & services_)
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Session manager is already initialized!");

      DT_LOG_TRACE(_logging_, "Number of services : " << services_.size());

      for (const auto & p : services_) {
        std::cerr << "[devel] key='" << p.first << "'" << std::endl;
      }


      if (!has_com()) {
        std::string com_name = client::com_service_name();
        vire::com::manager & com
          = datatools::grab<vire::com::manager>(services_, com_name);
        set_com(com);
      }

      if (!has_resources()) {
        std::string resources_name = client::resources_service_name();
        vire::resource::manager & resources
          = datatools::grab<vire::resource::manager>(services_, resources_name);
        set_resources(resources);
      }

      _initialized_ = true;
      _state_ = FSM_STATE_IDLE;
      DT_LOG_TRACE_EXITING(_logging_);
      return 0;
    }

    int session_manager::reset()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Session manager is not initialized!");
      _state_ = FSM_STATE_IDLE;
      _initialized_ = false;
      DT_LOG_TRACE_EXITING(_logging_);
      return 0;
    }

    bool session_manager::has_client() const
    {
      return _client_ != nullptr;
    }

    void session_manager::set_client(client & client_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Session manager is already initialized!");
      _client_ = &client_;
      return;
    }

    bool session_manager::has_com() const
    {
      return _com_ != nullptr;
    }

    void session_manager::set_com(vire::com::manager & com_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Session manager is already initialized!");
      _com_ = &com_;
      return;
    }

    const vire::com::manager & session_manager::get_com() const
    {
      DT_THROW_IF(_com_ == nullptr, std::logic_error,
                  "Session manager has no com manager handle!");
      return *_com_;
    }

    void session_manager::reset_com()
    {
      _com_ = nullptr;
      return;
    }

    bool session_manager::has_devices() const
    {
      return _devices_ != nullptr;
    }

    void session_manager::set_devices(const vire::device::manager & devices_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Session manager is already initialized!");
      _devices_ = &devices_;
      return;
    }

    const vire::device::manager & session_manager::get_devices() const
    {
      DT_THROW_IF(_devices_ == nullptr, std::logic_error,
                  "Session manager has no device manager handle!");
      return *_devices_;
    }

    void session_manager::reset_devices()
    {
      _devices_ = nullptr;
      return;
    }

    bool session_manager::has_resources() const
    {
      return _resources_ == nullptr;
    }

    const vire::resource::manager & session_manager::get_resources() const
    {
      DT_THROW_IF(_resources_ == nullptr, std::logic_error,
                  "Session manager has no resource manager handle!");
      return *_resources_;
    }

    void session_manager::set_resources(const vire::resource::manager & resources_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Session manager is already initialized!");
      _resources_ = &resources_;
      return;
    }

    void session_manager::reset_resources()
    {
      _resources_ = nullptr;
      return;
    }

    bool session_manager::is_idle() const
    {
      return _state_ == FSM_STATE_IDLE;
    }

    bool session_manager::is_negotiating() const
    {
      return _state_ == FSM_STATE_NEGOTIATING;
    }

    bool session_manager::is_connected() const
    {
      return _state_ == FSM_STATE_CONNECTED;
    }

    bool session_manager::is_stopped() const
    {
      return _state_ == FSM_STATE_STOPPED;
    }

    bool session_manager::is_error() const
    {
      return _state_ == FSM_STATE_STOPPED;
    }

    bool session_manager::is_stop_run_requested() const
    {
      return _stop_run_requested_;
    }

    void session_manager::run()
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      DT_THROW_IF(!is_idle(), std::logic_error,
                  "Can only start run from 'idle' state!");
      vire::utility::model_identifier transport_protocol_id = _client_->get_setup_infos().get_transport_protocol_id();
      vire::utility::model_identifier encoding_protocol_id = _client_->get_setup_infos().get_encoding_protocol_id();
      _com_->set_default_transport_type_id(transport_protocol_id);
      _com_->set_default_encoding_type_id(encoding_protocol_id);
      _com_->set_resources(get_resources());

      while (true) {
        _configure_com_for_negotiation();
        negotiation nego(*this);
        bool success = nego.run(_nego_result_);
        if (!success) {
          continue;
        }
        _deconfigure_com();
        _configure_com_for_connexion();
        // connection conn(*this);
        // success = conn.run(nego_result);
        // if (!success) {
        //   continue;
        // }
        // gate_terminate();
        _deconfigure_com();
        if (is_stop_run_requested()) break;
      }
      return;
      DT_LOG_TRACE_EXITING(_logging_);
    }

    void session_manager::_configure_com_for_negotiation()
    {
      ::vire::com::manager & com = *_com_;
      std::string domain_name_prefix = _client_->get_setup_infos().get_domain_name_prefix();
      std::string gate_domain_name
        = ::vire::com::domain_builder::build_cms_clients_gate_name(domain_name_prefix);
      DT_THROW_IF(com.has_domain(gate_domain_name),
                  std::logic_error,
                  "Already have a gate domain '" << gate_domain_name << "'!");
      // Create the gate domain:
      ::vire::com::domain & gatedom = com.create_domain(gate_domain_name,
                                                     ::vire::com::DOMAIN_CATEGORY_GATE,
                                                     _client_->get_setup_infos().get_transport_protocol_id(),
                                                     _client_->get_setup_infos().get_encoding_protocol_id()
                                                     );

      // Populate gate domain:
      ::vire::com::domain_builder dommaker;
      dommaker.build_clients_gate_domain(gatedom);

      return;
    }

    void session_manager::_deconfigure_com()
    {

      ::vire::com::manager & com = *_com_;
      std::string domain_name_prefix = _client_->get_setup_infos().get_domain_name_prefix();
      std::string gate_domain_name
        = ::vire::com::domain_builder::build_cms_clients_gate_name(domain_name_prefix);
      if (com.has_domain(gate_domain_name)) {
        com.remove_domain(gate_domain_name);
      }

      std::string control_domain_name
        = ::vire::com::domain_builder::build_cms_control_name(domain_name_prefix);
      if (com.has_domain(control_domain_name)) {
        com.remove_domain(control_domain_name);
      }

      std::string monitoring_domain_name
        = ::vire::com::domain_builder::build_cms_monitoring_name(domain_name_prefix);
      if (com.has_domain(monitoring_domain_name)) {
        com.remove_domain(monitoring_domain_name);
      }

      if (_nego_result_.has_id()) {
        std::string client_system_domain_name
          = ::vire::com::domain_builder::build_cms_client_system_name(domain_name_prefix,
                                                                      _nego_result_.get_id());
        if (com.has_domain(client_system_domain_name)) {
          com.remove_domain(client_system_domain_name);
        }
      }

      return;
    }

    void session_manager::_configure_com_for_connexion()
    {
      vire::com::manager & com = *_com_;
      return;
    }

    void session_manager::_loop_()
    {

      return;
    }

    void session_manager::tree_dump(std::ostream& out_,
                                    const std::string& title_,
                                    const std::string& indent_,
                                    bool inherit_) const
    {
      this->base_service::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Com : "
           << std::boolalpha << has_com() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Devices : "
           << std::boolalpha << has_devices() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Resources : "
           << std::boolalpha << has_resources() << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "State : "
           << std::boolalpha << session_manager::state_to_label(_state_)
           << std::endl;

      return;
    }

  } // namespace cmsclient

} // namespace vire
