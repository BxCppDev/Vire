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

// Third party:
// - Bayeux:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/multi_properties.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/service_manager.h>
#include <bayeux/datatools/logger.h>

// This project:
#include <vire/cmsclient/server_infos.h>
#include <vire/utility/instance_identifier.h>

namespace vire {

  namespace cmsclient {

    /// \brief Vire CMS client
    class client
      : public datatools::i_tree_dumpable
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

      //! Return the logging threshold
      datatools::logger::priority get_logging() const;

      //! Set the logging threshold
      void set_logging(const datatools::logger::priority);

      //! Check if setup ID is set
      bool has_setup_id() const;

      //! Return the setup ID
      const ::vire::utility::instance_identifier & get_setup_id() const;

      //! Set the setup ID
      void set_setup_id(const ::vire::utility::instance_identifier &);

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

      bool has_server_infos() const;

      void set_server_infos(const server_infos &);

      const server_infos & get_server_infos() const;

      server_infos & grab_server_infos();

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
      bool                        _initialized_ = false;
      datatools::logger::priority _logging_;

      // Configuration:
      ::vire::utility::instance_identifier _setup_id_;     //!< Setup ID
      datatools::multi_properties          _mconfig_;      //!< Client configuration parameters
      server_infos                         _server_infos_; //!< Server infos

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
