//! \file vire/cms/application.cc
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

// Ourselves:
#include <vire/cms/application.h>

namespace vire {

  namespace cms {

    // static
    std::string application::to_string(const category_type app_cat_)
    {
      switch(app_cat_) {
      case CATEGORY_SERVER : return std::string("cmsserver");
      case CATEGORY_SUBCONTRACTOR : return std::string("cmssubcontractor");
      case CATEGORY_CLIENT : return std::string("cmsclient");
      }
      return std::string("");
    }
      
    // static
    bool application::from_string(const std::string & label_,
                                  category_type & app_cat_)
    {
      app_cat_ = CATEGORY_UNDEF;
      if (label_ == to_string(CATEGORY_SERVER)) {
        app_cat_ = CATEGORY_SERVER;
      } else if (label_ == to_string(CATEGORY_SUBCONTRACTOR)) {
        app_cat_ = CATEGORY_SUBCONTRACTOR;
      } else if (label_ == to_string(CATEGORY_CLIENT)) {
        app_cat_ = CATEGORY_CLIENT;
      }
      return app_cat_ != CATEGORY_UNDEF;
    };
    

  } // namespace cms

} // namespace vire
