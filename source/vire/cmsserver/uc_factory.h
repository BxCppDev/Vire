//! \file  vire/cmsserver/uc_factory.h
//! \brief Vire CMS use case factory utilities
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_UC_FACTORY_H
#define VIRE_CMSSERVER_UC_FACTORY_H


// Standard Library:
#include <memory>

// This project:
#include <vire/cmsserver/base_use_case.h>
#include <vire/cmsserver/use_case_info.h>
#include <vire/cmsserver/session_info.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>
#include <vire/user/manager.h>
#include <vire/cmsserver/agenda.h>
#include <vire/cmsserver/session.h>

namespace vire {

  namespace cmsserver {

    /// \brief Use case factory
    class uc_factory
    {

    public:

      uc_factory();
      
      /// Create a use case in "dry-run" mode
      std::shared_ptr<base_use_case> create_dry(const use_case_info & uc_info_);

      /// Create a use case in "run" mode
      std::shared_ptr<base_use_case> create_run(const session_info & sinfo_,
                                                const session * mother_ = nullptr);

    private:

      std::shared_ptr<base_use_case> _base_create_(const std::string & uc_type_id_);
      
    private:

      const vire::device::manager *   _devices_   = nullptr; ///< Current device manager
      const vire::resource::manager * _resources_ = nullptr; ///< Current resource manager
      const vire::user::manager *     _users_     = nullptr; ///< Current user manager
      const vire::cmsserver::agenda * _agenda_    = nullptr; ///< Current agenda
      
    };
    
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_FACTORY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
