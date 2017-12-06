//! \file    vire/vire.h
//! \brief   Provide system init for Vire internals
//! \details
//
// Copyright (c) 2015-2016 by Francois Mauger <mauger@lpccaen.in2p3.fr>
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_VIRE_H
#define VIRE_VIRE_H

// Standard Library:
#include <string>

//! Main namespace for the Vire library
namespace vire {

  //! Check if the Vire library is initialized
  bool is_initialized();

  //! Initialize special resources of the Vire library
  void initialize(int argc_ = 0, char * argv_[] = 0, uint32_t flags_ = 0);

  //! Terminate special resources of the Vire library
  void terminate();

} // namespace vire

#endif // VIRE_VIRE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
