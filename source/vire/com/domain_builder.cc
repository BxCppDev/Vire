//! \file vire/com/domain_builder.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/com/domain_builder.h>

// Third Party:
// - Boost:
#include <boost/algorithm/string.hpp>
// - Bayeux/datatools:
#include <datatools/utils.h>
#include <datatools/exception.h>

namespace vire {

  namespace com {

    // static
    std::string domain_builder::build_cms_topic_name(const std::string & setup_id_, const std::string & topic_)
    {
      std::ostringstream oss;
      oss << '/' << setup_id_ << '/' << "cms" << '/' << "vire" << '/' << topic_;
      return oss.str();
    }

    // static
    std::string domain_builder::build_cms_control_name(const std::string & setup_id_)
    {
      return build_cms_topic_name(setup_id_, "control");
    }

    // static
    std::string domain_builder::build_cms_monitoring_name(const std::string & setup_id_)
    {
      return build_cms_topic_name(setup_id_, "monitoring");
    }

    // static
    std::string domain_builder::build_cms_subcontractor_system_name(const std::string & setup_id_,
                                                                    const std::string & subcontractor_id_)
    {
      std::ostringstream oss;
      oss << '/' << setup_id_ << '/' << "cms" << '/' << "vire" << '/' << "subcontractors" << '/' << "system" << '/'  << subcontractor_id_;
      return oss.str();
    }

    // static
    std::string domain_builder::build_cms_clients_gate_name(const std::string & setup_id_)
    {
      std::ostringstream oss;
      oss << '/' << setup_id_ << '/' << "cms" << '/' << "vire" << '/' << "clients" << '/' << "gate";
      return oss.str();
    }

    // static
    std::string domain_builder::build_cms_client_system_name(const std::string & setup_id_,
                                                             const std::string & client_id_)
    {
      std::ostringstream oss;
      oss << '/' << setup_id_ << '/' << "cms" << '/' << "vire" << '/' << "clients" << '/' << "system" << '/' << client_id_;
      return oss.str();
    }

    void domain_builder::_set_defaults_()
    {
      _setup_name_ = "";
      _default_transport_type_id_.from_string("rabbitmq");
      _default_encoding_type_id_.from_string("protobuf-3");
      return;
    }

    domain_builder::domain_builder()
    {
      _set_defaults_();
      return;
    }

    domain_builder::domain_builder(const std::string & setup_name_)
    {
      _set_defaults_();
      set_setup_name(setup_name_);
      return;
    }

    domain_builder::~domain_builder()
    {
      return;
    }

    bool domain_builder::is_valid() const
    {
      if (!has_setup_name()) return false;
      return true;
    }

    bool domain_builder::has_setup_name() const
    {
      return !_setup_name_.empty();
    }

    void domain_builder::set_setup_name(const std::string & setup_name_)
    {
      typedef std::vector<std::string > split_vector_type;
      split_vector_type splitted;
      boost::split(splitted, setup_name_, boost::is_any_of("/"));
      DT_THROW_IF(splitted.size() < 1,
                  std::logic_error,
                  "Invalid setup name '" << setup_name_ << "'!");
      uint32_t nv_flags = ::datatools::NV_NO_HYPHEN
        | ::datatools::NV_NO_DOT
        | ::datatools::NV_NO_COLON;
      for (std::size_t i = 0; i < splitted.size(); i++) {
        DT_THROW_IF(!datatools::name_validation(splitted[i], nv_flags),
                    std::logic_error,
                    "Invalid setup name '" << setup_name_ << "'!");
      }
      _setup_name_ = setup_name_;
      return;
    }

    const std::string & domain_builder::get_setup_name() const
    {
      return _setup_name_;
    }

    void domain_builder::build_clients_gate_domain(domain & dom_)
    {
      dom_.reset();
      dom_.set_name(domain_builder::build_cms_clients_gate_name(_setup_name_));
      dom_.set_category(vire::com::domain::CATEGORY_GENERAL);
      dom_.set_transport_type_id(_default_transport_type_id_);
      dom_.set_encoding_type_id(_default_encoding_type_id_);
      mailbox::permissions_type gate_perms = mailbox::usage_permission_from_string("--s---p--");
      dom_.add_mailbox("Gate",
                       mailbox::MODE_SERVICE,
                       mailbox::PRIVACY_PUBLIC,
                       "gate.service",
                       gate_perms,
                       true);
      return;
    }

    void domain_builder::build_control_domain(domain & dom_)
    {
      dom_.reset();
      dom_.set_name(domain_builder::build_cms_control_name(_setup_name_));
      dom_.set_category(vire::com::domain::CATEGORY_CONTROL);
      dom_.set_transport_type_id(_default_transport_type_id_);
      dom_.set_encoding_type_id(_default_encoding_type_id_);
      mailbox::permissions_type rr_perms = mailbox::usage_permission_from_string("p-------s");
      dom_.add_mailbox("ResourceRequest",
                       mailbox::MODE_SERVICE,
                       mailbox::PRIVACY_PUBLIC,
                       "resource_request.service",
                       rr_perms,
                       true);
      return;
    }

    void domain_builder::build_monitoring_domain(domain & dom_)
    {
      dom_.reset();
      dom_.set_name(domain_builder::build_cms_monitoring_name(_setup_name_));
      dom_.set_category(vire::com::domain::CATEGORY_MONITORING);
      dom_.set_transport_type_id(_default_transport_type_id_);
      dom_.set_encoding_type_id(_default_encoding_type_id_);
      mailbox::permissions_type rr_perms = mailbox::usage_permission_from_string("p-s--sp--");
      mailbox::permissions_type event_perms = mailbox::usage_permission_from_string("p-sp----s");
      dom_.add_mailbox("ResourceRequest",
                       mailbox::MODE_SERVICE,
                       mailbox::PRIVACY_PUBLIC,
                       "resource_request.service",
                       rr_perms,
                       true);
      dom_.add_mailbox("Alarm",
                       vire::com::mailbox::MODE_EVENT,
                       vire::com::mailbox::PRIVACY_PUBLIC,
                       "alarm.event",
                       event_perms,
                       true);
      dom_.add_mailbox("Log",
                       vire::com::mailbox::MODE_EVENT,
                       vire::com::mailbox::PRIVACY_PUBLIC,
                       "log.event",
                       event_perms,
                       true);
      dom_.add_mailbox("PubSub",
                       vire::com::mailbox::MODE_EVENT,
                       vire::com::mailbox::PRIVACY_PUBLIC,
                       "pubsub.event",
                       event_perms,
                       true);
      return;
    }

    void domain_builder::build_client_system_domain(domain & dom_, const std::string & client_id_)
    {
      dom_.reset();
      dom_.set_name(domain_builder::build_cms_client_system_name(_setup_name_, client_id_));
      dom_.set_category(vire::com::domain::CATEGORY_SYSTEM);
      dom_.set_transport_type_id(_default_transport_type_id_);
      dom_.set_encoding_type_id(_default_encoding_type_id_);
      mailbox::permissions_type s2c_perms = mailbox::usage_permission_from_string("p------k-");
      mailbox::permissions_type r2s_perms = mailbox::usage_permission_from_string("-k----p--");
      dom_.add_mailbox("SignalToClient",
                       vire::com::mailbox::MODE_EVENT,
                       vire::com::mailbox::PRIVACY_PRIVATE,
                       "client.event",
                       s2c_perms,
                       true);
      dom_.add_mailbox("RequestToServer",
                       vire::com::mailbox::MODE_SERVICE,
                       vire::com::mailbox::PRIVACY_PRIVATE,
                       "vireserver.service",
                       r2s_perms,
                       true);
      return;
    }

    void domain_builder::build_subcontractor_system_domain(domain & dom_, const std::string & subcontractor_id_)
    {
      dom_.reset();
      dom_.set_name(domain_builder::build_cms_subcontractor_system_name(_setup_name_, subcontractor_id_));
      dom_.set_category(vire::com::domain::CATEGORY_SYSTEM);
      dom_.set_transport_type_id(_default_transport_type_id_);
      dom_.set_encoding_type_id(_default_encoding_type_id_);
      mailbox::permissions_type s2b_perms = mailbox::usage_permission_from_string("p---k----");
      mailbox::permissions_type s2s_perms = mailbox::usage_permission_from_string("-k-p-----");
      mailbox::permissions_type r2s_perms = mailbox::usage_permission_from_string("-k-p-----");
      mailbox::permissions_type r2b_perms = mailbox::usage_permission_from_string("p---k----");
      dom_.add_mailbox("SignalToSubcontractor",
                       vire::com::mailbox::MODE_EVENT,
                       vire::com::mailbox::PRIVACY_PRIVATE,
                       "subcontractor.event",
                       s2b_perms,
                       true);
      dom_.add_mailbox("SignalToServer",
                       vire::com::mailbox::MODE_EVENT,
                       vire::com::mailbox::PRIVACY_PRIVATE,
                       "vireserver.event",
                       s2s_perms,
                       true);
      dom_.add_mailbox("RequestToServer",
                       vire::com::mailbox::MODE_SERVICE,
                       vire::com::mailbox::PRIVACY_PRIVATE,
                       "vireserver.service",
                       r2s_perms,
                       true);
      dom_.add_mailbox("RequestToSubcontractor",
                       vire::com::mailbox::MODE_SERVICE,
                       vire::com::mailbox::PRIVACY_PRIVATE,
                       "subcontractor.service",
                       r2b_perms,
                       true);
      return;
    }

  } // namespace com

} // namespace vire
