//! \file    vire/cmsserver/uc_model_description-serial.ipp
//! \brief   Serialization support for use case description
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

#ifndef VIRE_CMSSERVER_UC_MODEL_DESCRIPTION_SERIAL_IPP
#define VIRE_CMSSERVER_UC_MODEL_DESCRIPTION_SERIAL_IPP

// Ourselves:
#include <vire/cmsserver/uc_model_description.h>

// Third party:
// - Boost:
#include <boost/serialization/string.hpp>
#include <boost/serialization/optional.hpp>

// This project:
#include <vire/cmsserver/uc_composition_description-serial.ipp>

namespace vire {

  namespace cmsserver {

    template<class Archive>
    void uc_model_description::serialize(Archive & archive_,
                                         const unsigned int /* version_*/)
    {
      archive_ & DATATOOLS_SERIALIZATION_I_SERIALIZABLE_BASE_OBJECT_NVP;
      archive_ & boost::serialization::make_nvp("name", _name_);
      archive_ & boost::serialization::make_nvp("use_case_type_id", _use_case_type_id_);
      archive_ & boost::serialization::make_nvp("description", _description_);
      archive_ & boost::serialization::make_nvp("use_case_config_path", _use_case_config_path_);
      archive_ & boost::serialization::make_nvp("use_case_config", _use_case_config_);
      archive_ & boost::serialization::make_nvp("composition_description", _composition_description_);
      return;
    }

  } // end of namespace cmsserver

} // end of namespace vire

// #include <boost/serialization/version.hpp>
// BOOST_CLASS_VERSION(vire::cmsserver::uc_model_description, 0)

#endif // VIRE_CMSSERVER_UC_MODEL_DESCRIPTION_SERIAL_IPP

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
