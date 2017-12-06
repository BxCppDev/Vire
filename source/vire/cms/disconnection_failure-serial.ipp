//! \file  vire/cms/disconnection_failure-serial.ipp
//! \brief Disconnection failure serialization
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

#ifndef VIRE_CMS_DISCONNECTION_FAILURE_SERIAL_IPP
#define VIRE_CMS_DISCONNECTION_FAILURE_SERIAL_IPP

// Ourselves:
#include <vire/cms/disconnection_failure.h>

// This project:
#include <vire/utility/base_payload-serial.ipp>
#include <vire/utility/model_identifier-serial.ipp>
#include <vire/utility/invalid_context_error-serial.ipp>
#include <vire/utility/invalid_setup_id_error-serial.ipp>
#include <vire/cms/unknown_resources_error-serial.ipp>

namespace vire {

  namespace cms {

    template<class Archive>
    void disconnection_failure::serialize(Archive & archive_,
                                          const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("__base__",
                                                boost::serialization::base_object<vire::utility::base_payload>(*this));

      archive_ & boost::serialization::make_nvp("error_type_id", _error_type_id_);

      if (_error_type_id_.match("vire::utility::invalid_setup_id_error")) {
        archive_ & boost::serialization::make_nvp("error", boost::get<vire::utility::invalid_setup_id_error>(_error_));
      }

      if (_error_type_id_.match("vire::utility::invalid_context_error")) {
        archive_ & boost::serialization::make_nvp("error", boost::get<vire::utility::invalid_context_error>(_error_));
      }

      return;
    }

  } // end of namespace cms

} // end of namespace vire

#endif // VIRE_CMS_DISCONNECTION_FAILURE_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
