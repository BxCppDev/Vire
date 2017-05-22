//! \file  vire/device/physical_device.h
//! \brief Vire physical device
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

#ifndef VIRE_DEVICE_PHYSICAL_DEVICE_H
#define VIRE_DEVICE_PHYSICAL_DEVICE_H

// Standard library:
#include <string>
#include <iostream>

// This project:
#include <vire/device/device_utils.h>
#include <vire/device/base_physical.h>

namespace vire {

  namespace device {

    // Forward class declaration
    class logical_device;

    //! \brief The physical device
    class physical_device : public base_physical
    {
    public:

      //! Default constructor
      physical_device();

      //! Destructor
      virtual ~physical_device();

      //! Reset
      void reset();

      //! Check if the physical device has a logical device
      bool has_logical() const;

      //! Set the logical device
      void set_logical(const logical_device &);

      //! Return non mutable logical device
      const logical_device & get_logical() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      const logical_device * _logical_; //!< Handle to the logical device

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_PHYSICAL_DEVICE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
