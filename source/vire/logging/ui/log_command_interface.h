//! \file  vire/logging/ui/log_command_interface.h
//! \brief Command interface for simple logging service
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_LOGGING_UI_LOG_COMMAND_INTERFACE_H
#define VIRE_LOGGING_UI_LOG_COMMAND_INTERFACE_H

// Third Party:
// - Bayeux/datatools:
#include <datatools/ui/target_command_interface.h>

// This project:
#include <vire/logging/simple_logging_service.h>

namespace vire {

  namespace logging {

    namespace ui {

      /// \brief Simple logging service command interface
      class log_command_interface : public datatools::ui::target_command_interface<simple_logging_service>
      {
      public:

        //! Default constructor
        log_command_interface();

        //! Constructor
        log_command_interface(simple_logging_service & target_,
                              const std::string & name_,
                              const std::string & description_  = "",
                              const datatools::version_id & vid_ = datatools::version_id::invalid());

        //! Destructor
        virtual ~log_command_interface();

        //! Check initialization status
        virtual bool is_initialized() const;

        //! Initialization
        virtual void initialize(const datatools::properties & config_,
                                const datatools::service_manager & /* services_ */);

        //! Reset
        virtual void reset();

      private:

        bool _initialized_ = false; ///< Initialization flag

        DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(datatools::ui::base_command_interface,
                                                             vire::logging::ui::log_command_interface)

      };

    } // end of namespace ui

  } // end of namespace logging

} // end of namespace vire

#endif // VIRE_LOGGING_UI_LOG_COMMAND_INTERFACE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
