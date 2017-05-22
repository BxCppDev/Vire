//! \file vire/server/mos_interface.h
//! \brief Interface service for the Vire server to the MOS server
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

#ifndef VIRE_SERVER_MOS_INTERFACE_H
#define VIRE_SERVER_MOS_INTERFACE_H

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <datatools/bit_mask.h>
#include <datatools/base_service.h>

namespace vire {

  namespace server {

    //! \brief Interface service for the Vire server to the MOS server
    class mos_interface : public ::datatools::base_service
    {
    public:

      //! Default constructor
      mos_interface();

      //! Destructor
      virtual ~mos_interface();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Check if the service is initialized
      virtual bool is_initialized() const;

      //! Initialize the service using a list of properties with access to a dictionary of services
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the service
      virtual int reset();

    protected:

      //! Set default attribute values
      void _set_defaults();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(mos_interface);

    };

  } // namespace server

} // namespace vire

// OCD support:
#include <datatools/ocd_macros.h>
// @arg vire::user::manager the name of the class with OCD support
DOCD_CLASS_DECLARATION(vire::server::mos_interface)

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::server::mos_interface);
#endif // Q_MOC_RUN

#endif // VIRE_SERVER_MOS_INTERFACE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
