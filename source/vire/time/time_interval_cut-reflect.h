//! \file  vire/time/time_interval_cut-reflect.h
//! \brief Vire reflection support for time interval
//
// Copyright (c) 2015 by Francois Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_TIME_TIME_INTERVAL_CUT_REFLECT_H
#define VIRE_TIME_TIME_INTERVAL_CUT_REFLECT_H

// Ourselves:
#include <vire/time/time_interval_cut.h>

// Export the introspection code :
DR_CLASS_EXPORT(::vire::time::time_interval_cut, "vire::time::time_interval_cut")

namespace datatools {
  namespace detail {
    namespace reflection {

      // NOT IMPLEMENTED YET
      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::time::time_interval_cut, /*tag_*/)
      {
        DR_CLASS_DECLARE(::vire::time::time_interval_cut)
          // .DR_CLASS_BASE(::cuts::i_cut)
          .DR_CLASS_CONSTRUCTOR()
        ;
        return;
      }

    } // end of namespace introspection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_TIME_TIME_INTERVAL_CUT_REFLECT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
