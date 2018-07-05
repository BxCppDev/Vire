//! \file vire/cmsserver/uc_mounting_table.cc
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
#include <vire/cmsserver/uc_mounting_table.h>
  
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

    uc_resource_mounting_table::uc_resource_mounting_table()
    {
      return;
    }
      
    // virtual
    uc_resource_mounting_table::~uc_resource_mounting_table()
    {
      return;
    }

    bool uc_resource_mounting_table::has(const uc_port_id & from_id_) const
    {
      for (const auto & item : _mount_links_) {
        if (item.get_from_id() == from_id_) return true;
      }
      return false;
    }

    void uc_resource_mounting_table::add(const uc_resource_mount_link & mount_link_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Mounting table is locked!");
      DT_THROW_IF(has(mount_link_.get_from_id()),
                  std::logic_error,
                  "Mounting table already has a link from ID = '"
                  << mount_link_.get_from_id() << "'!");
      _mount_links_.push_back(mount_link_);
      return;
    }
   
    void uc_resource_mounting_table::add(const uc_port_id & from_id_,
                                         const std::string & to_key_,
                                         const vire::utility::relative_path & relpath_)
    {
      add(uc_resource_mount_link(from_id_, to_key_, relpath_));
      return;
    }

    void uc_resource_mounting_table::add(const std::string & from_id_name_,
                                         const std::string & from_id_port_key_,
                                         const std::string & to_key_,
                                         const vire::utility::relative_path & relpath_)
    {
      add(uc_port_id(from_id_name_, from_id_port_key_),
          to_key_,
          relpath_);
      return;
    }
  
    void uc_resource_mounting_table::add(const uc_port_id & from_id_,
                                         const std::string & to_key_,
                                         const std::string & relative_path_)
    {
      add(uc_resource_mount_link(from_id_, to_key_, relative_path_));
      return;
    }
         
    void uc_resource_mounting_table::add(const std::string & from_id_name_,
                                         const std::string & from_id_port_key_,
                                         const std::string & to_key_,
                                         const std::string & relative_path_)
    {
      add(uc_port_id(from_id_name_, from_id_port_key_),
          to_key_,
          relative_path_);
      return;
    }
 
    const uc_resource_mount_link &
    uc_resource_mounting_table::get(const uc_port_id & from_id_) const
    {
      const uc_resource_mount_link * found = nullptr;
      for (const auto & item : _mount_links_) {
        if (item.get_from_id() == from_id_) {
          found = &item;
          break;
        }
      }
      DT_THROW_IF(found == nullptr, std::logic_error, "No resource mount link '" << from_id_ << "'!");
      return *found;
    }

    const uc_resource_mounting_table::link_list_type &
    uc_resource_mounting_table::get_mount_links() const
    {
      return _mount_links_;
    }
   
    void uc_resource_mounting_table::lock()
    {
      _locked_ = true;
      return;
    }
    
    bool uc_resource_mounting_table::is_locked() const
    {
      return _locked_;
    }

    void uc_resource_mounting_table::configure_only_for(const datatools::properties & config_,
                                                        const std::set<std::string> & allowed_from_names_)
    {
      std::vector<std::string> link_reprs;
      if (config_.has_key("links")) {
        config_.fetch("links", link_reprs);
      }
      for (int i = 0; i < link_reprs.size(); i++) {
        const std::string & link_repr = link_reprs[i];
        uc_resource_mount_link mlink;
        if (!mlink.from_string(link_repr)) {
          DT_THROW(std::logic_error,
                   "Invalid syntax for resource mount link '" << link_repr << "'!");
        }
        DT_THROW_IF(allowed_from_names_.count(mlink.get_from_id().get_name()) == 0,
                    std::logic_error,
                    "Mount link from daughter name '" << mlink.get_from_id().get_name() << "' is not allowed!");
        add(mlink);
      }
      return;
    }

    void uc_resource_mounting_table::configure(const datatools::properties & config_)
    {
      std::vector<std::string> link_reprs;
      if (config_.has_key("links")) {
        config_.fetch("links", link_reprs);
      }
      for (int i = 0; i < link_reprs.size(); i++) {
        const std::string & link_repr = link_reprs[i];
        uc_resource_mount_link mlink;
        if (!mlink.from_string(link_repr)) {
          DT_THROW(std::logic_error,
                   "Invalid syntax for resource mount link '" << link_repr << "'!");
        }
        add(mlink);
      }
      
      if (config_.has_key("locked")) {
        bool lck = config_.fetch_boolean("locked");
        if (lck) lock();
      }
      
      return;
    }
   
    // virtual
    void uc_resource_mounting_table::print_tree(std::ostream & out_,
                                                const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      out_ << popts.indent << tag
           << "Mount links : " << _mount_links_.size()
           << std::endl;
      {
        std::size_t count = 0;
        for (const auto & ml : _mount_links_) {
          out_ << popts.indent << skip_tag;
          if (++count == _mount_links_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Link '" << ml << "'";
          out_ << std::endl;
        }
      }
      
      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Locked : " << std::boolalpha << is_locked()
           << std::endl;
  
      return;
    }
    
  } // namespace cmsserver

} // namespace vire
