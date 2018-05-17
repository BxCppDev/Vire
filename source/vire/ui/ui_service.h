//! \file  vire/ui/ui_service.h
//! \brief User Interface Service
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

#ifndef VIRE_UI_UI_SERVICE_H
#define VIRE_UI_UI_SERVICE_H

// This project
#include <datatools/properties.h>
#include <datatools/base_service.h>

namespace vire {

  namespace ui {

    //! \brief User Interface Context Service
    class ui_service
      : public ::datatools::base_service
    {
    public:

      //! Constructor
      ui_service();

      //! Destructor
      virtual ~ui_service();

      //! Check initialization status
      virtual bool is_initialized() const;

      //! Initialize the service from a list of properties
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & services_);

      //! Reset
      virtual int reset();

      //! Set the interactive flag
      void set_interactive(bool);

      //! Check interactive state
      bool is_interactive() const;

      //! Check batch state
      bool is_batch() const;

      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const;

    private:

      void _init_(const datatools::properties & config_);

      void _reset_();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
      bool _interactive_ = true; //!< Interactive flag

      // Working data:

      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(ui_service);

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_UI_SERVICE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
