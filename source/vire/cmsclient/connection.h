//! \file  vire/cmsclient/negotiation.h
//! \brief negotiation
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSCLIENT_NEGOTIATION_H
#define VIRE_CMSCLIENT_NEGOTIATION_H

// This project:

namespace vire {

  namespace cmsclient {

    class negotiation
    {
    public:

      // Negotiation worker interface
      struct i_worker
      {
        virtual bool run);

      };

      bool run(const session_connection_info & nego_result_);


    private:

    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_NEGOTIATION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
