//! \file vire/cmsserver/uc_scope_resources.cc
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
#include <vire/cmsserver/uc_scope_resources.h>
       
// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
 
namespace vire {

  namespace cmsserver {
    
    uc_scope_resources::uc_scope_resources()
    {
      return;
    }

    // virtual
    uc_scope_resources::~uc_scope_resources()
    {
      return;
    }

    void uc_scope_resources::add_absolute_resource(const vire::utility::path & p_)
    {
      DT_THROW_IF(is_locked(), std::logic_error,
                  "Lists of scope resources is locked!");
      _absolute_resources_.insert(p_);
      return;
    }
 
    void uc_scope_resources::add_relative_resource(const vire::utility::relative_path & rp_)
    {
      DT_THROW_IF(is_locked(), std::logic_error,
                  "Lists of scope resources is locked!");
      _relative_resources_.insert(rp_);
      return;
    }

    bool uc_scope_resources::has_absolute_resource(const vire::utility::path & p_) const
    {
      return _absolute_resources_.count(p_);
    }

    bool uc_scope_resources::has_relative_resource(const vire::utility::relative_path & rp_) const
    {
      return _relative_resources_.count(rp_);
    }
   
    const std::set<vire::utility::relative_path> & uc_scope_resources::get_relative_resources() const
    {
      return _relative_resources_;
    }

    const std::set<vire::utility::path> & uc_scope_resources::get_absolute_resources() const
    {
      return _absolute_resources_;
    }

    void uc_scope_resources::lock()
    {
      _locked_ = true;
      return;
    }

    bool uc_scope_resources::is_locked() const
    {
      return _locked_;
    }
 
    void uc_scope_resources::print_tree(std::ostream & out_,
                                        const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      out_ << popts.indent << tag
           << "Relative resources : " << _relative_resources_.size()
           << std::endl;

      out_ << popts.indent << tag
           << "Absolute resources : " << _absolute_resources_.size()
           << std::endl;
    
      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Locked : " << std::boolalpha << is_locked()
           << std::endl;
      
      return;
    }

  } // namespace cmsserver

} // namespace vire
