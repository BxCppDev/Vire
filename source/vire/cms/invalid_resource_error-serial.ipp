//! \file  vire/cms/invalid_resource_error-serial.ipp
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

#ifndef VIRE_CMS_INVALID_RESOURCE_ERROR_SERIAL_IPP
#define VIRE_CMS_INVALID_RESOURCE_ERROR_SERIAL_IPP

// Ourselves:
#include <vire/cms/invalid_resource_error.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>

namespace vire {

  namespace cms {

    template<class Archive>
    void invalid_resource_error::serialize(Archive & archive_, const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("__base__",
                                                boost::serialization::base_object<vire::utility::base_error>(*this));
      if (is_invalid_resource_path()) {
        archive_ & boost::serialization::make_nvp("invalid_resource_path", _invalid_resource_path_);
      }
      if (is_invalid_resource_id()) {
        archive_ & boost::serialization::make_nvp("invalid_resource_id",   _invalid_resource_id_);
      }
      return;
    }

  } // end of namespace cms

} // end of namespace vire

#endif // VIRE_CMS_INVALID_RESOURCE_ERROR_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
