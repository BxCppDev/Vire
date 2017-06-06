//! \file  vire/cmslapp/connection_success-serial.ipp
//! \brief Connection success serialization
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

#ifndef VIRE_CMSLAPP_CONNECTION_SUCCESS_SERIAL_IPP
#define VIRE_CMSLAPP_CONNECTION_SUCCESS_SERIAL_IPP

// Ourselves:
#include <vire/cmslapp/connection_success.h>

// This project:
#include <vire/cms/resource_status_record.ipp>

namespace vire {

  namespace cmslapp {

    template<class Archive>
    void connection_success::serialize(Archive & archive_,
                                       const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("__base__",
                                                boost::serialization::base_object<vire::utility::base_payload>(*this));
      archive_ & boost::serialization::make_nvp("resource_snapshots", _resource_snapshots_);
      return;
    }

  } // end of namespace cmslapp

} // end of namespace vire

#endif // VIRE_CMSLAPP_CONNECTION_SUCCESS_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
