//! \file  vire/cmsclient/client.h
//! \brief Vire CMS client
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSCLIENT_CLIENT_H
#define VIRE_CMSCLIENT_CLIENT_H

// Standard library:
#include <string>
#include <iostream>

// Third party:
// - Bayeux:
#include <bayeux/datatools/enriched_base.h>
#include <bayeux/datatools/multi_properties.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/service_manager.h>
#include <bayeux/datatools/logger.h>

// This project:
#include <vire/cmsclient/setup_infos.h>

namespace vire {

  namespace cmsclient {

    /// \brief Vire CMS client
    class client
      : public datatools::enriched_base
    {
    public:

      static std::string com_service_name();

      static std::string session_service_name();

      static std::string devices_service_name();

      static std::string resources_service_name();

      //! Default constructor
      client();

      //! Destructor
      virtual ~client();

      //! Check initialization status
      bool is_initialized() const;

      //! Initialize the client
      void initialize(const datatools::multi_properties &);

      //! Terminate the client
      void reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      bool has_setup_infos() const;

      void set_setup_infos(const setup_infos &);

      const setup_infos & get_setup_infos() const;

      setup_infos & grab_setup_infos();

      datatools::service_manager & grab_services();

      const datatools::service_manager & get_services() const;

    protected:

      void _init_main_(const datatools::properties &);

      void _reset_main_();

      void _start_system_services();

      void _stop_system_services();

      void _start_business_services();

      void _stop_business_services();

    private:

      void _at_init_();

      void _at_reset_();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
      datatools::multi_properties _mconfig_;     //!< Client configuration parameters
      setup_infos                 _setup_infos_; //!< Setup infos

      // Working:
      datatools::service_manager _services_;

    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_CLIENT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
