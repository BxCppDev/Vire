//! \file  vire/device/device_info.h
//! \brief Vire device info
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

#ifndef VIRE_DEVICE_DEVICE_INFO_H
#define VIRE_DEVICE_DEVICE_INFO_H

// Third Party:
// - Bayeux/datatools:
#include <datatools/i_predicate.h>
// - Bayeux/geomtools:
#include <geomtools/geom_id.h>

// This project:
#include <vire/device/logical_device.h>
// #include <vire/device/i_instance.h>

namespace vire {

  namespace device {

    //! \brief Information about a device
    class device_info
    {
    public:

      //! Default constructor
      device_info();

      //! Destructor
      virtual ~device_info();

      //! Check if there is an device identifier (DID)
      bool has_device_id() const;

      //! Set the electronic identifier (DID)
      void set_device_id(const geomtools::geom_id & did_);

      //! Get the electronic identifier (DID)
      const geomtools::geom_id & get_device_id() const;

      //! Check if there is a parent DID
      bool has_parent_device_id() const;

      //! Set the DID of the parent device
      void set_parent_device_id(const geomtools::geom_id & did_);

      //! Get the DID of the parent device
      const geomtools::geom_id & get_parent_device_id() const;

      //! Check if there is a logical device
      bool has_logical() const;

      //! Set the logical device
      void set_logical(const logical_device &);

      //! Return a const reference to the logical device
      const logical_device & get_logical() const;

      //! Check if there is a serial number
      bool has_serial_number() const;

      //! Set the serial number
      void set_serial_number(const std::string &);

      //! Return the serial number
      const std::string & get_serial_number() const;

      //! Return validity status
      bool is_valid() const;

      //! Reset
      void reset();

      //! Streaming
      friend std::ostream & operator<<(std::ostream &, const device_info &);

    private:

      geomtools::geom_id _device_id_; //!< Electronic identifier (DID) of the device
      geomtools::geom_id _parent_device_id_; //!< Electronic identifier (DID) of the parent device
      const logical_device * _logical_; //!< Electronic device model
      std::string _serial_number_; //!< Manufacturer's unique identifier of the physical device
      //datatools::properties  _auxiliaries_; //!< Auxiliary properties

    };

    struct has_device_type_predicate : public datatools::i_predicate<device_info>
    {
    public:
      has_device_type_predicate(uint32_t device_type_) : _device_type_(device_type_) {
        return;
      }
      bool operator()(const device_info & dinfo_) const {
        return (dinfo_.get_device_id().get_type() == _device_type_);
      }
    private:
      uint32_t _device_type_;
    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_DEVICE_INFO_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
