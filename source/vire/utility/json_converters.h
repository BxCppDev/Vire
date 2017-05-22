//! \file  vire/utility/json_converters.h
//! \brief JSON converters for foreign classes
//
// Copyright (c) 2016 by Jean Hommet <hommet@lpccaen.in2p3.fr>,
//                       François Mauger <mauger@lpccaen.in2p3.fr>,
//                       Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_JSON_CONVERTERS_H
#define VIRE_UTILITY_JSON_CONVERTERS_H

// Third party:
#include <bayeux/datatools/version_id.h>
// - BxJsontools:
#include <bayeux/jsontools/core.h>
#include <bayeux/jsontools/node.h>

namespace jsontools {

  //! \brief A JSON converter for the 'datatools::version_id' class
  template<>
  class converter<datatools::version_id>
  {
  public:

    static void jsonize(node & node_, datatools::version_id & vid_);

    static void dejsonize(node & node_, datatools::version_id & vid_);

  };

} // end of namespace jsontools

#endif // VIRE_UTILITY_JSON_CONVERTERS_H

// Local Variables: --
// Mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
