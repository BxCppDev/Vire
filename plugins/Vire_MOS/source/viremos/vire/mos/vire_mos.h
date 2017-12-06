//! \file    vire/mos/vire_mos.h
//! \brief   Provide system init for Vire MOS internals
//! \details
//
// Copyright (c) 2015-2017 by Francois Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_MOS_VIRE_MOS_H
#define VIRE_MOS_VIRE_MOS_H

// Standard Library:
#include <string>

// This project Library:
#include <vire/vire.h>

//! Main namespace for the Vire library
namespace vire {

  namespace mos {

    //! Check if the Vire MOS library is initialized
    bool is_initialized();

    //! Initialize special resources of the Vire MOS library
    void initialize(int argc_ = 0, char * argv_[] = 0, uint32_t flags_ = 0);

    //! Terminate special resources of the Vire MOS library
    void terminate();

  } // namespace mos

} // namespace vire

#endif // VIRE_MOS_VIRE_MOS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
