//! \file vire/com/mailbox.cc
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
#include <vire/com/mailbox.h>

namespace vire {

  namespace com {

    // static
    std::string mailbox::privacy_label(const privacy_type pt_)
    {
      switch(pt_) {
      case PRIVACY_PUBLIC: return "public";
      case PRIVACY_PRIVATE: return "private";
      default:
        break;
      }
      return "";
    }

    // static
    mailbox::privacy_type mailbox::privacy(const std::string & label_)
    {
      if (label_ == privacy_label(PRIVACY_PUBLIC)) return PRIVACY_PUBLIC;
      if (label_ == privacy_label(PRIVACY_PRIVATE)) return PRIVACY_PRIVATE;
      return PRIVACY_INVALID;
    }

    // static
    std::string mailbox::mode_label(const mode_type pt_)
    {
      switch(pt_) {
      case MODE_SERVICE: return "service";
      case MODE_EVENT: return "event";
      default:
        break;
      }
      return "";
    }

    // static
    mailbox::mode_type mailbox::mode(const std::string & label_)
    {
      if (label_ == mode_label(MODE_SERVICE)) return MODE_SERVICE;
      if (label_ == mode_label(MODE_EVENT)) return MODE_EVENT;
      return MODE_INVALID;
    }

    // static
    std::string mailbox::usage_permission_label(const usage_permission_flag pf_)
    {
      switch(pf_) {
      case USAGE_PERMISSION_PUT: return "put";
      case USAGE_PERMISSION_PRIVATE_PICKUP: return "private_pickup";
      case USAGE_PERMISSION_PUBLIC_SUBSCRIBE: return "public_subscribe";
      default:
        break;
      }
      return "";
    }

    // static
    char mailbox::usage_permission_symbol(const usage_permission_flag pf_)
    {
      switch(pf_) {
      case USAGE_PERMISSION_PUT: return 'p';
      case USAGE_PERMISSION_PRIVATE_PICKUP: return 'k';
      case USAGE_PERMISSION_PUBLIC_SUBSCRIBE: return 's';
      default:
        break;
      }
      return '-';
    }

    // static
    bool mailbox::usage_permission_from_string(const std::string & repr_, permissions_type & perm_)
    {
      // std::cerr << "[devel] usage_permission_from_string: repr='" << repr_ << "'..." << std::endl;
      perm_.reset();
      std::string repr(repr_);
      std::transform(repr.begin(), repr.end(), repr.begin(), ::tolower);
      if (repr.length() != 9) return false;
      static const char pks[3] = { 'p', 'k', 's' };
      for (int i = 0; i < 3; i++) {
        // i == 0 : S => server
        // i == 1 : B => subcontractor
        // i == 2 : C => client
        for (int j = 0; j < 3; j++) {
          // j == 0 : P => put
          // j == 1 : K => private pickup
          // j == 2 : S => public subscribe
          int sympos = (i * 3 + j);
          char sym = repr[sympos];
          // std::cerr << "[devel] usage_permission_from_string: sympos='" << sympos << "' sym='" << sym << "'" << std::endl;
          if (sym == pks[j]) {
            perm_.set(8 - sympos);
          } else if (sym != '-'){
            return false;
          }
        }
      }
      return true;
    }

    std::string mailbox::usage_permission_to_string(const permissions_type & perms_)
    {
      std::ostringstream out;
      static const char pks[3] = { 'p', 'k', 's' };
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          int sympos = (i * 3 + j);
          if (perms_.test(8 - sympos)) {
            out << pks[j];
          } else {
            out << '-';
          }
        }
      }
      return out.str();
    }

    // static
    mailbox::permissions_type mailbox::usage_permission_from_string(const std::string & repr_)
    {
      permissions_type perms;
      DT_THROW_IF(!usage_permission_from_string(repr_, perms),
                  std::logic_error,
                  "Invalid representation of mailbox permissions '" << repr_ << "'!");
      return perms;
    }

    // static
    mailbox::usage_permission_flag mailbox::usage_permission(const std::string & label_)
    {
      if (label_ == usage_permission_label(USAGE_PERMISSION_PUT)) return USAGE_PERMISSION_PUT;
      if (label_ == usage_permission_label(USAGE_PERMISSION_PRIVATE_PICKUP)) return USAGE_PERMISSION_PRIVATE_PICKUP;
      if (label_ == usage_permission_label(USAGE_PERMISSION_PUBLIC_SUBSCRIBE)) return USAGE_PERMISSION_PUBLIC_SUBSCRIBE;
      return USAGE_PERMISSION_NULL;
    }

    mailbox::mailbox()
    {
      return;
    }

    mailbox::mailbox(const std::string & name_,
                     const mode_type mode_,
                     const privacy_type privacy_,
                     const std::string & address_,
                     const permissions_type & permissions_)
    {
      set_name(name_);
      set_mode(mode_);
      set_privacy(privacy_);
      set_address(address_);
      set_permissions(permissions_);
      return;
    }

    mailbox::mailbox(const std::string & name_,
                     const mode_type mode_,
                     const privacy_type privacy_,
                     const std::string & address_,
                     const std::string & perms_repr_)
    {
      set_name(name_);
      set_mode(mode_);
      set_privacy(privacy_);
      set_address(address_);
      mailbox::permissions_type perms
        = mailbox::usage_permission_from_string(perms_repr_);
      set_permissions(perms);
      return;
    }

    mailbox::~mailbox()
    {
      return;
    }

    bool mailbox::is_valid() const
    {
      if (_name_.empty()) return false;
      if (_address_.empty()) return false;
      if (_mode_ == MODE_INVALID) return false;
      if (_privacy_ == PRIVACY_INVALID) return false;
      return true;
    }

    void mailbox::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Mailbox is locked!");

      if (!has_name()) {
        if (config_.has_key("name")) {
          set_name(config_.fetch_string("name"));
        }
      }

      if (!has_address()) {
        if (config_.has_key("address")) {
          set_address(config_.fetch_string("address"));
        }
      }

      if (config_.has_key("mode")) {
        std::string mode_label = config_.fetch_string("mode");
        mode_type the_mode = mode(mode_label);
        DT_THROW_IF(the_mode == MODE_INVALID,
                    std::logic_error,
                    "Invalid mailbox mode '" << mode_label << "'!");
        set_mode(the_mode);
      }

      if (config_.has_key("privacy")) {
        std::string privacy_label = config_.fetch_string("privacy");
        privacy_type the_privacy = privacy(privacy_label);
        DT_THROW_IF(the_privacy == PRIVACY_INVALID,
                    std::logic_error,
                    "Invalid mailbox privacy '" << privacy_label << "'!");
        set_privacy(the_privacy);
      }

      return;
    }

    void mailbox::reset()
    {
      _locked_ = false;
      _domain_ = false;
      _name_.clear();
      _address_.clear();
      _mode_ = MODE_INVALID;
      _privacy_ = PRIVACY_INVALID;
      return;
    }

    bool mailbox::has_name() const
    {
      return !_name_.empty();
    }

    void mailbox::set_name(const std::string & name_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Mailbox is locked!");
      _name_ = name_;
      return;
    }

    const std::string & mailbox::get_name() const
    {
      return _name_;
    }

    bool mailbox::has_address() const
    {
      return !_address_.empty();
    }

    void mailbox::set_address(const std::string & address_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Mailbox is locked!");
      _address_ = address_;
      return;
    }

    const std::string & mailbox::get_address() const
    {
      return _address_;
    }

    bool mailbox::has_mode() const
    {
      return _mode_ != MODE_INVALID;
    }

    void mailbox::set_mode(const mode_type & mode_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Mailbox is locked!");
      _mode_ = mode_;
      return;
    }

    mailbox::mode_type mailbox::get_mode() const
    {
      return _mode_;
    }

    bool mailbox::is_event() const
    {
      return _mode_ == MODE_EVENT;
    }

    bool mailbox::is_service() const
    {
      return _mode_ == MODE_SERVICE;
    }

    bool mailbox::has_privacy() const
    {
      return _privacy_ != PRIVACY_INVALID;
    }

    void mailbox::set_privacy(const privacy_type & privacy_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Mailbox is locked!");
      _privacy_ = privacy_;
      return;
    }

    mailbox::privacy_type mailbox::get_privacy() const
    {
      return _privacy_;
    }

    bool mailbox::is_public() const
    {
      return _privacy_ == PRIVACY_PUBLIC;
    }

    bool mailbox::is_private() const
    {
      return _privacy_ == PRIVACY_PRIVATE;
    }

    void mailbox::set_permissions(const permissions_type & permissions_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Mailbox is locked!");
      _permissions_ = permissions_;
      return;
    }

    void mailbox::set_permissions(const std::string & permissions_repr_)
    {
      permissions_type perms;
      DT_THROW_IF(!usage_permission_from_string(permissions_repr_, perms),
                  std::logic_error,
                  "Invalid representation of mailbox permissions ('" << permissions_repr_ << "')!");
      set_permissions(perms);
      return;
    }

    const mailbox::permissions_type & mailbox::get_permissions() const
    {
      return _permissions_;
    }

    bool mailbox::check_permissions(const actor & actor_,
                                    const usage_permission_flag flag_) const
    {
      std::size_t perm_flag = 3;
      switch(flag_) {
      case USAGE_PERMISSION_PUT:              perm_flag = 0; break;
      case USAGE_PERMISSION_PRIVATE_PICKUP:   perm_flag = 1; break;
      case USAGE_PERMISSION_PUBLIC_SUBSCRIBE: perm_flag = 2; break;
      default:
        perm_flag = 3;
      }
      if (perm_flag == 3) return false;
      std::size_t actor_flag = 3;
      switch(actor_.get_category()) {
      case actor::CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM: actor_flag = 0; break;
      case actor::CATEGORY_SERVER_CLIENT_SYSTEM:        actor_flag = 0; break;
      case actor::CATEGORY_SERVER_GATE:                 actor_flag = 0; break;
      case actor::CATEGORY_SERVER_CMS:                  actor_flag = 0; break;
      case actor::CATEGORY_CLIENT_SYSTEM:               actor_flag = 1; break;
      case actor::CATEGORY_CLIENT_CMS:                  actor_flag = 1; break;
      case actor::CATEGORY_CLIENT_GATE:                 actor_flag = 1; break;
      case actor::CATEGORY_SUBCONTRACTOR:               actor_flag = 2; break;
      default:
        actor_flag = 3;
      }
      if (actor_flag == 3) return false;
      std::size_t bit = actor_flag * 3 + perm_flag;
      return _permissions_.test(bit);
    }

    bool mailbox::is_locked() const
    {
      return _locked_;
    }

    bool mailbox::is_domain() const
    {
      return _domain_;
    }

    void mailbox::lock(bool domain_)
    {
      _locked_ = true;
      _domain_ = domain_;
      return;
    }

    void mailbox::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name        : '" << _name_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Address     : '" << _address_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Mode        : '" << mode_label(_mode_) << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Privacy     : '" << privacy_label(_privacy_) << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Permissions : '" << usage_permission_to_string(_permissions_) << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Validity    : " << std::boolalpha << is_valid() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Domain      : " << std::boolalpha << is_domain() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Locked      : " << std::boolalpha << is_locked() << std::endl;

      return;
    }

  } // namespace com

} // namespace vire
