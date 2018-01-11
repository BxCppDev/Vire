//! \file  vire/device/mapping_info.h
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

#ifndef VIRE_DEVICE_MAPPING_INFO_H
#define VIRE_DEVICE_MAPPING_INFO_H

// Standard library:
#include <set>

// Third Party:
// - Bayeux/datatools:
#include <datatools/i_predicate.h>
// - Bayeux/geomtools:
#include <geomtools/geom_id.h>

// This project:
#include <vire/device/logical_device.h>
#include <vire/device/instance_info.h>

namespace vire {

  namespace device {

    // Forward class declarations:
    class logical_device;
    class logical_port;
    class mapping;

    //! \brief Information about a device
    class mapping_info
      :  public instance_info
    {
    public:

      //! Default constructor
      mapping_info();

      //! Destructor
      virtual ~mapping_info();

      //! Check if there is a mapping identifier (MID)
      bool has_mapping_id() const;

      //! Set the electronic identifier (MID)
      void set_mapping_id(const geomtools::geom_id & mid_);

      //! Get the electronic identifier (MID)
      const geomtools::geom_id & get_mapping_id() const;

      //! Check if there is a parent device MID
      bool has_parent_device_id() const;

      //! Set the MID of the parent device
      void set_parent_device_id(const geomtools::geom_id & did_);

      //! Get the MID of the parent device
      const geomtools::geom_id & get_parent_device_id() const;

      //! Check if there is a daughter device MIDs
      bool has_daughter_device_ids() const;

      //! Get the set of MIDS of the daughter devices
      const std::set<geomtools::geom_id> & get_daughter_device_ids() const;

      //! Return validity status
      bool is_valid() const;

      //! Reset
      void reset();

      //! Streaming
      friend std::ostream & operator<<(std::ostream &, const mapping_info &);

    private:

      void _add_daughter_device_id_(const geomtools::geom_id & mid_);

    private:

      geomtools::geom_id _mapping_id_; //!< Unique identifier (MID) of the mapped object
      geomtools::geom_id _parent_device_id_; //!< Unique identifier (MID) of the mapped object's parent device
      std::set<geomtools::geom_id> _daughter_device_ids_; //!< Set of daughter device identifiers

      friend class mapping;

    };

    struct has_device_type_predicate : public datatools::i_predicate<mapping_info>
    {
    public:
      has_device_type_predicate(uint32_t device_type_) : _device_type_(device_type_) {
        return;
      }
      bool operator()(const mapping_info & dinfo_) const {
        return (dinfo_.is_device() && dinfo_.get_mapping_id().get_type() == _device_type_);
      }
    private:
      uint32_t _device_type_;
    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_MAPPING_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
