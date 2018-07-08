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

// This project:
#include <vire/com/domain.h>

namespace vire {

  namespace com {

    // static
    bool domain_builder::validate_domain_name_prefix(const std::string & prefix_)
    {
      if (prefix_.empty()) {
        // std::cerr << "[devel] empty prefix='" << prefix_ << "'\n";
        return false;
      }
      if (prefix_[0] != '/') {
        // std::cerr << "[devel] missing leading '/' prefix='" << prefix_ << "'\n";
        return false;
      }
      if (prefix_.length() < 1) return false;
      std::string domain_name_prefix_tmp = prefix_.substr(1);
      // std::cerr << "[devel] domain_name_prefix_tmp=" << domain_name_prefix_tmp << "\n";
      typedef std::vector<std::string > split_vector_type;
      split_vector_type splitted;
      boost::split(splitted, domain_name_prefix_tmp, boost::is_any_of("/"));
      DT_THROW_IF(splitted.size() < 1,
                  std::logic_error,
                  "Invalid domain name prefix '" << prefix_ << "'!");
      uint32_t nv_flags = ::datatools::NV_NO_HYPHEN
        | ::datatools::NV_NO_DOT
        | ::datatools::NV_NO_COLON;
      for (std::size_t i = 0; i < splitted.size(); i++) {
        if (!datatools::name_validation(splitted[i], nv_flags)) {
          // std::cerr << "[devel] invalid segment : splitted[i]=" << splitted[i] << "\n";
          return false;
        }
      }
      return true;
    }

    // static
    std::string domain_builder::domain_name_prefix(const std::string & domain_name_prefix_)
    {
      std::string dnp;
      if (domain_name_prefix_.empty()) {
        DT_THROW(std::logic_error, "Invalid domain name prefix!");
      }
      if (domain_name_prefix_[0] != '/') {
        dnp.push_back('/');
      }
      dnp += domain_name_prefix_;
      return dnp;
    }

    // static
    std::string domain_builder::build_cms_topic_name(const std::string & domain_name_prefix_, const std::string & topic_)
    {
      std::ostringstream oss;
      oss << domain_name_prefix(domain_name_prefix_) << '/' << "cms" << '/' << "vire" << '/' << topic_;
      return oss.str();
    }

    // static
    std::string domain_builder::build_cms_control_name(const std::string & domain_name_prefix_)
    {
      return build_cms_topic_name(domain_name_prefix_, "control");
    }

    // static
    std::string domain_builder::build_cms_monitoring_name(const std::string & domain_name_prefix_)
    {
      return build_cms_topic_name(domain_name_prefix_, "monitoring");
    }

    // static
    std::string domain_builder::build_cms_subcontractor_system_name(const std::string & domain_name_prefix_,
                                                                    const std::string & subcontractor_id_)
    {
      std::ostringstream oss;
      oss << "subcontractors" << '/' << "system" << '/'  << subcontractor_id_;
      return build_cms_topic_name(domain_name_prefix_, oss.str());
    }

    // static
    std::string domain_builder::build_cms_clients_gate_name(const std::string & domain_name_prefix_)
    {
      std::ostringstream oss;
      oss << "clients" << '/' << "gate";
      return build_cms_topic_name(domain_name_prefix_, oss.str());
    }

    // static
    std::string domain_builder::build_cms_client_system_name(const std::string & domain_name_prefix_,
                                                             const std::string & client_id_)
    {
      std::ostringstream oss;
      oss << "clients" << '/' << "system" << '/' << client_id_;
      return build_cms_topic_name(domain_name_prefix_, oss.str());
    }

    void domain_builder::_set_defaults_()
    {
      _domain_name_prefix_.clear();
      _transport_type_id_.reset();
      _encoding_type_id_.reset();
      return;
    }

    domain_builder::domain_builder()
    {
      _set_defaults_();
      return;
    }

    domain_builder::domain_builder(const std::string & domain_name_prefix_)
    {
      _set_defaults_();
      set_domain_name_prefix(domain_name_prefix_);
      return;
    }

    domain_builder::~domain_builder()
    {
      return;
    }

    bool domain_builder::is_valid() const
    {
      if (!has_domain_name_prefix()) return false;
      if (!has_encoding_type_id()) return false;
      if (!has_transport_type_id()) return false;
      return true;
    }

    bool domain_builder::has_domain_name_prefix() const
    {
      return !_domain_name_prefix_.empty();
    }

    void domain_builder::set_domain_name_prefix(const std::string & domain_name_prefix_)
    {
      DT_THROW_IF(!validate_domain_name_prefix(domain_name_prefix_),
                  std::logic_error,
                  "Invalid domain name prefix '" << domain_name_prefix_ << "'!");
      _domain_name_prefix_ = domain_name_prefix_;
      return;
    }

    const std::string & domain_builder::get_domain_name_prefix() const
    {
      return _domain_name_prefix_;
    }

    bool domain_builder::has_encoding_type_id() const
    {
      return _encoding_type_id_.is_valid();
    }

    void domain_builder::set_encoding_type_id(const vire::utility::model_identifier & encoding_type_id_)
    {
      _encoding_type_id_ = encoding_type_id_;
      return;
    }

    const vire::utility::model_identifier & domain_builder::get_encoding_type_id() const
    {
      return _encoding_type_id_;
    }

    bool domain_builder::has_transport_type_id() const
    {
      return _transport_type_id_.is_valid();
    }

    void domain_builder::set_transport_type_id(const vire::utility::model_identifier & transport_type_id_)
    {
      _transport_type_id_ = transport_type_id_;
      return;
    }

    const vire::utility::model_identifier & domain_builder::get_transport_type_id() const
    {
      return _transport_type_id_;
    }

    void domain_builder::build_clients_gate_domain(domain & dom_) const 
    {
      if (!dom_.has_name()) {
        dom_.set_name(domain_builder::build_cms_clients_gate_name(_domain_name_prefix_));
      }
      if (!dom_.has_category()) {
        dom_.set_category(vire::com::DOMAIN_CATEGORY_GATE);
      }
      if (!dom_.has_transport_type_id()) {
        dom_.set_transport_type_id(_transport_type_id_);
      }
      if (!dom_.has_encoding_type_id()) {
        dom_.set_encoding_type_id(_encoding_type_id_);
      }

      mailbox::permissions_type gate_perms = mailbox::usage_permission_from_string("--s---p--");
      dom_.add_mailbox("Gate",
                       mailbox::MODE_SERVICE,
                       mailbox::PRIVACY_PUBLIC,
                       "gate.service",
                       gate_perms,
                       true);
      return;
    }

    void domain_builder::build_control_domain(domain & dom_) const
    {
      if (!dom_.has_name()) {
        dom_.set_name(domain_builder::build_cms_control_name(_domain_name_prefix_));
      }
      if (!dom_.has_category()) {
        dom_.set_category(vire::com::DOMAIN_CATEGORY_CONTROL);
      }
      if (!dom_.has_transport_type_id()) {
        dom_.set_transport_type_id(_transport_type_id_);
      }
      if (!dom_.has_encoding_type_id()) {
        dom_.set_encoding_type_id(_encoding_type_id_);
      }

      mailbox::permissions_type rr_perms = mailbox::usage_permission_from_string("--s---p--");
      dom_.add_mailbox("ResourceRequest",
                       mailbox::MODE_SERVICE,
                       mailbox::PRIVACY_PUBLIC,
                       "resource_request.service",
                       rr_perms,
                       true);
      return;
    }

    void domain_builder::build_monitoring_domain(domain & dom_) const
    {
      if (!dom_.has_name()) {
        dom_.set_name(domain_builder::build_cms_monitoring_name(_domain_name_prefix_));
      }
      if (!dom_.has_category()) {
        dom_.set_category(vire::com::DOMAIN_CATEGORY_MONITORING);
      }
      if (!dom_.has_transport_type_id()) {
        dom_.set_transport_type_id(_transport_type_id_);
      }
      if (!dom_.has_encoding_type_id()) {
        dom_.set_encoding_type_id(_encoding_type_id_);
      }

      mailbox::permissions_type rr_perms = mailbox::usage_permission_from_string("p-s--sp--");
      dom_.add_mailbox("ResourceRequest",
                       mailbox::MODE_SERVICE,
                       mailbox::PRIVACY_PUBLIC,
                       "resource_request.service",
                       rr_perms,
                       true);
      mailbox::permissions_type event_perms = mailbox::usage_permission_from_string("p-sp----s");
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

    void domain_builder::build_client_system_domain(domain & dom_, const std::string & client_id_) const
    {
      if (!dom_.has_name()) {
        dom_.set_name(domain_builder::build_cms_client_system_name(_domain_name_prefix_, client_id_));
      }
      if (!dom_.has_category()) {
        dom_.set_category(vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM);
      }
      if (!dom_.has_transport_type_id()) {
        dom_.set_transport_type_id(_transport_type_id_);
      }
      if (!dom_.has_encoding_type_id()) {
        dom_.set_encoding_type_id(_encoding_type_id_);
      }

      mailbox::permissions_type rts_perms = mailbox::usage_permission_from_string("--s---p--");
      dom_.add_mailbox("RequestToServer",
                       vire::com::mailbox::MODE_SERVICE,
                       vire::com::mailbox::PRIVACY_PUBLIC,
                       "vireserver.service",
                       rts_perms,
                       true);

      mailbox::permissions_type sfs_perms = mailbox::usage_permission_from_string("p-------s");
      dom_.add_mailbox("SignalFromServer",
                       vire::com::mailbox::MODE_EVENT,
                       vire::com::mailbox::PRIVACY_PUBLIC,
                       "vireserver.event",
                       sfs_perms,
                       true);
      return;
    }

    void domain_builder::build_subcontractor_system_domain(domain & dom_, const std::string & subcontractor_id_) const
    {
      if (!dom_.has_name()) {
        dom_.set_name(build_cms_subcontractor_system_name(_domain_name_prefix_, subcontractor_id_));
      }
      if (!dom_.has_category()) {
        dom_.set_category(vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM);
      }
      if (!dom_.has_transport_type_id()) {
        dom_.set_transport_type_id(_transport_type_id_);
      }
      if (!dom_.has_encoding_type_id()) {
        dom_.set_encoding_type_id(_encoding_type_id_);
      }

      mailbox::permissions_type rts_perms = mailbox::usage_permission_from_string("--sp-----");
      dom_.add_mailbox("RequestToServer",
                       vire::com::mailbox::MODE_SERVICE,
                       vire::com::mailbox::PRIVACY_PUBLIC,
                       "vireserver.service",
                       rts_perms,
                       true);

      mailbox::permissions_type sfs_perms = mailbox::usage_permission_from_string("p----s---");
      dom_.add_mailbox("SignalFromServer",
                       vire::com::mailbox::MODE_EVENT,
                       vire::com::mailbox::PRIVACY_PUBLIC,
                       "vireserver.event",
                       sfs_perms,
                       true);

      mailbox::permissions_type rtc_perms = mailbox::usage_permission_from_string("p----s---");
      dom_.add_mailbox("RequestToSubcontractor",
                       vire::com::mailbox::MODE_SERVICE,
                       vire::com::mailbox::PRIVACY_PUBLIC,
                       "subcontractor.service",
                       rtc_perms,
                       true);

      mailbox::permissions_type sfc_perms = mailbox::usage_permission_from_string("--sp-----");
      dom_.add_mailbox("SignalFromSubcontractor",
                       vire::com::mailbox::MODE_EVENT,
                       vire::com::mailbox::PRIVACY_PUBLIC,
                       "subcontractor.event",
                       sfc_perms,
                       true);
      return;
    }

  } // namespace com

} // namespace vire
