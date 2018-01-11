//! \file vire/auth/manager.cc
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
#include <vire/auth/manager.h>

// Standard library:
#include <fstream>

// Third party:
// - Boost:
#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/auth/credentials.h>
#include <vire/user/manager.h>

namespace vire {

  namespace auth {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(manager, "vire::auth::manager");

    // static
    const int manager::DEFAULT_EXPIRE_AFTER;

    // static
    const std::string & manager::default_service_name()
    {
      static const std::string _n("auth");
      return _n;
    }

    manager::auth_status manager::authenticate(const std::string & login_,
                                               const std::string & password_) const
    {
      if (!has_credentials(login_)) {
        return AUTH_INVALID_LOGIN;
      }
      const credentials & cred = get_credentials(login_);
      if (cred.is_locked()) {
        return AUTH_LOCKED;
      }
      if (cred.is_password_expired()) {
        return AUTH_EXPIRED;
      }
      if (cred.has_password()) {
        if (!cred.match_password(password_)) {
          return AUTH_INVALID_PASSWORD;
        }
      }
      return AUTH_SUCCESS;
    }

    manager::manager(uint32_t flags_)
    {
      _initialized_ = false;
      _set_defaults();
      if (flags_ & LOG_TRACE) {
        set_logging_priority(datatools::logger::PRIO_TRACE);
      }
      if (flags_ & DONT_LOAD_TABLES) {
        set_dont_load_tables(true);
      }
      if (flags_ & DONT_STORE_TABLES) {
        set_dont_store_tables(true);
      }
      if (flags_ & DONT_BACKUP_TABLES) {
        set_dont_backup_tables(true);
      }
      return;
    }

    manager::~manager()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool manager::has_users() const
    {
      return _users_ != nullptr;
    }

    void manager::set_users(const vire::user::manager & users_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Authentication manager is initialized!");
      _users_ = &users_;
      return;
    }

    const vire::user::manager & manager::get_users() const
    {
      DT_THROW_IF(!has_users(), std::logic_error,
                  "Users manager is not available!");
      return *_users_;
    }

    bool manager::has_credentials_table_path() const
    {
      return !_credentials_table_path_.empty();
    }

    const std::string & manager::get_credentials_table_path() const
    {
      return _credentials_table_path_;
    }

    void manager::set_credentials_table_path(const std::string & path_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Authentication manager is initialized!");
      _credentials_table_path_ = path_;
      return;
    }

    bool manager::dont_load_tables() const
    {
      return _dont_load_tables_;
    }

    bool manager::is_load_tables() const
    {
      return _dont_load_tables_ == false;
    }

    bool manager::dont_store_tables() const
    {
      return _dont_store_tables_;
    }

    bool manager::is_store_tables() const
    {
      return _dont_store_tables_ == false;
    }

    bool manager::dont_backup_tables() const
    {
      return _dont_backup_tables_;
    }

    bool manager::is_backup_tables() const
    {
      return _dont_backup_tables_ == false;
    }

    void manager::set_dont_load_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Auth manager is already initialized!");
      _dont_load_tables_ = flag_;
      DT_LOG_TRACE(get_logging_priority(), "Set dont_load_tables = " << _dont_load_tables_);
      return;
    }

    void manager::set_dont_store_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Auth manager is already initialized!");
      _dont_store_tables_ = flag_;
      DT_LOG_TRACE(get_logging_priority(), "Set dont_store_tables = " << _dont_store_tables_);
      return;
    }

    void manager::set_dont_backup_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Auth manager is already initialized!");
      _dont_backup_tables_ = flag_;
      DT_LOG_TRACE(get_logging_priority(), "Set dont_backup_tables = " << _dont_backup_tables_);
      return;
    }

    bool manager::has_credentials(const std::string & login_) const
    {
      return _credentials_.count(login_) > 0;
    }

    bool manager::is_credentials_valid(const std::string & login_) const
    {
      DT_THROW_IF(!has_credentials(login_), std::logic_error,
                  "Credentials with login '" << login_ << "' does not exist!");
      const credentials & cred = get_credentials(login_);
      return cred.is_valid();
    }

    const credentials & manager::get_credentials(const std::string & login_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Auth manager is not initialized!");
      credentials_dict_type::const_iterator found = _credentials_.find(login_);
      DT_THROW_IF (found == _credentials_.end(), std::logic_error,
                   "Cannot find credentials with login='" << login_ << "'!");
      return found->second;
    }

    credentials & manager::grab_credentials(const std::string & login_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Auth manager is not initialized!");
      credentials_dict_type::iterator found = _credentials_.find(login_);
      DT_THROW_IF (found == _credentials_.end(), std::logic_error,
                   "Cannot find credentials with login='" << login_ << "'!");
      return found->second;
    }

    credentials & manager::create_new_credentials(const std::string & login_,
                                                  uint32_t flags_,
                                                  const std::string & password_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Auth manager is not initialized!");
      return _create_new_credentials(login_, flags_, password_);
    }

    void manager::remove_credentials(const std::string & login_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Auth manager is not initialized!");
      std::string removed_login;
      for (credentials_dict_type::iterator i = _credentials_.begin();
           i != _credentials_.end();
           i++) {
        if (i->second.get_login() == login_) {
          removed_login = i->first;
          break;
        }
      }
      DT_THROW_IF(removed_login.empty(),
                  std::logic_error,
                  "Credentials with login '" << login_ << "' does not exist!");
      _remove_credentials(login_);
      return;
    }

    void manager::remove_all_credentials()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Auth manager is not initialized!");
      _credentials_.clear();
      return;
    }

    void manager::load_tables(uint32_t load_flags_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Auth manager is not initialized!");
      // First load users:
      remove_all_credentials();
      DT_THROW_IF(!has_credentials_table_path(),
                  std::logic_error,
                  "No credentials table path is defined!");
      _load_credentials_table(_credentials_table_path_, load_flags_);
      return;
    }

    void manager::store_tables(uint32_t store_flags_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Auth manager is not initialized!");
      DT_THROW_IF(!has_credentials_table_path(),
                  std::logic_error,
                  "No credentials table path is defined!");
      _store_credentials_table(_credentials_table_path_, store_flags_);
      return;
    }

    void manager::_remove_credentials(const std::string & login_)
    {
      credentials_dict_type::iterator found = _credentials_.find(login_);
      DT_THROW_IF(found == _credentials_.end(), std::logic_error,
                  "Credentials with login '" << login_ << "' does not exist!");
      _credentials_.erase(found);
      return;
    }


    void manager::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Credentials table path : '"
           << _credentials_table_path_ << "'"
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Don't load tables : '" << _dont_load_tables_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Don't store tables : '" << _dont_store_tables_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Don't backup tables : '" << _dont_backup_tables_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Users service : ";
      if (has_users()) {
        out_ << "'" << _users_->get_name() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Credentials : ";
      if (_credentials_.size()) {
        out_ << std::endl;
        for (credentials_dict_type::const_iterator i = _credentials_.begin();
             i != _credentials_.end();
             i++) {
          out_ << indent_ << datatools::i_tree_dumpable::last_skip_tag;
          credentials_dict_type::const_iterator j = i;
          if (++j == _credentials_.end()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "login='" << i->second.get_login() << "' : "
               << "lock='" << i->second.is_locked() << "' "
               << std::endl;
        }
      } else {
        out_ << "<none>" << std::endl;
      }

      return;
    }

    bool manager::has_users_service_name() const
    {
      return !_users_service_name_.empty();
    }

    void manager::set_users_service_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Auth manager is already initialized!");
      DT_THROW_IF(has_users(),  std::logic_error,
                  "Auth manager already has a users manager service!");
      _users_service_name_ = name_;
      return;
    }

    const std::string & manager::get_users_service_name() const
    {
      return _users_service_name_;
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
                  "Auth manager is already initialized!");

      if (datatools::logger::is_debug(get_logging_priority())) {
        config_.tree_dump(std::cerr, "Auth manager config: ", "[debug] ");
      }
      this->::datatools::base_service::common_initialize(config_);

      //DT_LOG_DEBUG(datatools::logger::PRIO_ALWAYS, "Name = '" << get_name() << "'");
      //DT_LOG_DEBUG(datatools::logger::PRIO_ALWAYS, "Logging priority = " << datatools::logger::get_priority_label(get_logging_priority()));
      //DT_LOG_DEBUG(datatools::logger::PRIO_ALWAYS, "Credentials table path = " << _credentials_table_path_);

      if (!has_users()) {
        if (_users_service_name_.empty()) {
          std::string users_service_name;
          if (config_.has_key("users_service_name")) {
            users_service_name = config_.fetch_string("users_service_name");
          } else {
            users_service_name = vire::user::manager::default_service_name();
          }
          set_users_service_name(users_service_name);
          DT_LOG_DEBUG(get_logging_priority(),
                       "Users service name = '" << _users_service_name_ << "'");
        }
        if (!has_users_service_name()) {
          const vire::user::manager & userMgr
            = datatools::get<vire::user::manager>(service_dict_,
                                                  _users_service_name_);
          set_users(userMgr);
        }
      }

      DT_LOG_DEBUG(get_logging_priority(), "Credentials table path = " << _credentials_table_path_);
      if (_credentials_table_path_.empty()) {
        DT_LOG_DEBUG(get_logging_priority(), "No credentials table path...");
        if (config_.has_key("credentials_table_path")) {
          DT_LOG_DEBUG(get_logging_priority(), "Found key 'credentials_table_path'!");
          set_credentials_table_path(config_.fetch_string("credentials_table_path"));
        }
      }
      DT_LOG_DEBUG(get_logging_priority(), "Credentials table path = " << _credentials_table_path_);

      if (config_.has_key("dont_load_tables")) {
        set_dont_load_tables(config_.fetch_boolean("dont_load_tables"));
      }

      if (config_.has_key("dont_store_tables")) {
        set_dont_store_tables(config_.fetch_boolean("dont_store_tables"));
      }

      if (is_store_tables()) {
        if (config_.has_key("dont_backup_tables")) {
          set_dont_backup_tables(config_.fetch_boolean("dont_backup_tables"));
        }
      }

      // Checks:
      DT_THROW_IF(!has_credentials_table_path(), std::logic_error,
                  "Missing credentials table path!");

      // Initialization:
      std::string tmp_path = _credentials_table_path_;
      datatools::fetch_path_with_env(tmp_path);
      if (boost::filesystem::exists(tmp_path)) {
        if (is_load_tables()) {
          DT_LOG_DEBUG(get_logging_priority(), "Loading credentials table...");
          _load_credentials_table(_credentials_table_path_, 0);
          DT_LOG_DEBUG(get_logging_priority(), "Loading credentials table done.");
        }
      } else {
        DT_LOG_DEBUG(get_logging_priority(), "No credentials table file '" << _credentials_table_path_ << "' exists!");
      }

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    int manager::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Auth manager is not initialized!");
      _initialized_ = false;
      if (is_store_tables()) {
        _store_credentials_table(_credentials_table_path_, 0);
      }
      _credentials_.clear();
      _set_defaults();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void manager::_set_defaults()
    {
      _dont_load_tables_ = false;
      _dont_store_tables_ = false;
      _dont_backup_tables_ = false;
      _credentials_table_path_ = "";
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _users_ = nullptr;
      return;
    }

    // virtual
    void manager::_store_credentials_table(const std::string & target_, uint32_t /* flags_ */) const
    {
      std::string target = target_;
      datatools::fetch_path_with_env(target);

      if (boost::filesystem::exists(target)) {
        DT_LOG_DEBUG(get_logging_priority(), "Credentials table file '" << target << "' exists...");
        DT_THROW_IF(! boost::filesystem::is_regular_file(target),
                    std::runtime_error,
                    "Target '" << target_ << "' is not a regular file!");
        if (is_backup_tables()) {
          DT_LOG_DEBUG(get_logging_priority(), "Backup credentials table...");
          std::ostringstream backup_target_oss;
          backup_target_oss << target << "~";
          std::string backup_target = backup_target_oss.str();
          std::string tmp_backup_target = backup_target + ".tmp";
          if (boost::filesystem::exists(backup_target)) {
            DT_LOG_DEBUG(get_logging_priority(), "Backuped credentials table '" << backup_target << "' exists...");
            if (boost::filesystem::exists(tmp_backup_target)) {
              boost::filesystem::remove(tmp_backup_target);
            }
            DT_LOG_DEBUG(get_logging_priority(), "Rename in safe tmp file '" << tmp_backup_target << "'...");
            boost::filesystem::rename(backup_target, tmp_backup_target);
          }
          boost::system::error_code ec;
          boost::filesystem::copy(boost::filesystem::path(target),
                                  boost::filesystem::path(backup_target),
                                  ec);
          if (ec) {
            boost::filesystem::rename(tmp_backup_target, backup_target);
            DT_LOG_DEBUG(get_logging_priority(), "Restore the safe tmp file '" << tmp_backup_target << "' to '" << backup_target << "'...");
            DT_THROW(std::runtime_error,
                     "Cannot backup the credentials table '" << backup_target << "' : "
                     << ec.message() << "!");
          } else {
            if (boost::filesystem::exists(tmp_backup_target)) {
              boost::filesystem::remove(tmp_backup_target);
              DT_LOG_DEBUG(get_logging_priority(), "Remove the safe tmp file '" << tmp_backup_target << "'...");
            }
          }
        } else {
          boost::filesystem::remove(target);
        }
      }

      std::ofstream fcredentials(target.c_str());
      DT_THROW_IF(!fcredentials, std::runtime_error,
                  "Cannot open credentials table file '" << target << "'!");
      for (std::map<std::string, credentials>::const_iterator icred = _credentials_.begin();
           icred != _credentials_.end();
           icred++) {
        icred->second.to_table_record_line(fcredentials);
      }
      return;
    }

    void manager::_load_credentials_table(const std::string & source_, uint32_t /* flags_ */)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      std::string source = source_;
      datatools::fetch_path_with_env(source);

      std::ifstream fin;
      fin.open(source.c_str());
      DT_THROW_IF(!fin, std::runtime_error,
                  "Cannot open credentials file '" << source << "'!");
      std::size_t line_count = 0;
      while (fin) {
        std::string line;
        std::getline(fin, line);
        line_count++;
        {
          std::string word;
          std::istringstream check(line);
          check >> word;
          if (word.empty() || word[0] == '#') {
            continue;
          }
        }
        credentials cred;
        DT_THROW_IF(! cred.from_table_record_line(line), std::logic_error,
                    "Invalid credentials format at line [" << line_count << "] in file '"
                    << source << "'!");
        _credentials_[cred.get_login()] = cred;
        if (fin.eof()) {
          break;
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    credentials & manager::_create_new_credentials(const std::string & login_,
                                                   uint32_t flags_,
                                                   const std::string & password_)
    {
      DT_THROW_IF(has_credentials(login_), std::logic_error,
                  "Credentials with login '" << login_ << "' already exists!");
      if (flags_ & create_check_login) {
        DT_LOG_DEBUG(get_logging_priority(),
                     "Checking login '" << login_ << "'...");
        DT_THROW_IF(!has_users(), std::logic_error,
                    "Auth manager has no users service!");
        DT_THROW_IF(!_users_->has_user_by_name(login_),
                    std::logic_error,
                    "Users manager has not user with login '" << login_ << "'!");
      }
      {
        credentials cred;
        cred.set_login(login_);
        if (flags_ & create_defaults) {
          cred.set_default_min_max_warn();
        }
        if (flags_ & create_lock) {
          cred.lock();
        }
        if (flags_ & create_no_expiration) {
          cred.set_password_expire(credentials::DATE_UNSET);
        } else {
          cred.set_password_expire(99999);
        }
        if (flags_ & create_with_password) {
          DT_THROW_IF(password_.empty(), std::logic_error,
                      "Empty password!");
          cred.set_password(password_);
          // std::string * p = const_cast<std::string *>(&password_):
          // p->clear();
          // p->shrink_to_fit();
        }
        if (!cred.has_password()) {
          cred.lock();
        }
        _credentials_[login_] = cred;
      }
      credentials & new_cred = _credentials_.find(login_)->second;
      return new_cred;
    }

  } // namespace auth

} // namespace vire
