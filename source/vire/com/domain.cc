//! \file vire/com/domain.cc
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
#include <vire/com/domain.h>

// Standard Library:
#include <vector>

// Third Party:
// - Boost:
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>
// - Bayeux/datatools:
#include <datatools/utils.h>
#include <datatools/exception.h>

namespace vire {

  namespace com {

    // static
    bool domain::validate_domain_name(const std::string & candidate_)
    {
      if (candidate_.empty()) return true;
      if (candidate_ == "/") return true;
      if (candidate_[0] != '/') return false;
      std::string path = candidate_.substr(1);
      typedef std::vector<std::string > split_vector_type;
      split_vector_type splitted;
      boost::split(splitted, path, boost::is_any_of("/"));
      if (splitted.size() == 0) return false;
      for (std::size_t i = 0; i < splitted.size(); i++) {
        uint32_t nv_flags = ::datatools::NV_NO_HYPHEN
          | ::datatools::NV_NO_DOT
          | ::datatools::NV_NO_COLON;
        if (!datatools::name_validation(splitted[i], nv_flags)) {
          return false;
        }
      }
      return true;
    }

    domain::domain()
    {
      return;
    }

    domain::domain(const std::string & name_,
                   const std::string & cat_repr_,
                   const std::string & transport_id_repr_,
                   const std::string & encoding_id_repr_)
    {
      set_name(name_);
      set_category(cat_repr_);
      set_transport_type_id(transport_id_repr_);
      set_encoding_type_id(encoding_id_repr_);
      return;
    }

    domain::domain(const std::string & name_,
                   const domain_category_type cat_,
                   const vire::utility::model_identifier & transport_id_,
                   const vire::utility::model_identifier & encoding_id_)
    {
      set_name(name_);
      set_category(cat_);
      set_transport_type_id(transport_id_);
      set_encoding_type_id(encoding_id_);
      return;
    }

    domain::~domain()
    {
      return;
    }

    bool domain::is_valid() const
    {
      if (!has_name()) return false;
      if (!has_category()) return false;
      if (!has_transport_type_id()) return false;
      if (!has_encoding_type_id()) return false;
      return true;
    }

    bool domain::has_name() const
    {
      return !_name_.empty();
    }

    void domain::set_name(const std::string & name_)
    {
      DT_THROW_IF(!validate_domain_name(name_), std::logic_error,
                  "Invalid domain name '" << name_ << "'!");
      _name_ = name_;
      return;
    }

    const std::string & domain::get_name() const
    {
      return _name_;
    }

    bool domain::has_category() const
    {
      return _category_ != DOMAIN_CATEGORY_INVALID;
    }

    void domain::set_category(const domain_category_type cat_)
    {
      _category_ = cat_;
      return;
    }

    void domain::set_category(const std::string & cat_repr_)
    {
      DT_THROW_IF(!from_string(cat_repr_, _category_),
                  std::logic_error,
                  "Invalid domain category representation '" << cat_repr_ << "'!");
      return;
    }

    domain_category_type domain::get_category() const
    {
      return _category_;
    }

    bool domain::is_gate() const
    {
      return _category_ == DOMAIN_CATEGORY_GATE;
    }

    bool domain::is_subcontractor_system() const
    {
      return _category_ == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM;
    }

    bool domain::is_client_system() const
    {
      return _category_ == DOMAIN_CATEGORY_CLIENT_SYSTEM;
    }

    std::string domain::get_subcontractor_identifier() const
    {
      DT_THROW_IF(!is_subcontractor_system(),
                  std::logic_error,
                  "Domain '" << _name_ << "' is not of the '"
                  << to_string(DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) << "' category!");
      std::vector<std::string> strs;
      boost::split(strs, _name_, boost::is_any_of("/"));
      return strs.back();
    }

    std::string domain::get_client_identifier() const
    {
      DT_THROW_IF(!is_client_system(),
                  std::logic_error,
                  "Domain '" << _name_ << "' is not of the '"
                  << to_string(DOMAIN_CATEGORY_CLIENT_SYSTEM) << "' category!");
      std::vector<std::string> strs;
      boost::split(strs, _name_, boost::is_any_of("/"));
      return strs.back();
    }

    bool domain::is_control() const
    {
      return _category_ == DOMAIN_CATEGORY_CONTROL;
    }

    bool domain::is_monitoring() const
    {
      return _category_ == DOMAIN_CATEGORY_MONITORING;
    }

    bool domain::has_transport_type_id() const
    {
      return _transport_type_id_.is_valid();
    }

    void domain::set_transport_type_id(const vire::utility::model_identifier & id_)
    {
      _transport_type_id_ = id_;
      return;
    }

    void domain::set_transport_type_id(const std::string & id_repr_)
    {
      DT_THROW_IF(!_transport_type_id_.from_string(id_repr_),
                  std::logic_error,
                  "Invalid transport type ID representation '" << id_repr_ << "'!");
      return;
    }

    const vire::utility::model_identifier & domain::get_transport_type_id() const
    {
      return _transport_type_id_;
    }

    bool domain::has_encoding_type_id() const
    {
      return _encoding_type_id_.is_valid();
    }

    void domain::set_encoding_type_id(const vire::utility::model_identifier & id_)
    {
      _encoding_type_id_ = id_;
      return;
    }

    void domain::set_encoding_type_id(const std::string & id_repr_)
    {
      DT_THROW_IF(!_encoding_type_id_.from_string(id_repr_), std::logic_error, "Invalid encoding type ID representation '" << id_repr_ << "'!");
      return;
    }

    const vire::utility::model_identifier & domain::get_encoding_type_id() const
    {
      return _encoding_type_id_;
    }

    const datatools::properties &
    domain::get_transport_driver_params() const
    {
      return _transport_driver_params_;
    }

    void domain::set_transport_driver_params(const datatools::properties & params_)
    {
      _transport_driver_params_ = params_;
      return;
    }

    const datatools::properties &
    domain::get_encoding_driver_params() const
    {
      return _encoding_driver_params_;
    }

    void domain::set_encoding_driver_params(const datatools::properties & params_)
    {
      _encoding_driver_params_ = params_;
      return;
    }

    void domain::initialize(const datatools::properties & config_)
    {
      if (!has_name()) {
        if (config_.has_key("name")) {
          const std::string & name = config_.fetch_string("name");
          set_name(name);
        }
      }

      if (!has_category()) {
        if (config_.has_key("category")) {
          const std::string & category_label = config_.fetch_string("category");
          set_category(category_label);
        }
      }

      if (!has_transport_type_id()) {
        if (config_.has_key("transport_type")) {
          const std::string & transport_type_id = config_.fetch_string("transport_type");
          set_transport_type_id(transport_type_id);
        }
      }

      if (!has_encoding_type_id()) {
        if (config_.has_key("encoding_type")) {
          const std::string & encoding_type_id = config_.fetch_string("encoding_type");
          set_encoding_type_id(encoding_type_id);
        }
      }

      if (_transport_driver_params_.size() == 0) {
        config_.export_and_rename_starting_with(_transport_driver_params_, "transport.", "");
      }

      if (_encoding_driver_params_.size() == 0) {
        config_.export_and_rename_starting_with(_encoding_driver_params_, "encoding.", "");
      }

     return;
    }

    void domain::reset()
    {
      _name_.clear();
      _category_ = DOMAIN_CATEGORY_INVALID;
      _transport_type_id_.reset();
      _transport_driver_params_.clear();
      _encoding_type_id_.reset();
      _encoding_driver_params_.clear();
      _mailboxes_.clear();
      _encoding_driver_.reset();
      _transport_driver_.reset();
      return;
    }

    void domain::add_mailbox(const std::string & name_,
                             const mailbox::mode_type mode_,
                             const mailbox::privacy_type privacy_,
                             const std::string & address_,
                             const mailbox::permissions_type & perms_,
                             const bool domain_)
    {
      DT_THROW_IF(has_mailbox(name_), std::logic_error,
                  "Domain '" << _name_ << "' already host a mailbox named '" << name_ <<"'!");
      mailbox_entry mbe;
      mbe.mb.set_name(name_);
      mbe.mb.set_mode(mode_);
      mbe.mb.set_privacy(privacy_);
      mbe.mb.set_address(address_);
      mbe.mb.set_permissions(perms_);
      mbe.mb.lock(domain_);
      _mailboxes_[name_] = mbe;
      return;
    }

    void domain::add_mailbox(const std::string & name_,
                             const mailbox::mode_type mode_,
                             const mailbox::privacy_type privacy_,
                             const std::string & address_,
                             const std::string & perms_repr_,
                             const bool domain_)
    {
      mailbox::permissions_type perms
        = mailbox::usage_permission_from_string(perms_repr_);
      add_mailbox(name_, mode_, privacy_, address_, perms, domain_);
      return;
    }

    /*
    std::string domain::add_private_mailbox(const std::string & owner_id_,
                                            const mailbox::mode_type mode_,
                                            const mailbox::permissions_type perms_)
    {
      DT_THROW_IF(mode_ == mailbox::MODE_INVALID, std::logic_error, "Invalid mailbox mode!");
      std::string mb_name = generate_private_mailbox_name(owner_id_, mode_);
      std::string mb_address = ""; // grab_transport_driver().new_private_mailbox_address(mailbox::MODE_EVENT);
      DT_THROW_IF(mb_address.empty(), std::logic_error,
                  "Transport driver failed to provide an address for a new private mailbox in domain '" << get_name() << "'!");
      add_mailbox(mb_name,
                  mode_,
                  mailbox::PRIVACY_PRIVATE,
                  mb_address,
                  perms_,
                  false);
      return mb_name;
    }
    */
    
    bool domain::has_mailbox(const std::string & name_) const
    {
      return _mailboxes_.count(name_) == 1;
    }

    void domain::remove_mailbox(const std::string & name_)
    {
      DT_THROW_IF(!has_mailbox(name_), std::logic_error,
                  "Domain '" << _name_ << "' has no mailbox named '" << name_ <<"'!");
      _mailboxes_.erase(name_);
      return;
    }

    const mailbox & domain::get_mailbox(const std::string & name_) const
    {
      mailbox_dict_type::const_iterator found = _mailboxes_.find(name_);
      DT_THROW_IF(found ==  _mailboxes_.end(), std::logic_error,
                  "Domain '" << _name_ << "' has no mailbox named '" << name_ << "'!");
      return found->second.mb;
    }

    // std::string domain::generate_private_mailbox_name(const std::string & owner_id_,
    //                                                   const mailbox::mode_type m_)
    // {
    //    boost::uuids::uuid mb_uuid = _mailbox_uuid_gen_();
    //    std::ostringstream out;
    //    out << "__mb." << owner_id_ << "." << mailbox::mode_label(m_) << "." << mb_uuid;
    //    return out.str();
    // }

    const i_encoding_driver & domain::get_encoding_driver() const
    {
      domain * mutable_this = const_cast<domain*>(this);
      return mutable_this->_encoding_driver_instance_();
    }

    i_encoding_driver & domain::_encoding_driver_instance_()
    {
      if (_encoding_driver_.get() == nullptr) {
        std::string encoding_driver_type_id = _encoding_type_id_.get_name();
        i_encoding_driver::factory_register_type & sys_factory_register
          = DATATOOLS_FACTORY_GRAB_SYSTEM_REGISTER(i_encoding_driver);
        DT_THROW_IF(! sys_factory_register.has(encoding_driver_type_id), std::logic_error,
                    "No encoding type ID '" << encoding_driver_type_id << "' factory is known from the system register!");
        // Factory instantiates a new object:
        const i_encoding_driver::factory_register_type::factory_type & the_factory
          = sys_factory_register.get(encoding_driver_type_id);
        _encoding_driver_.reset(the_factory());
        _encoding_driver_->initialize(_encoding_driver_params_);
      }
      return *_encoding_driver_.get();
    }

    const i_transport_driver & domain::get_transport_driver() const
    {
      domain * mutable_this = const_cast<domain*>(this);
      return mutable_this->_transport_driver_instance_();
    }

    i_transport_driver & domain::grab_transport_driver()
    {
      return _transport_driver_instance_();
    }

    i_transport_driver & domain::_transport_driver_instance_()
    {
      if (_transport_driver_.get() == nullptr) {
        std::string transport_driver_type_id = _transport_type_id_.get_name();
        i_transport_driver::factory_register_type & sys_factory_register
          = DATATOOLS_FACTORY_GRAB_SYSTEM_REGISTER(i_transport_driver);
        DT_THROW_IF(! sys_factory_register.has(transport_driver_type_id), std::logic_error,
                    "No transport type ID '" << transport_driver_type_id << "' factory is known from the system register!");
        // Factory instantiates a new object:
        const i_transport_driver::factory_register_type::factory_type & the_factory
          = sys_factory_register.get(transport_driver_type_id);
        _transport_driver_.reset(the_factory());
        _transport_driver_->set_domain(*this);
        _transport_driver_->initialize(get_transport_driver_params());
      }
      return *_transport_driver_.get();
    }

    const std::shared_ptr<i_encoding_driver> & domain::get_encoding_driver_ptr() const
    {
      const_cast<domain*>(this)->_encoding_driver_instance_();
      return _encoding_driver_;
    }
 
    const std::shared_ptr<i_transport_driver> & domain::get_transport_driver_ptr() const
    {
      const_cast<domain*>(this)->_transport_driver_instance_();
      return _transport_driver_;
    }
    
    std::shared_ptr<i_transport_driver> & domain::grab_transport_driver_ptr()
    {
      _transport_driver_instance_();
      return _transport_driver_;
    }
  
    void domain::tree_dump(std::ostream & out_,
                           const std::string & title_,
                           const std::string & indent_,
                           bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name              : '" << _name_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Category          : '" << to_string(_category_) << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Transport type ID : '" << _transport_type_id_.to_string() << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Encoding type ID  : '" << _encoding_type_id_.to_string() << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Mailboxes         : [" << _mailboxes_.size() << "]" << std::endl;
      for (mailbox_dict_type::const_iterator i = _mailboxes_.begin();
            i != _mailboxes_.end();
           i++) {
        mailbox_dict_type::const_iterator j = i;
        out_ << indent_ << datatools::i_tree_dumpable::skip_tag;
        if (++j == _mailboxes_.end()) {
          out_ << datatools::i_tree_dumpable::last_tag;
        } else {
          out_ << datatools::i_tree_dumpable::tag;
        }
        const mailbox & mb = i->second.mb;
        out_ << "Mailbox '" << i->first << "' : "  ;
        out_ << "{mode=" << mailbox::mode_label(mb.get_mode())
             << ";privacy=" << mailbox::privacy_label(mb.get_privacy())
             << ";address=" << mb.get_address()
             << ";permissions=[" << mailbox::usage_permission_to_string(mb.get_permissions()) << ']'
             << '}';
        out_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Encoding driver   : " << _encoding_driver_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Transport driver  : " << _transport_driver_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Validity          : " << std::boolalpha << is_valid() << std::endl;

      return;
    }

  } // namespace com

} // namespace vire
