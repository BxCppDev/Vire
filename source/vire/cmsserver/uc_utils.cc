//! \file vire/cmsserver/uc_utils.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/uc_utils.h>

// Third party:
// - Boost:
#include <boost/algorithm/string/trim.hpp>
 
// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>

namespace vire {

  namespace cmsserver {

    bool validate_name(const std::string & name_)
    {
      static const uint32_t nv_flags = datatools::NV_NO_COLON
        | datatools::NV_NO_DOT
        | datatools::NV_NO_HYPHEN;
      return datatools::name_validation(name_, nv_flags);
    }

    uc_construction_context_type::uc_construction_context_type()
    {
     return;
    }

    uc_construction_context_type::uc_construction_context_type(const vire::utility::path & path_,
                                                               const std::string & model_id_,
                                                               const std::string & description_)
    {
      path = path_;
      model_id = model_id_;
      description = description_;
      return;
    }

    bool uc_construction_context_type::is_complete() const
    {
      if (!path.is_valid()) return false;
      if (model_id.empty()) return false;
      if (factory == nullptr) return false;
      if (devices == nullptr) return false;
      if (resources == nullptr) return false;
      return true;
    }
   
    uc_port_id::uc_port_id()
    {
      return;
    }

    uc_port_id::uc_port_id(const std::string & port_key_)
    {
      set_port_key(port_key_);
      return;
    }

    uc_port_id::uc_port_id(const std::string & name_,
                           const std::string & port_key_)
    {
      set_name(name_);
      set_port_key(port_key_);
      return;
    }

    bool uc_port_id::is_functional() const
    {
      return !has_name();
    }

    bool uc_port_id::is_distributable() const
    {
      return has_name();
    }

    bool uc_port_id::has_name() const
    {
      return !_name_.empty();
    }
        
    void uc_port_id::set_name(const std::string & name_)
    {
      DT_THROW_IF(!validate_name(name_), std::logic_error,
                  "Invalid name '" << name_ << "'!");
      _name_ = name_;
      return;
    }
                        
    const std::string & uc_port_id::get_name() const
    {
      return _name_;
    }
          
    bool uc_port_id::has_port_key() const
    {
      return !_port_key_.empty();
    }
                
    void uc_port_id::set_port_key(const std::string & port_key_)
    {
      DT_THROW_IF(!validate_name(port_key_), std::logic_error,
                  "Invalid port key '" << port_key_ << "'!");
      _port_key_ = port_key_;
      return;
    }
        
    const std::string & uc_port_id::get_port_key() const
    {
      return _port_key_;
    }

    bool uc_port_id::operator==(const uc_port_id & other_) const
    {
      if (this->_name_ != other_._name_) return false;
      if (this->_port_key_ != other_._port_key_) return false;
      return true;
    }
    
    bool uc_port_id::operator<(const uc_port_id & other_) const
    {
      if (this->_name_ < other_._name_) {
        return true;
      } else if (this->_name_ == other_._name_) {
        if (this->_port_key_ < other_._port_key_) {
          return true;
        } 
      }
      return false;
    }

    void uc_port_id::to_string(std::string & repr_) const
    {
      repr_ = this->to_string();
      return;
    }

    std::string uc_port_id::to_string() const
    {
      std::ostringstream out;
      out << *this;
      return out.str();
    }
  
    bool uc_port_id::from_string(const std::string & repr_)
    {
      std::string repr = repr_;
      boost::algorithm::trim(repr);
      if (repr.size() == 0) return false;
      std::string nm;
      std::string pk;
      if (repr[0] == '@') {
        std::size_t nfound = repr.find(':');
        if (nfound == repr.npos) return false;
        nm =  repr.substr(1, nfound - 1);
        pk = repr.substr(nfound + 1);
      } else {
        pk = repr;
      }
      if (!nm.empty()) {
        this->set_name(nm);
      }
      this->set_port_key(pk);
      return true;
    }
    
    void uc_port_id::reset()
    {
      _name_.clear();
      _port_key_.clear();
      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const uc_port_id & id_)
    {
      if (id_.has_name()) {
        out_ << "@" << id_._name_ << ":";
      }
      out_ << id_._port_key_;
      return out_;
    }
  
  } // namespace cmsserver

} // namespace vire
