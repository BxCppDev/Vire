//! \file  vire/cmsclient/setup_infos_tui.h
//! \brief Server infos text user interface
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

#ifndef VIRE_CMSCLIENT_SETUP_INFOS_TUI_H
#define VIRE_CMSCLIENT_SETUP_INFOS_TUI_H

// This project:
#include <vire/cmsclient/setup_infos.h>

namespace vire {

  namespace cmsclient {

    /// \brief Text user interface for the setup informations
    class setup_infos_tui
    {
    public:

      /// Constructor
      setup_infos_tui();

      /// Run the user interface
      bool run(setup_infos & sinfo_);

    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_SETUP_INFOS_TUI_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
