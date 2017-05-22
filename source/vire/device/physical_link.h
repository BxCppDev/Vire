//! \file  vire/device/physical_link.h
//! \brief Vire physical link
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

#ifndef VIRE_DEVICE_PHYSICAL_LINK_H
#define VIRE_DEVICE_PHYSICAL_LINK_H

// Standard library:
#include <string>
#include <iostream>

// // Third party:
// // - Bayeux/datatools:
// #include <datatools/enriched_base.h>

// This project:
#include <vire/device/device_utils.h>
#include <vire/device/base_physical.h>
#include <vire/device/port_utils.h>
#include <vire/device/device_link_info.h>

namespace vire {

  namespace device {

    // Forward class declaration
    class logical_device;
    class physical_port;

    //! \brief A physical link
    class physical_link : public base_physical
    {
    public:

      //! Default constructor
      physical_link();

      //! Destructor
      virtual ~physical_link();

      //! Reset
      void reset();

      //! Check if the physical link has a device link info
      bool has_link_info() const;

      //! Set the device link info
      void set_link_info(const device_link_info &);

      //! Return non mutable device link info
      const device_link_info & get_link_info() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      device_link_info _link_info_; //!< Informations about linked ends/ports

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_PHYSICAL_LINK_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
