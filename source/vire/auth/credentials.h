//! \file  vire/auth/credentials.h
//! \brief Vire auth credentials
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

#ifndef VIRE_AUTH_CREDENTIALS_H
#define VIRE_AUTH_CREDENTIALS_H

// Standard library:
#include <string>

// Third party:
// - Boost:
#include <boost/cstdlib.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace auth {

    //! \brief User credentials data
    class credentials
      : public datatools::i_tree_dumpable
    {
    public:

      static const int32_t FORCE_PASSWORD_CHANGE_NEXT = 0;
      static const int32_t DATE_UNSET            = -1;
      static const int32_t DEFAULT_PASS_MAX_DAYS = 99999;
      static const int32_t DEFAULT_PASS_MIN_DAYS =     0;
      static const int32_t DEFAULT_PASS_WARN_AGE =     7;

      /// Default constructor
      credentials();

      /// Destructor
      ~credentials();

      /// Check validity
      bool is_valid() const;

      /// Reset
      void reset();

      bool has_login() const;

      bool is_locked() const;

      void lock();

      void unlock();

      bool has_password() const;

      void set_login(const std::string & login_);

      const std::string & get_login() const;

      bool set_password(const std::string & password_);

      void set_password_encrypted(const std::string & password_encrypted_);

      bool can_change_password() const;

      bool must_change_password() const;

      void set_password_last_change(int32_t);

      int32_t get_password_last_change() const;

      void set_password_min(int32_t);

      int32_t get_password_min() const;

      void set_password_max(int32_t);

      int32_t get_password_max() const;

      void set_password_warn(int32_t);

      int32_t get_password_warn() const;

      void set_password_inact(int32_t);

      int32_t get_password_inact() const;

      void set_password_expire(int32_t);

      int32_t get_password_expire() const;

      bool is_password_expired() const;

      void set_default_min_max_warn();

      bool match_password(const std::string & password_, bool clear_ = true) const;

      const std::string & get_password_encrypted() const;

      const std::string & get_password() const;

      /// Parse from a character string record
      bool from_table_record_line(const std::string & record_);

      /// Print a table record line
      void to_table_record_line(std::ostream & out_) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      // see shadow.h header
      std::string _login_;                 ///< Login name
      bool        _password_lock_ = false; ///< Password lock flag
      std::string _password_;              ///< Plain password
      std::string _password_encrypted_;    ///< Encrypted password
      int32_t     _password_last_change_ = DATE_UNSET;  ///< Date of last change
      int32_t     _password_min_   = DATE_UNSET; ///< Minimum number of days between changes
      int32_t     _password_max_   = DATE_UNSET; ///< Maximum number of days between changes.
      int32_t     _password_warn_  = DATE_UNSET; ///< Number of days to warn user to change the password
      int32_t     _password_inact_ = DATE_UNSET; ///< Number of days the account may be inactive
      int32_t     _password_expire_ = DATE_UNSET; ///< Number of days since 1970-01-01 until account expires.

    };

    typedef std::map<std::string, credentials> credentials_dict_type;

  } // namespace auth

} // namespace vire

#endif // VIRE_AUTH_CREDENTIALS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
