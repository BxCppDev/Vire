//! \file    vire/cmsserver/session_reservation-serial.ipp
//! \brief   Vire cmsserver session reservation serialization support
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

#ifndef VIRE_CMSSERVER_SESSION_RESERVATION_SERIAL_IPP
#define VIRE_CMSSERVER_SESSION_RESERVATION_SERIAL_IPP

// Ourselves:
#include <vire/cmsserver/session_reservation.h>

// Third party:
// - Boost:
#include <boost/serialization/string.hpp>

namespace vire {

  namespace cmsserver {

    template<class Archive>
    void session_reservation::serialize(Archive & archive_,
                                        const unsigned int /* version_*/)
    {
      archive_ & DATATOOLS_SERIALIZATION_I_SERIALIZABLE_BASE_OBJECT_NVP;
      archive_ & boost::serialization::make_nvp("id", _id_);
      archive_ & boost::serialization::make_nvp("booked_by", _booked_by_);
      archive_ & boost::serialization::make_nvp("last_update", _last_update_);
      archive_ & boost::serialization::make_nvp("resource_scope", _resource_scope_);
      archive_ & boost::serialization::make_nvp("when", _when_);
      archive_ & boost::serialization::make_nvp("use_case_info", _use_case_info_);
      archive_ & boost::serialization::make_nvp("start_macro", _start_macro_);
      archive_ & boost::serialization::make_nvp("stop_macro",  _stop_macro_);
      return;
    }

  } // end of namespace cmsserver

} // end of namespace vire

// #include <boost/serialization/version.hpp>
// BOOST_CLASS_VERSION(vire::cmsserver::session_reservation, 0)

#endif // VIRE_CMSSERVER_SESSION_RESERVATION_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
