// vire/cmsclient/setup_infos.cc
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
#include <vire/cmsclient/setup_infos.h>

// Standard libraries:
#include <sstream>
#include <iomanip>

// Third party:
// - Bayeux:
#include <bayeux/datatools/version_id.h>

namespace vire {

  namespace cmsclient {

    setup_infos::setup_infos()
    {
      return;
    }

    setup_infos::~setup_infos()
    {
      return;
    }

    bool setup_infos::has_setup_id() const
    {
      return _setup_id_.is_valid();
    }

    void setup_infos::set_setup_id(const ::vire::utility::instance_identifier & setup_id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked!");
      _setup_id_ = setup_id_;
      return;
    }

    const ::vire::utility::instance_identifier & setup_infos::get_setup_id() const
    {
      return _setup_id_;
    }

    bool setup_infos::has_host() const
    {
      return !_host_.empty();
    }

    void setup_infos::set_host(const std::string & host_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked!");
      _host_ = host_;
      return;
    }

    const std::string & setup_infos::get_host() const
    {
      return _host_;
    }

    bool setup_infos::has_port() const
    {
      return _port_ != -1;
    }

    void setup_infos::set_port(const int port_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked!");
      _port_ = (port_ > 0 ? port_ : -1);
      return;
    }

    int setup_infos::get_port() const
    {
      return _port_;
    }

    bool setup_infos::has_domain_name_prefix() const
    {
      return !_domain_name_prefix_.empty();
    }

    void setup_infos::set_domain_name_prefix(const std::string & tp_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked!");
      _domain_name_prefix_ = tp_;
      return;
    }

    const std::string & setup_infos::get_domain_name_prefix() const
    {
      return _domain_name_prefix_;
    }

    bool setup_infos::has_transport_driver_type_id() const
    {
      return _transport_driver_type_id_.is_valid();
    }

    void setup_infos::set_transport_driver_type_id(const ::vire::utility::model_identifier & tp_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked!");
      _transport_driver_type_id_ = tp_;
      return;
    }

    const ::vire::utility::model_identifier & setup_infos::get_transport_driver_type_id() const
    {
      return _transport_driver_type_id_;
    }

    bool setup_infos::has_encoding_driver_type_id() const
    {
      return _encoding_driver_type_id_.is_valid();
    }

    void setup_infos::set_encoding_driver_type_id(const ::vire::utility::model_identifier & ep_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked!");
      _encoding_driver_type_id_ = ep_;
      return;
    }

    const ::vire::utility::model_identifier & setup_infos::get_encoding_driver_type_id() const
    {
      return _encoding_driver_type_id_;
    }

    bool setup_infos::has_gate_client_user_login() const
    {
      return !_gate_client_user_login_.empty();
    }

    void setup_infos::set_gate_client_user_login(const std::string & login_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked!");
      _gate_client_user_login_ = login_;
      return;
    }

    const std::string & setup_infos::get_gate_client_user_login() const
    {
      return _gate_client_user_login_;
    }

    bool setup_infos::has_gate_client_user_password() const
    {
      return !_gate_client_user_password_.empty();
    }

    void setup_infos::set_gate_client_user_password(const std::string & ep_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked!");
      _gate_client_user_password_ = ep_;
      return;
    }

    const std::string & setup_infos::get_gate_client_user_password() const
    {
      return _gate_client_user_password_;
    }

    bool setup_infos::is_valid() const
    {
      if (!has_setup_id()) return false;
      if (!has_host()) return false;
      if (!has_port()) return false;
      if (!has_domain_name_prefix()) return false;
      if (!has_transport_driver_type_id()) return false;
      if (!has_encoding_driver_type_id()) return false;
      if (!has_gate_client_user_login()) return false;
      // if (!has_gate_client_user_password()) return false;
      return true;
    }

    void setup_infos::lock()
    {
      _initialized_ = true;
      return;
    }

    void setup_infos::unlock()
    {
      _initialized_ = false;
      return;
    }

    bool setup_infos::is_initialized() const
    {
      return _initialized_;
    }

    void setup_infos::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Already initialized!");

      if (config_.has_key("setup_id")) {
        vire::utility::instance_identifier sid;
        const std::string & sid_repr = config_.fetch_string("setup_id");
        DT_THROW_IF(!sid.from_string(sid_repr), std::logic_error,
                    "Invalid setup ID '" << sid_repr<< "'!");
        set_setup_id(sid);
      }

      if (config_.has_key("host")) {
        const std::string & h = config_.fetch_string("host");
        set_host(h);
      }

      if (config_.has_key("port")) {
        int p = config_.fetch_positive_integer("port");
        set_port(p);
      }

      if (config_.has_key("domain_name_prefix")) {
        const std::string & dnp = config_.fetch_string("domain_name_prefix");
        set_domain_name_prefix(dnp);
      }

      if (config_.has_key("transport_driver_type_id")) {
        vire::utility::model_identifier tpid;
        const std::string & tpid_repr = config_.fetch_string("transport_driver_type_id");
        DT_THROW_IF(!tpid.from_string(tpid_repr), std::logic_error,
                    "Invalid transport driver type ID '" << tpid_repr<< "'!");
        set_transport_driver_type_id(tpid);
      }

      if (config_.has_key("encoding_driver_type_id")) {
        vire::utility::model_identifier tpid;
        const std::string & tpid_repr = config_.fetch_string("encoding_driver_type_id");
        DT_THROW_IF(!tpid.from_string(tpid_repr), std::logic_error,
                    "Invalid encoding driver type ID '" << tpid_repr<< "'!");
        set_encoding_driver_type_id(tpid);
      }

      if (config_.has_key("gate_client_user_login")) {
        const std::string & gl = config_.fetch_string("gate_client_user_login");
        set_gate_client_user_login(gl);
      }

      if (config_.has_key("gate_client_user_password")) {
        const std::string & gp = config_.fetch_string("gate_client_user_password");
        set_gate_client_user_password(gp);
      }

      bool lock_it = true;
      if (config_.has_key("nolock")) {
        lock_it = false;
      }
      if (lock_it) {
        _initialized_ = true;
      }
      return;
    }

    void setup_infos::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Not initialized!");
      _initialized_ = false;
      _setup_id_.reset();
      _host_.clear();
      _port_ = -1;
      _domain_name_prefix_.clear();
      _transport_driver_type_id_.reset();
      _encoding_driver_type_id_.reset();
      _gate_client_user_login_.clear();
      _gate_client_user_password_.clear();
      return;
    }

    void setup_infos::print_tree(std::ostream & out_,
                                 const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);
      std::ostringstream outs;
      if (!popts.title.empty()) {
        outs << popts.indent << popts.title << std::endl;
      }

      outs << popts.indent << datatools::i_tree_dumpable::tag
           << "Setup ID : '" << _setup_id_.to_string() << "'" << std::endl;

      outs << popts.indent << datatools::i_tree_dumpable::tag
           << "Host : '" << _host_ << "'" << std::endl;

      outs << popts.indent << datatools::i_tree_dumpable::tag
           << "Port : " << _port_ << std::endl;

      outs << popts.indent << datatools::i_tree_dumpable::tag
           << "Domain name prefix : '" << _domain_name_prefix_ << "'" <<std::endl;

      outs << popts.indent << datatools::i_tree_dumpable::tag
           << "Transport driver type ID : '" << _transport_driver_type_id_.to_string() << "'" <<std::endl;

      outs << popts.indent << datatools::i_tree_dumpable::tag
           << "Encoding driver type ID : '" << _encoding_driver_type_id_.to_string() << "'" << std::endl;

      outs << popts.indent << datatools::i_tree_dumpable::tag
           << "Gate client user login : '" << _gate_client_user_login_ << "'" << std::endl;

      outs << popts.indent << datatools::i_tree_dumpable::tag
           << "Gate client user password : ";
      if (has_gate_client_user_password()) {
        outs << "'" << std::setfill ('*')
             << std::setw(_gate_client_user_password_.size()) << "'";
      } else {
        outs << "<none>";
      }
      outs << std::endl;

      outs << popts.indent << datatools::i_tree_dumpable::tag
           << "Validity : "
           << std::boolalpha << is_valid() << std::endl;

      outs << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Initialized : "
           << std::boolalpha << _initialized_ << std::endl;
      
      out_ << outs.str();
      return;
    }

  } // namespace cmsclient

} // namespace vire
