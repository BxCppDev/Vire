//! \file  vire/sandbox/sandbox.h
//! \brief Vire sandbox utilities
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>

#ifndef VIRE_SANDBOX_SANDBOX_H
#define VIRE_SANDBOX_SANDBOX_H

// Standard library:
#include <string>

// This project:
#include <vire/user/manager.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>
#include <vire/com/manager.h>

namespace vire {

  namespace sandbox {
 
    void init_users(vire::user::manager & users_,
                    const std::string & sandbox_ = "snemock",
                    const std::string & tag_ = "test1");

    void init_devices(vire::device::manager & devices_, 
                      const std::string & sandbox_ = "snemock",
                      const std::string & tag_ = "test1");
      
    void init_resources(vire::resource::manager & resources_,
                        const vire::device::manager & devices_,
                        const std::string & sandbox_ = "snemock",
                        const std::string & tag_ = "test1");

    void init_com(vire::com::manager & com_,
                  const vire::resource::manager & resources_,
                  const vire::cms::application::category_type appcat_,
                  const std::string & sandbox_ = "snemock",
                  const std::string & tag_ = "test1");
    
  } // namespace sandbox

} // namespace vire

#endif // VIRE_SANDBOX_SANDBOX_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
