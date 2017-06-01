//! \file    vire/user/group-serial.ipp
//! \brief   Vire group serialization support
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

#ifndef VIRE_USER_GROUP_SERIAL_IPP
#define VIRE_USER_GROUP_SERIAL_IPP

// Ourselves:
#include <vire/user/group.h>

// Third party:
// - Boost:
// #include <boost/serialization/set.hpp>
// - Bayeux/datatools:
#include <datatools/enriched_base.ipp>

namespace vire {

  namespace user {

    template<class Archive>
    void group::serialize(Archive & archive_,
                          const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("datatools__enriched_base",
                                                boost::serialization::base_object<datatools::enriched_base>(*this));
      archive_ & boost::serialization::make_nvp("gid", _gid_);
      // archive_ & boost::serialization::make_nvp("groups", _groups_);
      return;
    }

  } // end of namespace user

} // end of namespace vire

// #include <boost/serialization/version.hpp>
// BOOST_CLASS_VERSION(vire::user::group, 0)

#endif // VIRE_USER_GROUP_SERIAL_IPP

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
