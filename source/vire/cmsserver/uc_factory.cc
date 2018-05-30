//! \file vire/cmsserver/uc_factory.cc
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
#include <vire/cmsserver/uc_factory.h>
  
namespace vire {

  namespace cmsserver {

    std::shared_ptr<base_use_case>
    uc_factory::_base_create_(const std::string & uc_type_id_)
    {
      std::shared_ptr<base_use_case> uc_ptr;
      const base_use_case::factory_register_type & the_factory_register
        = DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(vire::cmsserver::base_use_case);
      DT_THROW_IF(!the_factory_register.has(uc_type_id_),
                  std::logic_error,
                  "Use case factory has no type with identifier '"
                  << uc_type_id_ << "'!");
      const base_use_case::factory_register_type::factory_type & the_factory
        = the_factory_register.get(uc_type_id_);
      uc_ptr.reset(the_factory());
      return uc_ptr;
    }

    std::shared_ptr<base_use_case>
    uc_factory::create_dry(const use_case_info & uc_info_)
    {
      DT_THROW_IF(!uc_info_.is_valid(),
                  std::logic_error,
                  "Invalid use case info record!");
      std::shared_ptr<base_use_case> uc_ptr
        = _base_create_(uc_info_.get_use_case_type_id());
      uc_ptr->_run_mode_ = false;
      uc_ptr->_run_session_ = nullptr;
      datatools::properties uc_config;
      const datatools::properties * uc_config_p = nullptr;
      if (uc_info_.has_use_case_config()) {
        uc_config_p = &uc_info_.get_use_case_config();
      } else {
        if (uc_info_.has_use_case_config_path()) {
          std::string path = uc_info_.get_use_case_config_path();
          datatools::fetch_path_with_env(path);
          datatools::properties::read_config(path, uc_config);
          uc_config_p = &uc_config;
        }
      }
      uc_ptr->initialize(*uc_config_p);
      return uc_ptr;
    }

    std::shared_ptr<base_use_case>
    uc_factory::create_run(const session_info & sinfo_,
                           const session * run_session_)
    {
      std::shared_ptr<base_use_case> uc_ptr;
      /*
      uc_ptr->_run_mode_ = true;
      uc_ptr->_run_session_ = run_session_;
      */
      return uc_ptr;
    }
    
  } // namespace cmsserver

} // namespace vire
