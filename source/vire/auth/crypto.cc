//! \file vire/auth/crypto.cc
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
#include <vire/auth/crypto.h>

// Standard library:
#include <random>
#include <string>
#include <sstream>
#include <iostream>
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif // _XOPEN_SOURCE
#include <crypt.h>

// Third party:
#include <datatools/logger.h>

namespace vire {

  namespace auth {

    bool sha512::extract_salt(const std::string & encrypted_, std::string & salt_)
    {
      salt_.clear();
      if (encrypted_.substr(0, 3) != "$6$") {
        return false;
      }
      std::string s = encrypted_.substr(3);
      std::size_t pos = s.find("$");
      if (pos != 16) {
        return false;
      }
      salt_ = s.substr(0, pos);
      return true;
    }

    bool sha512::match(const std::string & key_, const std::string & encrypted_)
    {
      std::string salt;
      if (!sha512::extract_salt(encrypted_, salt)) {
        return false;
      }
      std::string encrypted_candidate;
      if (!sha512::encrypt(key_, encrypted_candidate, sha512::encrypt_force_salt, salt)) {
        return false;
      }
      return encrypted_candidate == encrypted_;
    }

    bool sha512::encrypt(const std::string & key_,
                         std::string & encrypted_,
                         uint32_t flags_,
                         const std::string & salt_)
    {
      static const std::string saltchars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./");
      bool debug = false;
      bool force_salt = false;
      bool clear_key = false;
      if (flags_ & sha512::encrypt_clear_key) {
        clear_key = true;
      }
      if (flags_ & sha512::encrypt_debug) {
        debug = true;
      }
      if (flags_ & sha512::encrypt_force_salt) {
        if (salt_.length() != 16) {
          // Invalid salt:
          if (debug) {
            DT_LOG_DEBUG(datatools::logger::PRIO_DEBUG, "Invalid salt! Abandon!");
          }
          return false;
        }
        std::size_t found_invalid_char = salt_.find_first_not_of(saltchars);
        if (found_invalid_char != std::string::npos) {
          // Invalid salt:
          if (debug) {
            DT_LOG_DEBUG(datatools::logger::PRIO_DEBUG, "Invalid salt! Abandon!");
          }
          return false;
        }
        force_salt = true;
      }
      struct crypt_data cryptdata;
      cryptdata.initialized = 0;
      const char * key = key_.c_str();
      const char * salt = nullptr;
      std::ostringstream cppsalt_oss;
      cppsalt_oss << "$6$";
      if (force_salt) {
        cppsalt_oss << salt_;
      } else {
        // Randomly generate a 16 characters salt:
        if (debug) {
          DT_LOG_DEBUG(datatools::logger::PRIO_DEBUG, "Randomly generate a 16 characters salt...");
        }
        unsigned int time_ui =(unsigned int)( time(NULL) );
        auto const seed = std::random_device()();
        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> distribution(0, saltchars.length() - 1);
        for (int i = 0; i < 16; i++) {
          int index = distribution(generator);
          cppsalt_oss << saltchars[index];
        }
      }
      cppsalt_oss << '$';
      std::string cppsalt = cppsalt_oss.str();
      if (debug) {
        DT_LOG_DEBUG(datatools::logger::PRIO_DEBUG, "cppsalt = '" << cppsalt << "'");
      }
      salt = cppsalt.c_str();
      if (debug) {
        DT_LOG_DEBUG(datatools::logger::PRIO_DEBUG, "salt = '" << salt << "'");
      }
      char * encrypted = crypt_r(key, salt, &cryptdata);
      if (encrypted == nullptr) {
        DT_LOG_DEBUG(datatools::logger::PRIO_DEBUG, "salt = '" << salt << "'");
      }
      if (clear_key) {
        std::string * key_ptr = const_cast<std::string *>(&key_);
        key_ptr->clear();
        key_ptr->shrink_to_fit();
      }
      if (debug) {
        DT_LOG_DEBUG(datatools::logger::PRIO_DEBUG, "encrypted = '" << encrypted << "'");
      }
      if (encrypted != NULL) {
        encrypted_ = std::string(encrypted);
        if (debug) {
          DT_LOG_DEBUG(datatools::logger::PRIO_DEBUG, "encrypted string = '" << encrypted_ << "'");
        }
      } else {
        return false;
      }
      return true;
    }

  } // namespace auth

} // namespace vire
