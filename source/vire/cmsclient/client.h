//! \file  vire/cmsclient/client.h
//! \brief Vire CMS client
//
// Copyright (c) 2017-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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

      static std::string log_service_name();

      static std::string com_service_name();

      static std::string session_service_name();

      static std::string devices_service_name();

      static std::string resources_service_name();

      enum ui_mode_type {
        UI_UNDEF = 0, //!< Undefined user interface
        UI_BATCH = 1, //!< Not interactive mode
        UI_TUI   = 2, //!< Interactive mode with text user interface
        UI_GUI   = 3  //!< Interactive mode with graphical user interface
      };

      //! Default constructor
      client();

      //! Destructor
      virtual ~client();

      //! Check if UI mode is set
      bool has_ui_mode() const;

      //! Set the UI mode
      void set_ui_mode(const ui_mode_type);

      //! Return the UI mode
      ui_mode_type get_ui_mode() const;

      //! Check interactive UI mode
      bool is_interactive() const;

      //! Check non-interactive UI mode
      bool is_batch() const;

      //! Check initialization status
      bool is_initialized() const;

      //! Initialize the client
      void initialize(const datatools::multi_properties &);

      //! Terminate the client
      void reset();

      //! Check is client is in the setup
      bool in_setup() const;

      //! Entering the setup
      void setup_enter();

      //! Leave the setup
      void setup_quit();

      bool has_setup_infos() const;

      void set_setup_infos(const setup_infos &);

      const setup_infos & get_setup_infos() const;

      setup_infos & grab_setup_infos();

      datatools::service_manager & grab_services();

      const datatools::service_manager & get_services() const;

      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const;

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

      void _init_com_();

      void _terminate_com_();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag
      bool _in_setup_ = false; //!< In-setup flag

      // Configuration:
      datatools::multi_properties _mconfig_; //!< Client configuration parameters
      ui_mode_type _ui_mode_ = UI_UNDEF;     //!< User interface mode

      // Working:
      setup_infos                _setup_infos_; //!< Setup infos
      datatools::service_manager _services_;

      struct pimpl_type;
      std::unique_ptr<pimpl_type> _pimpl_;
      
    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_CLIENT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
