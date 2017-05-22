//! \file  vire/device/connection_info.h
//! \brief Vire connection info between physical port and implemented
//         within physical links.
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

#ifndef VIRE_DEVICE_PORTS_CONNECTION_H
#define VIRE_DEVICE_PORTS_CONNECTION_H

namespace vire {

  namespace device {

    //! \brief Information about the connection between 2 ports at the device level
    class ports_connection {

    public:

      /// Default constructor
      ports_connection();

      /// Destructor
      ~ports_connection();

      /// Check connnection validity
      bool is_valid();

    private:

      const physical_port *  _start_port_;           //!< Handle to the start physical port
      port_interface_type    _start_port_interface_; //!< Interface type of the start port
      const physical_port *  _stop_port_;            //!< Handle to the stop physical port
      port_interface_type    _stop_port_interface_;  //!< Interface type of the stop port

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_PORTS_CONNECTION_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
