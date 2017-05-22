//! \file  vire/resource/resource_status.h
//! \brief Device/resource (dynamic) status flag definition
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMS_STATUS_H
#define VIRE_CMS_STATUS_H

// Standard Library:
#include <string>

// Third party
// - Bayeux/datatools
#include <datatools/bit_mask.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

namespace vire {

  //! Nested namespace for Vire CMS
  namespace cms {

    //! \brief Device/resource dynamic status utilities
    struct status {

      /// \brief Device/resource dynamic status bits
      enum flag {
        none     = 0,
        failed   = datatools::bit_mask::bit00,
        pending  = datatools::bit_mask::bit01,
        disabled = datatools::bit_mask::bit02,
        missing  = datatools::bit_mask::bit03
      };

      //! Return the status flag associated to a label
      static flag get_flag(const std::string & label_);

      //! Return the label associated to a status flag
      static std::string get_flag_label(flag);

    };

  } // namespace cms

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::cms::status' enum:
DR_TYPE_INIT(::vire::cms::status);
#endif // Q_MOC_RUN

#endif // VIRE_CMS_STATUS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
