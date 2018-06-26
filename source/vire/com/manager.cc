//! \file vire/com/manager.cc
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/com/manager.h>

// Third party
// - Boost:
#include <boost/algorithm/string.hpp>
// - Bayeux/datatools
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/com/domain.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace com {

    // Auto-registration of this service class in
    // a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(manager, "vire::com::manager");

    // static
    const std::string & manager::default_service_name()
    {
      static std::string _name("com");
      return _name;
    }

    manager::manager(uint32_t /* flags_ */)
    {
      _initialized_ = false;
      _set_defaults_();
      // if (flags_ & XXX) {
      //   set_xxx(true);
      // }
      return;
    }

    manager::~manager()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool manager::has_actor() const
    {
      return _actor_.is_valid();
    }

    void manager::set_actor(const actor & a_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is already initialized!");
      if (!a_.is_valid()) {
        a_.tree_dump(std::cerr, "Actor: ", "[error] ");
        DT_THROW(std::logic_error, "Invalid actor!");
      }
      _actor_ = a_;
      return;
    }

    void manager::reset_actor()
    {
      _actor_.reset();
      return;
    }

    const actor & manager::get_actor() const
    {
      return _actor_;
    }

    bool manager::has_transport_type_id() const
    {
      return _transport_type_id_.is_valid();
    }

    void manager::set_transport_type_id(const vire::utility::model_identifier & id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _transport_type_id_ = id_;
      return;
    }

    const vire::utility::model_identifier & manager::get_transport_type_id() const
    {
      return _transport_type_id_;
    }

    bool manager::has_encoding_type_id() const
    {
      return _encoding_type_id_.is_valid();
    }

    void manager::set_encoding_type_id(const vire::utility::model_identifier & id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _encoding_type_id_ = id_;
      return;
    }

    const vire::utility::model_identifier & manager::get_encoding_type_id() const
    {
      return _encoding_type_id_;
    }

    bool manager::has_subcontractor(const std::string & name_) const
    {
      return _subcontractors_.count(name_);
    }

    void manager::add_subcontractor(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _subcontractors_.insert(name_);
      return;
    }

    bool manager::has_resource_service_name() const
    {
      return !_resource_service_name_.empty();
    }
    
    void manager::set_resource_service_name(const std::string & resource_service_name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _resource_service_name_ = resource_service_name_;
      return;
    }
                        
    const std::string & manager::get_resource_service_name() const
    {
      return _resource_service_name_;
    }

    bool manager::has_resources() const
    {
      return _resources_ != nullptr;
    }

    void manager::set_resources(const vire::resource::manager & resources_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _resources_ = &resources_;
      return;
    }

    void manager::reset_resources()
    {
      _resources_ = nullptr;
      return;
    }

    const vire::resource::manager & manager::get_resources() const
    {
      DT_THROW_IF(!has_resources(), std::logic_error,
                  "No resources manager is available!");
      return *_resources_;
    }

    const domain_builder & manager::get_domain_maker() const
    {
      return _domain_maker_;
    }

    bool manager::has_domains() const
    {
      return _domains_.size();
    }

    bool manager::has_domain(const std::string & domain_name_) const
    {
      return _domains_.find(domain_name_) != _domains_.end();
    }

    const domain & manager::get_domain(const std::string & domain_name_) const
    {
      domain_dict_type::const_iterator found = _domains_.find(domain_name_);
      DT_THROW_IF(found == _domains_.end(),
                  std::logic_error,
                  "No domain with identifier '" << domain_name_ << "'!");
      return *found->second;
    }

    domain & manager::grab_domain(const std::string & domain_name_)
    {
      domain_dict_type::iterator found = _domains_.find(domain_name_);
      DT_THROW_IF(found == _domains_.end(),
                  std::logic_error,
                  "No domain with identifier '" << domain_name_ << "'!");
      return *found->second;
    }

    domain &
    manager::create_domain(const std::string & domain_name_,
                           const domain::category_type & domain_category_,
                           const vire::utility::model_identifier & domain_protocol_id_,
                           const vire::utility::model_identifier & domain_encoding_id_)
    {
      DT_THROW_IF(has_domain(domain_name_),
                  std::logic_error,
                  "Manager already has a domain with ID '" << domain_name_ << "'!");
      std::shared_ptr<domain> sp;
      DT_THROW_IF(!domain_protocol_id_.is_valid(), std::logic_error, "Invalid protocol ID !");
      DT_THROW_IF(!domain_encoding_id_.is_valid(), std::logic_error, "Invalid encoding ID !");
      sp.reset(new domain(domain_name_, domain_category_, domain_protocol_id_, domain_encoding_id_));
      if (sp.get() != nullptr) {
        _domains_[domain_name_] = sp;
      }
      return *_domains_.find(domain_name_)->second;
    }

    domain &
    manager::create_domain(const std::string & domain_name_,
                           const std::string & domain_category_repr_,
                           const std::string & domain_protocol_id_repr_,
                           const std::string & domain_encoding_id_repr_)
    {
      domain::category_type dom_cat = domain::category_from_label(domain_category_repr_);
      DT_THROW_IF(dom_cat == domain::CATEGORY_INVALID,
                  std::logic_error,
                  "Invalid domain category '" << domain_category_repr_ << "' for domain ID '" << domain_name_ << "'!");
      vire::utility::model_identifier protocol_id;
      DT_THROW_IF(!protocol_id.from_string(domain_protocol_id_repr_),
                  std::logic_error,
                  "Invalid protocol ID '" << domain_protocol_id_repr_ << "'!");
      vire::utility::model_identifier encoding_id;
      DT_THROW_IF(!encoding_id.from_string(domain_encoding_id_repr_),
                  std::logic_error,
                  "Invalid encoding ID '" << domain_encoding_id_repr_ << "'!");
      return create_domain(domain_name_, dom_cat, protocol_id, encoding_id);
    }

    void manager::remove_domain(const std::string & domain_name_)
    {
      domain_dict_type::iterator found = _domains_.find(domain_name_);
      DT_THROW_IF(found == _domains_.end(),
                  std::logic_error,
                  "No domain with identifier '" << domain_name_ << "'!");
      _domains_.erase(found);
      return;
    }

    const plug_factory & manager::get_plug_factory() const
    {
      return *_factory_.get();
    }

    void manager::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Actor : ";
      if (has_actor()) {
        out_ << "'" << get_actor().get_name() << "' "
             << "(category='" << actor::category_label(get_actor().get_category()) << "')";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Resources service : ";
      if (has_resources()) {
        out_ << "'" << _resources_->get_name() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Domains : ";
      if (_domains_.size()) {
        out_ << std::endl;
        for (domain_dict_type::const_iterator i = _domains_.begin();
             i != _domains_.end();
             i++) {
          out_ << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
          domain_dict_type::const_iterator j = i;
          if (++j == _domains_.end()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "id='" << i->first << "' (category='" << domain::label_from_category(i->second.get()->get_category()) << "')" << std::endl;
        }
      } else {
        out_ << "<none>" << std::endl;
      }
      return;
    }

    bool manager::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int manager::initialize(const datatools::properties & config_,
                            datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is already initialized!");

      this->::datatools::base_service::common_initialize(config_);

      if (!has_resources()) {
        if (!has_resource_service_name()) {
          std::string resource_service_name;
          if (config_.has_key("resource_service_name")) {
            resource_service_name = config_.fetch_string("resource_service_name");
          } else {
            resource_service_name = vire::resource::manager::default_service_name();
          }
          set_resource_service_name(resource_service_name);
        }
      }
 
      if (! has_actor()) {
        std::string actor_setup_name;
        std::string actor_id;
        std::string actor_category_repr;
        if (config_.has_key("actor.setup_name")) {
          actor_setup_name = config_.fetch_string("actor.setup_name");
        }
        if (config_.has_key("actor.id")) {
          actor_id = config_.fetch_string("actor.id");
        }
        if (config_.has_key("actor.category")) {
          actor_category_repr = config_.fetch_string("actor.category");
        }
        actor::category_type cat = actor::category(actor_category_repr);
        actor a;
        a.set_name(actor::build_name(cat, actor_setup_name, actor_id));
        a.set_category(cat);
        set_actor(a);
      }

      _at_init_();

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    int manager::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Communication manager is not initialized!");
      _initialized_ = false;
      _at_reset_();
      _set_defaults_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void manager::_set_defaults_()
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      return;
    }

    void manager::_at_init_()
    {
      // DT_THROW_IF(!has_resources(), std::logic_error, "Missing 'resources' service!");

      if (_actor_.is_server()) {

      } else if (_actor_.is_client()) {

      } else if (_actor_.is_subcontractor()) {

      }

      _factory_.reset(new plug_factory(*this));

      return;
    }

    void manager::_at_reset_()
    {
      _factory_.reset();
      _domains_.clear();
      _resources_ = nullptr;
      _actor_.reset();
      return;
    }

  } // namespace com

} // namespace vire
