//! \file  vire/cmsclient/default_negotiation_worker.h
//! \brief Default negotiation worker
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

#ifndef VIRE_CMSCLIENT_DEFAULT_NEGOTIATION_WORKER_H
#define VIRE_CMSCLIENT_DEFAULT_NEGOTIATION_WORKER_H

// This project:
#include <vire/cmsclient/session_connection_parameters.h>
#include <vire/cmsclient/negotiation.h>

namespace vire {

  namespace cmsclient {

    class default_negotiation_worker
      : public negotiation::i_worker
    {
    public:
      enum mode_type {
        MODE_INVALID,
        MODE_FAKE,
        MODE_BASIC_TUI
      };

      default_negotiation_worker(negotiation & nego_);

      virtual ~default_negotiation_worker();

      void set_mode(const mode_type);

      mode_type get_mode() const;

      virtual bool run(session_connection_parameters & result_);

    private:

      bool _run_fake_(session_connection_parameters & result_);

      bool _run_basic_tui_(session_connection_parameters & result_);

    private:

      mode_type _mode_ = MODE_INVALID;

    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_DEFAULT_NEGOTIATION_WORKER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
