//! \file vire/cmsserver/uc_resource_mount_link.cc
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
#include <vire/cmsserver/uc_resource_mount_link.h>
  
// Third party:
// - Boost:
#include <boost/algorithm/string/trim.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>

// This project:
#include <vire/utility/path.h>

namespace vire {

  namespace cmsserver {
        
    uc_resource_mount_link::uc_resource_mount_link()
    {
      return;
    }
              
    uc_resource_mount_link::uc_resource_mount_link(const uc_port_id & from_id_,
                                                   const std::string & to_key_)
    {
      set_from_id(from_id_);
      set_to_key(to_key_);
      return;
    }
            
    uc_resource_mount_link::uc_resource_mount_link(const uc_port_id & from_id_,
                                                   const std::string & to_key_,
                                                   const vire::utility::relative_path & relpath_)
    {
      set_from_id(from_id_);
      set_to_key(to_key_);
      set_relative_path(relpath_);
      return;
    }
           
    uc_resource_mount_link::uc_resource_mount_link(const uc_port_id & from_id_,
                                                   const std::string & to_key_,
                                                   const std::string & relpath_repr_)
    {
      set_from_id(from_id_);
      set_to_key(to_key_);
      set_relative_path(relpath_repr_);
      return;
    }

    bool uc_resource_mount_link::is_complete() const
    {
      if (!has_from_id()) return false;
      if (!has_to_key()) return false;
      return true;
    }
        
    bool uc_resource_mount_link::has_from_id() const
    {
      return _from_id_.has_port_key();
    }

    const uc_port_id & uc_resource_mount_link::get_from_id() const
    {
      return _from_id_;
    }

    void uc_resource_mount_link::set_from_id(const uc_port_id & from_id_)
    {
      _from_id_ = from_id_;
      return;
    }
         
    bool uc_resource_mount_link::has_to_key() const
    {
      return !_to_key_.empty();
    }

    void uc_resource_mount_link::set_to_key(const std::string & to_key_)
    {
      if (! vire::utility::path::validate_path_segment(to_key_)) {
        DT_THROW(std::logic_error, "Invalid link to key '" << to_key_ << "'!");
      }
      _to_key_ = to_key_;
      return;
    }
   
    const std::string & uc_resource_mount_link::get_to_key() const
    {
      return _to_key_;
    }
    
    void uc_resource_mount_link::reset_to_key()
    {
      _to_key_.clear();
      return;
    }
 
    void uc_resource_mount_link::reset_relative_path()
    {
      _relpath_.clear();
      return;
    }

    const vire::utility::relative_path & uc_resource_mount_link::get_relative_path() const
    {
      return _relpath_;
    }

    void uc_resource_mount_link::set_relative_path(const vire::utility::relative_path & relpath_)
    {
      _relpath_ = relpath_;
      return;
    }

    void uc_resource_mount_link::set_relative_path(const std::string & relpath_repr_)
    {
      if (relpath_repr_.empty()) {
        _relpath_.clear();
        return;
      }
      vire::utility::relative_path rp;
      DT_THROW_IF(!rp.from_string(relpath_repr_),
                  std::logic_error,
                  "Invalid relative path representation '" << relpath_repr_ << "'");
      _relpath_ = rp;
      return;
    }

    bool uc_resource_mount_link::has_relative_path() const
    {
      return !_relpath_.is_empty();
    }

    std::string uc_resource_mount_link::to_string() const
    {
      std::ostringstream out;
      out << *this;
      return out.str();
    }

    bool uc_resource_mount_link::from_string(const std::string & repr_)
    {
      std::string repr = repr_;
      boost::algorithm::trim(repr); 
      std::size_t arrow_found = repr.find("->");
      if (arrow_found == repr.npos) return false;
      std::string s_from = repr.substr(0, arrow_found);
      std::string s_to = repr.substr(arrow_found + 2);
      uc_port_id from_id;
      std::string to_key;
      vire::utility::relative_path relpath;
      if (!from_id.from_string(s_from)) {
        return false;
      }
      std::size_t relpath_sep_found = s_to.find('[');
      if (relpath_sep_found == repr.npos) {
        to_key = s_to;
      } else {
        if (s_to.back() != ']') {
          return false;
        }
        s_to.pop_back();
        to_key = s_to.substr(0, relpath_sep_found);
        std::string relpath_repr = s_to.substr(relpath_sep_found+1);
        if (!relpath.from_string(relpath_repr)) {
          return false;
        }
      }
      if (!vire::utility::path::validate_path_segment(to_key)) {
        return false;
      }
      _from_id_ = from_id;
      _to_key_  = to_key;
      _relpath_ = relpath;
      return true;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const uc_resource_mount_link & mlink_)
    {
      out_ << mlink_._from_id_ << "->" << mlink_._to_key_;
      if (mlink_.has_relative_path()) {
        out_ << '[' << mlink_._relpath_ << ']';
      }
      return out_;
    }
     
  } // namespace cmsserver

} // namespace vire
