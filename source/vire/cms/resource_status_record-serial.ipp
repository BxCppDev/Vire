//! \file  vire/cms/resource_status_record-serial.ipp
//! \brief Resource status record serialization
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMS_RESOURCE_STATUS_RECORD_SERIAL_IPP
#define VIRE_CMS_RESOURCE_STATUS_RECORD_SERIAL_IPP

// Ourselves:
#include <vire/cms/resource_status_record.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>

namespace vire {

  namespace cms {

    template<class Archive>
    void resource_status_record::serialize (Archive & archive_, const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("__base__",
                                                boost::serialization::base_object<vire::utility::base_payload>(*this));
      archive_ & boost::serialization::make_nvp("path",      _path_);
      archive_ & boost::serialization::make_nvp("timestamp", _timestamp_);
      archive_ & boost::serialization::make_nvp("missing",   _missing_);
      archive_ & boost::serialization::make_nvp("failed",    _failed_);
      archive_ & boost::serialization::make_nvp("pending",   _pending_);
      archive_ & boost::serialization::make_nvp("disabled" , _disabled_);
      return;
    }

  } // end of namespace cms

} // end of namespace vire

#endif // VIRE_CMS_RESOURCE_STATUS_RECORD_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
