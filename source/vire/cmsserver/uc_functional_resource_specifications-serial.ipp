//! \file  vire/cmsserver/uc_functional_resource_specifications-serial.ipp
//! \brief Use case functional resource specifications serialization
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

#ifndef VIRE_CMSSERVER_UC_FUNCTIONAL_RESOURCE_SPECIFICATIONS_SERIAL_IPP
#define VIRE_CMSSERVER_UC_FUNCTIONAL_RESOURCE_SPECIFICATIONS_SERIAL_IPP

// Ourselves:
#include <vire/cmsserver/uc_functional_resource_specifications.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>

namespace vire {

  namespace cmsserver {

    template<class Archive>
    void uc_functional_resource_description::serialize(Archive & archive_,
                                                       const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("absolute_path", _absolute_path_);
      archive_ & boost::serialization::make_nvp("mount_link",    _mount_link_);
      return;
    }

    template<class Archive>
    void uc_functional_resource_specifications::serialize(Archive & archive_,
                                                          const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("locked", _locked_);
      archive_ & boost::serialization::make_nvp("specs",  _specs_);
      return;
    }

  } // end of namespace cmsserver

} // end of namespace vire

#endif // VIRE_CMSSERVER_UC_FUNCTIONAL_RESOURCE_SPECIFICATIONS_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
