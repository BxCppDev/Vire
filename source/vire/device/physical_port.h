//! \file  vire/device/physical_port.h
//! \brief Vire physical port
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

#ifndef VIRE_DEVICE_PHYSICAL_PORT_H
#define VIRE_DEVICE_PHYSICAL_PORT_H

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

namespace vire {

  namespace device {

    // Forward class declaration
    class logical_port;
    class logical_device;
    class physical_link;

    //! \brief A physical port
    //!
    //! A port represents a I/O access to the device it belongs to. It
    //! can be connected on both sides of the device's border with links.
    //! @code
    //!              +--------------------------+
    //!              |           Logical device |
    //!              |                          |
    //!              | Physical port #0         |
    //!   (outside) o=o (inside)                |
    //!              |                          |
    //!              |                          |
    //!              | Physical port #1         |
    //!   (outside) o=o (inside)                |
    //!              |                          |
    //!              |                          |
    //!              | Physical port #2         |
    //!   (outside) o=o (inside)                |
    //!              |                          |
    //!              |                          |
    //!              +--------------------------+
    //! @endcode
    class physical_port : public base_physical
    {
    public:

      //! Default constructor
      physical_port();

      //! Destructor
      virtual ~physical_port();

      //! Reset
      void reset();

      //! Check if the physical port has a logical port
      bool has_logical() const;

      //! Set the logical port
      void set_logical(const logical_port &);

      //! Return non mutable logical device
      const logical_port & get_logical() const;

      //! Check if the physical port has an inside physical link
      bool has_inside_link() const;

      //! Check if the physical port has an outside physical link
      bool has_outside_link() const;

      //! Check if the physical port has an physical link with given interface
      bool has_link(port_interface_type) const;

      //! Set the inside physical link
      void set_inside_link(const physical_link &);

      //! Reset the inside physical link
      void reset_inside_link();

      //! Set the outside physical link
      void set_outside_link(const physical_link &);

      //! Reset the outside physical link
      void reset_outside_link();

      //! Set a physical link
      void set_link(const physical_link &, port_interface_type);

      //! Reset a physical link
      void reset_link(port_interface_type);

      //! Return non mutable inside link
      const physical_link & get_inside_link() const;

      //! Return non mutable outside link
      const physical_link & get_outside_link() const;

      //! Return non mutable link
      const physical_link & get_link(port_interface_type) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      const logical_port  * _logical_;      //!< Handle to the logical port
      const physical_link * _inside_link_;  //!< Handle to the inside link
      const physical_link * _outside_link_; //!< Handle to the outside link

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_PHYSICAL_PORT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
