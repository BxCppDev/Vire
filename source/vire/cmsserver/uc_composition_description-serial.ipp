//! \file    vire/cmsserver/uc_composition_description-serial.ipp
//! \brief   Serialization support for use case composition
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

#ifndef VIRE_CMSSERVER_UC_COMPOSITION_DESCRIPTION_SERIAL_IPP
#define VIRE_CMSSERVER_UC_COMPOSITION_DESCRIPTION_SERIAL_IPP

// Ourselves:
#include <vire/cmsserver/uc_composition_description.h>

// Third party:
// - Boost:
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/properties.ipp>

// This project:
#include <vire/cmsserver/uc_description-serial.ipp>

namespace vire {

  namespace cmsserver {

    template<class Archive>
    void uc_composition_description::daughter_entry::serialize(Archive & archive_,
                                                               const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("model_id", model_id);
      archive_ & boost::serialization::make_nvp("description", description);
      archive_ & boost::serialization::make_nvp("serial_rank", serial_rank);
      return;
    }

    template<class Archive>
    void uc_composition_description::serialize(Archive & archive_,
                                               const unsigned int /* version_*/)
    {
      archive_ & DATATOOLS_SERIALIZATION_I_SERIALIZABLE_BASE_OBJECT_NVP;
      archive_ & boost::serialization::make_nvp("scheduling", _scheduling_);
      if (is_scheduling_repeat()) {
        archive_ & boost::serialization::make_nvp("number_of_repetitions", _number_of_repetitions_);
      }
      archive_ & boost::serialization::make_nvp("daughters", _daughters_);
      archive_ & boost::serialization::make_nvp("locked", _locked_);
      return;
    }

  } // end of namespace cmsserver

} // end of namespace vire

// #include <boost/serialization/version.hpp>
// BOOST_CLASS_VERSION(vire::cmsserver::uc_composition_description, 0)

#endif // VIRE_CMSSERVER_UC_COMPOSITION_DESCRIPTION_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
