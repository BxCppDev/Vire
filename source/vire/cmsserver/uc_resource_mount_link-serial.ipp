//! \file  vire/cmsserver/uc_resource_mount_link-serial.ipp
//! \brief Resource mounting link serialization
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

#ifndef VIRE_CMSSERVER_UC_RESOURCE_MOUNT_LINK_SERIAL_IPP
#define VIRE_CMSSERVER_UC_RESOURCE_MOUNT_LINK_SERIAL_IPP

// Ourselves:
#include <vire/cmsserver/uc_resource_mount_link.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

// This project:
#include <vire/utility/path-serial.ipp>
#include <vire/cmsserver/uc_utils-serial.ipp>

namespace vire {

  namespace cmsserver {

    template<class Archive>
    void uc_resource_mount_link::serialize(Archive & archive_, const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("from_id", _from_id_);
      archive_ & boost::serialization::make_nvp("to_key",  _to_key_);
      archive_ & boost::serialization::make_nvp("relpath", _relpath_);
      return;
    }

  } // end of namespace cmsserver

} // end of namespace vire

#endif // VIRE_CMSSERVER_UC_RESOURCE_MOUNT_LINK_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
