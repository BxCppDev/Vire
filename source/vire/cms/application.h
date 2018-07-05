//! \file  vire/cms/application.h
//! \brief Vire CMS application utilities
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

#ifndef VIRE_CMS_APPLICATION_H
#define VIRE_CMS_APPLICATION_H

// Standard library:
#include <string>

namespace vire {

  namespace cms {

    struct application {

      /// \brief Application category
      enum category_type {
        CATEGORY_UNDEF         = 0,
        CATEGORY_SERVER        = 1,
        CATEGORY_SUBCONTRACTOR = 2,
        CATEGORY_CLIENT        = 3
      };

      static std::string to_string(const category_type app_cat_);
      
      static bool from_string(const std::string & label_, category_type & app_cat_);
     
    };
    
  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_APPLICATION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
