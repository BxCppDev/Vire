//! \file vire/cmsserver/uc_composition.cc
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
#include <vire/cmsserver/uc_composition.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace cmsserver {
    
    uc_composition::uc_composition()
    {
      return;
    }

    uc_composition::~uc_composition()
    {
      return;
    }

    bool uc_composition::is_initialized() const
    {
      return _initialized_;
    }

    void uc_composition::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Compostion is already initialized and locked!");

      // std::vector<std::string> daughters;
      // if (config_.has_key("daughters")) {
      //        config_.fetch("daughters", daughters);
      // }

      // for (std::size_t i = 0; i < daughters.size(); i++) {
      //        const std::string & dname = daughters[i];       
      // }
      
      return;
    }

    void uc_composition::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Not initialized!");
      _daughters_.clear();
      return;
    }

    bool uc_composition::has_daughters() const
    {
      return _daughters_.size();
    }
    
    bool uc_composition::has_daughter(const std::string & name_) const
    {
      return _daughters_.count(name_);
    }

    void uc_composition::add_daughter(const std::string & name_,
                                      const use_case_info & uc_info_,
                                      const datatools::properties & traits_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked composition!");
      DT_THROW_IF(has_daughter(name_), std::logic_error,
                  "Daughter use case '" << name_ << "' already exists!");
      daughter_entry e;
      e.name = name_;
      e.uc_info = uc_info_;
      e.traits = traits_;
      _daughters_[name_] = e;
      return;
    }

    const uc_composition::daughter_entry &
    uc_composition::get_daughter(const std::string & name_) const
    {
      daughters_dict_type::const_iterator found = _daughters_.find(name_);
      DT_THROW_IF(found == _daughters_.end(), std::logic_error,
                  "Missing daughter use case '" << name_ << "'!");
      return found->second;
    }
                                       
    void uc_composition::remove_daughter(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Locked composition!");
      daughters_dict_type::const_iterator found = _daughters_.find(name_);
      DT_THROW_IF(found == _daughters_.end(), std::logic_error,
                  "Missing daughter use case '" << name_ << "'!");
      _daughters_.erase(name_);
      return;
    }

  } // namespace cmsserver

} // namespace vire
