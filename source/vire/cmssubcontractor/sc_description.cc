//! \file vire/cmssubcontractor/sc_description.cc
//
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmssubcontractor/sc_description.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
namespace vire {

  namespace cmssubcontractor {

    sc_description::sc_description()
    {
      return;
    }
    
    sc_description::~sc_description()
    {
      return;
    }

    bool sc_description::is_complete() const
    {
      if (!has_name()) {
        return false;
      }
      if (!has_com_login()) {
        return false;
      }
      if (!has_com_password()) {
        return false;
      }
      if (!has_com_server_host()) {
        return false;
      }
      if (!has_com_server_port()) {
        return false;
      }
      return true;
    }

    bool sc_description::has_com_login() const
    {
      return !_com_login_.empty();
    }
    
    const std::string & sc_description::get_com_login() const
    {
      return _com_login_;
    }
 
    void sc_description::set_com_login(const std::string & login_)
    {
      _com_login_ = login_;
      return;
    }
     
    bool sc_description::has_com_password() const
    {
      return !_com_password_.empty();
    }
  
    const std::string & sc_description::get_com_password() const
    {
      return _com_password_;
    }
   
    void sc_description::set_com_password(const std::string & password_)
    {
      _com_password_ = password_;
      return;
    }
  
    bool sc_description::has_com_server_host() const
    {
      return !_com_server_host_.empty();
    }

    const std::string & sc_description::get_com_server_host() const
    {
      return _com_server_host_;
    }
   
    void sc_description::set_com_server_host(const std::string & host_)
    {
      _com_server_host_ = host_;
      return;
    }
   
    bool sc_description::has_com_server_port() const
    {
      return _com_server_port_ > 0;
    }
    
    int sc_description::get_com_server_port() const
    {
      return _com_server_port_;
    }
    
    void sc_description::set_com_server_port(const int port_)
    {
      _com_server_port_ = port_;
      return;
    }
    
    void sc_description::initialize(const datatools::properties & config_)
    {
      ::datatools::enriched_base::initialize(config_, false);

      if (!has_com_login()) {
        if (config_.has_key("com_login")) {
          set_com_login(config_.fetch_string("com_login"));
        }
      }

      if (!has_com_password()) {
        if (config_.has_key("com_password")) {
          set_com_password(config_.fetch_string("com_password"));
        }
      }

      if (!has_com_server_host()) {
        if (config_.has_key("com_server_host")) {
          set_com_server_host(config_.fetch_string("com_server_host"));
        }
      }

      if (!has_com_server_port()) {
        if (config_.has_key("com_server_port")) {
          set_com_server_port(config_.fetch_positive_integer("com_server_port"));
        }
      }
      
      return;
    }

    void sc_description::reset()
    {
      _com_login_.clear();
      _com_password_.clear();
      _com_server_host_.clear();
      _com_server_port_ = 0;
      
      ::datatools::enriched_base::reset();
      return;
    }
   
    // virtual
    void sc_description::print_tree(std::ostream & out_,
                                    const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      ::datatools::enriched_base::tree_dump(out_, "", popts.indent, true);
      
      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Complete : " << std::boolalpha << is_complete()
           << std::endl;

      return;
    }
       
  } // namespace cmssubcontractor

} // namespace vire

