//! \file vire/mos/export_utils.h
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

#ifndef VIRE_MOS_EXPORT_UTILS_H
#define VIRE_MOS_EXPORT_UTILS_H

// Standard library:
#include <string>
#include <iostream>

// Third Party:
// - Boost++:
#include <boost/filesystem.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>
#include <datatools/properties.h>

namespace vire {

  namespace mos {

    //! \brief Store informations about a device or port model
    struct device_entry_type : public datatools::i_tree_dumpable
    {
      std::string           name;       //!< Name of the device or port model
      std::string           model_type; //!< Type of the model (port/device)
      std::string           class_id;   //!< Class identifier
      datatools::properties config;     //!< Configuration parameters

      //! Default constructor
      device_entry_type();

      //! Reset
      void reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    };

  } // namespace mos

} // namespace vire

#endif // VIRE_MOS_EXPORT_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
