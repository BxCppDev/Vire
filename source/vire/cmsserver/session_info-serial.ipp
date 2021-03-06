//! \file    vire/cmsserver/session_info-serial.ipp
//! \brief   Vire cmsserver session info serialization support
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

#ifndef VIRE_CMSSERVER_SESSION_INFO_SERIAL_IPP
#define VIRE_CMSSERVER_SESSION_INFO_SERIAL_IPP

// Ourselves:
#include <vire/cmsserver/session_info.h>

// Third party:
// - Boost:
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/optional.hpp>

namespace vire {

  namespace cmsserver {

    template<class Archive>
    void session_info::serialize(Archive & archive_,
                         const unsigned int /* version_*/)
    {
      archive_ & DATATOOLS_SERIALIZATION_I_SERIALIZABLE_BASE_OBJECT_NVP;

      archive_ & boost::serialization::make_nvp("key", _key_);
      archive_ & boost::serialization::make_nvp("description", _description_);
      archive_ & boost::serialization::make_nvp("role_identifier", _role_identifier_);
      archive_ & boost::serialization::make_nvp("when", _when_);
      archive_ & boost::serialization::make_nvp("special_functional_cardinalities",
                                                _special_functional_cardinalities_);
      archive_ & boost::serialization::make_nvp("special_distributable_cardinalities",
                                                _special_distributable_cardinalities_);
      archive_ & boost::serialization::make_nvp("use_case_type_id", _use_case_type_id_);
      archive_ & boost::serialization::make_nvp("use_case_config_path", _use_case_config_path_);
      archive_ & boost::serialization::make_nvp("use_case_config", _use_case_config_);
      return;
    }

  } // end of namespace cmsserver

} // end of namespace vire

// #include <boost/serialization/version.hpp>
// BOOST_CLASS_VERSION(vire::cmsserver::session_info, 0)

#endif // VIRE_CMSSERVER_SESSION_INFO_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
