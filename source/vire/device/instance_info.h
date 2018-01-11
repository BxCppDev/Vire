//! \file  vire/device/instance_info.h
//! \brief Vire device/port instance info
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_DEVICE_INSTANCE_INFO_H
#define VIRE_DEVICE_INSTANCE_INFO_H

// Standard library:
#include <string>
#include <memory>

// Third Party:
// - Bayeux/datatools:
#include <datatools/i_predicate.h>
#include <datatools/properties.h>

// This project:

namespace vire {

  namespace device {

    // Forward class declarations:
    class logical_device;
    class logical_port;

    //! \brief Basic information about a device/port instance in a hierarchy of devices
    class instance_info
    {
    public:

      //! Default constructor
      instance_info();

      //! Destructor
      virtual ~instance_info();

      //! Check if there is a path
      bool has_path() const;

      //! Get path
      const std::string & get_path() const;

      //! Set the path
      void set_path(const std::string & path_);

      //! Check if the instantiated object is a device
      bool is_device() const;

      //! Check if the instantiated object is a port
      bool is_port() const;

      //! Check if there is a logical device or port
      bool has_logical() const;

      //! Check if there is a logical device
      bool has_logical_device() const;

      //! Check if there is a logical port
      bool has_logical_port() const;

      //! Set the logical device
      void set_logical_device(const logical_device &);

      //! Return a const reference to the logical device
      const logical_device & get_logical_device() const;

      //! Set the logical port
      void set_logical_port(const logical_port &);

      //! Return a const reference to the logical port
      const logical_port & get_logical_port() const;

      //! Check if there is a serial number
      bool has_serial_number() const;

      //! Set the serial number
      void set_serial_number(const std::string &);

      //! Return the serial number
      const std::string & get_serial_number() const;

      //! Check if there are some metadata
      bool has_metadata() const;

      //! Set the metadata
      void set_metadata(const datatools::properties &);

      //! Return the metadata
      const datatools::properties & get_metadata() const;

      //! Return the metadata
      datatools::properties & grab_metadata();

      //! Return validity status
      bool is_valid() const;

      //! Reset
      void reset();

      //! Streaming
      friend std::ostream & operator<<(std::ostream &, const instance_info &);

    private:

      std::string            _path_;           //!< Unique path of the mapped object
      const logical_device * _logical_device_; //!< Handle to the logical device
      const logical_port *   _logical_port_;   //!< Handle to the logical port
      std::string            _serial_number_;  //!< Manufacturer's unique identifier of the physical device
      datatools::properties  _metadata_;       //!< Metadata

    };

    typedef std::shared_ptr<instance_info> instance_info_ptr;

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_INSTANCE_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
