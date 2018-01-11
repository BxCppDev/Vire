//! \file  vire/inspector/device_command_interface.h
//! \brief Vire physical device command interface
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_INSPECTOR_DEVICE_COMMAND_INTERFACE_H
#define VIRE_INSPECTOR_DEVICE_COMMAND_INTERFACE_H

// Third party:
// - Bayeux/datatools:
#include <datatools/ui/target_command_interface.h>

// This project:
#include <vire/inspector/device_desc.h>

namespace vire {

  namespace inspector {

    //! \brief Device command interface
    class device_command_interface
      : public datatools::ui::const_target_command_interface<device_desc>
    {
    public:

      //! Default constructor
      device_command_interface(const device_desc & target_,
                               const std::string & name_ = "",
                               const std::string & description_ = "",
                               const datatools::version_id & vid_ = datatools::version_id::invalid());

      //! Destructor
      virtual ~device_command_interface();

      //! Check initialization status
      virtual bool is_initialized() const;

      //! Initialization
      virtual void initialize(const datatools::properties & config_,
                              const datatools::service_manager & /* services_ */);

      //! Reset
      virtual void reset();

    };

  } // end of namespace inspector

} // end of namespace vire

#endif // VIRE_DEVICE_INSPECTOR_COMMAND_INTERFACE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
