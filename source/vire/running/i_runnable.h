//! \file  vire/running/i_runnable.h
//! \brief Vire process runnable interface
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_RUNNING_I_RUNNABLE_H
#define VIRE_RUNNING_I_RUNNABLE_H

// Standard Library:
#include <string>

namespace vire {

  namespace running {

    class run_control;

    struct i_runnable
    {
      virtual void run() = 0;

      virtual void stop() = 0;

      virtual const run_control & get_rc() const = 0;
      
    };

  } // namespace running

} // namespace vire

#endif // VIRE_RUNNING_I_RUNNABLE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
