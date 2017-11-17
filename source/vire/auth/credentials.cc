//! \file vire/auth/credentials.cc
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/auth/credentials.h>

// Standard library:
#include <vector>

// Third party:
// - Boost:
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

// This project:
#include <vire/auth/crypto.h>
#include <vire/auth/utils.h>

namespace vire {

  namespace auth {

    // static
    const int32_t credentials::FORCE_PASSWORD_CHANGE_NEXT;
    const int32_t credentials::DATE_UNSET;
    const int32_t credentials::DEFAULT_PASS_MAX_DAYS;
    const int32_t credentials::DEFAULT_PASS_MIN_DAYS;
    const int32_t credentials::DEFAULT_PASS_WARN_AGE;

    credentials::credentials()
    {
      return;
    }

    credentials::~credentials()
    {
      return;
    }

    bool credentials::is_valid() const
    {
      if (!has_login()) return false;
      return true;
    }

    bool credentials::has_login() const
    {
      return !_login_.empty();
    }

    void credentials::set_login(const std::string & login_)
    {
      static const std::string allowed("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
      std::size_t found_invalid_char = login_.find_first_not_of(allowed);
      DT_THROW_IF(found_invalid_char != std::string::npos,
                  std::logic_error,
                  "Invalid login!");
      _login_ = login_;
      return;
    }

    const std::string & credentials::get_login() const
    {
      return _login_;
    }

    bool credentials::is_locked() const
    {
      return _password_lock_;
    }

    void credentials::lock()
    {
      _password_lock_ = true;
      return;
    }

    void credentials::unlock()
    {
      _password_lock_ = false;
      return;
    }

    bool credentials::has_password() const
    {
      return !_password_encrypted_.empty();
    }

    void credentials::set_password_encrypted(const std::string & password_encrypted_)
    {
      if (_password_encrypted_ != password_encrypted_) {
        _password_encrypted_ = password_encrypted_;
        _password_last_change_ = today_from_epoch(); // (int32_t) from_epoch.days() ;
      }
      return;
    }

    const std::string & credentials::get_password() const
    {
      return _password_;
    }

    bool credentials::set_password(const std::string & password_)
    {
      _password_ = password_;
      std::string encrypted_password;
      if (sha512::encrypt(password_, encrypted_password, sha512::encrypt_clear_key)) {
        set_password_encrypted(encrypted_password);
        return true;
      }
      return false;
    }

    const std::string & credentials::get_password_encrypted() const
    {
      return _password_encrypted_;
    }

    void credentials::set_default_min_max_warn()
    {
      _password_min_ = DEFAULT_PASS_MIN_DAYS;
      _password_max_ = DEFAULT_PASS_MAX_DAYS;
      _password_warn_ = DEFAULT_PASS_WARN_AGE;
      return;
    }

    void credentials::set_password_last_change(int32_t lc_)
    {
      _password_last_change_ = lc_ < DATE_UNSET ? DATE_UNSET : lc_;
      return;
    }

    int32_t credentials::get_password_last_change() const
    {
      return _password_last_change_;
    }

    void credentials::set_password_min(int32_t min_)
    {
      _password_min_ = min_ < DATE_UNSET ? DATE_UNSET : min_;
      return;
    }

    int32_t credentials::get_password_min() const
    {
      return _password_min_;
    }

    void credentials::set_password_max(int32_t max_)
    {
      _password_max_ = max_ < DATE_UNSET ? DATE_UNSET : max_;
      return;
    }

    int32_t credentials::get_password_max() const
    {
      return _password_max_;
    }

    void credentials::set_password_warn(int32_t warn_)
    {
      _password_warn_ = warn_ < DATE_UNSET ? DATE_UNSET : warn_;
      return;
    }

    int32_t credentials::get_password_warn() const
    {
      return _password_warn_;
    }

    void credentials::set_password_inact(int32_t inact_)
    {
      _password_inact_ = inact_ < DATE_UNSET ? DATE_UNSET : inact_;
      return;
    }

    int32_t credentials::get_password_inact() const
    {
      return _password_inact_;
    }

    void credentials::set_password_expire(int32_t expire_)
    {
      _password_expire_ = expire_ < DATE_UNSET ? DATE_UNSET : expire_;
      return;
    }

    int32_t credentials::get_password_expire() const
    {
      return _password_expire_;
    }

    bool credentials::is_password_expired() const
    {
      if (_password_expire_ == DATE_UNSET) return false;
      if (today_from_epoch() >= _password_expire_) {
        return true;
      }
      return false;
    }

    bool credentials::can_change_password() const
    {
      if (_password_min_ > 0) {
        if ((today_from_epoch() - _password_last_change_) < _password_min_) {
          return false;
        }
      }
      return true;
    }

    bool credentials::must_change_password() const
    {
      if (_password_max_ > 0 && _password_max_ > _password_min_) {
        if ((today_from_epoch() - _password_last_change_) >= _password_max_) {
          return true;
        }
      }
      return false;
    }

    bool credentials::match_password(const std::string & password_, bool clear_) const
    {
      bool match = vire::auth::sha512::match(password_, _password_encrypted_);
      if (clear_) {
        std::string * p = const_cast<std::string*>(&password_);
        p->clear();
        p->shrink_to_fit();
      }
      return match;
    }

    void credentials::reset()
    {
      _login_.clear();
      _password_lock_ = false;
      _password_encrypted_.clear();
      _password_last_change_  = DATE_UNSET;
      _password_min_   = DATE_UNSET;
      _password_max_   = DATE_UNSET;
      _password_warn_  = DATE_UNSET;
      _password_inact_ = DATE_UNSET;
      _password_expire_ = DATE_UNSET;
      return;
    }

    bool credentials::from_table_record_line(const std::string & record_)
    {
      static const char locksym('!');
      this->reset();
      std::vector<std::string> tokens;
      boost::split(tokens, record_, boost::is_any_of(":"));
      if (tokens.size() != 9) {
        return false;
      }
      try {
        set_login(tokens[0]);
        if (tokens[1].size() && tokens[1][0] == locksym) {
          _password_lock_ = true;
          set_password_encrypted(tokens[1].substr(1));
        } else {
          set_password_encrypted(tokens[1]);
        }
        if (tokens[2].length()) {
          set_password_last_change(boost::lexical_cast<int32_t>(tokens[2]));
        }
        if (tokens[3].length()) {
          set_password_min(boost::lexical_cast<int32_t>(tokens[3]));
        }
        if (tokens[4].length()) {
          set_password_max(boost::lexical_cast<int32_t>(tokens[4]));
        }
        if (tokens[5].length()) {
          set_password_warn(boost::lexical_cast<int32_t>(tokens[5]));
        }
        if (tokens[6].length()) {
          set_password_inact(boost::lexical_cast<int32_t>(tokens[6]));
        }
        if (tokens[7].length()) {
          set_password_expire(boost::lexical_cast<int32_t>(tokens[7]));
        }
        // tokens[8] unused
      } catch (std::exception & error) {
        return false;
      }
      return true;
    }

    void credentials::to_table_record_line(std::ostream & out_) const
    {
      static const char sep(':');
      static const char locksym('!');

      out_ << _login_ << sep;

      if (_password_lock_) {
        out_ << locksym;
      }
      out_ << _password_encrypted_;
      out_ << sep;

      if (_password_last_change_ >= 0) {
        out_ << _password_last_change_;
      }
      out_ << sep;

      if (_password_min_ >= 0) {
        out_ << _password_min_;
      }
      out_ << sep;

      if (_password_max_ >= 0) {
        out_ << _password_max_;
      }
      out_ << sep;

      if (_password_warn_ >= 0) {
        out_ << _password_warn_;
      }
      out_ << sep;

      if (_password_inact_ >= 0) {
        out_ << _password_inact_;
      }
      out_ << sep;

      if (_password_expire_ >= 0) {
        out_ << _password_expire_;
      }
      out_ << sep;

      out_ << std::endl;
      return;
    }

    void credentials::tree_dump(std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_,
                                bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Login : '" << _login_ << "'" << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Lock  : " << _password_lock_ << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Encrypted password   : '" << _password_encrypted_<< "'"  << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Password last change : " << _password_last_change_ << ""  << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Password min         : " << _password_min_ << ""  << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Password max         : " << _password_max_ << ""  << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Password warn        : " << _password_warn_ << ""  << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Password inact       : " << _password_inact_ << ""  << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Password expire      : " << _password_expire_ << ""  << std::endl;

      return;
    }

  } // namespace auth

} // namespace vire
