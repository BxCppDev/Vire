// /vire/cmsclient/default_negotiation_worker.cc
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

// Ourselves:
#include <vire/cmsclient/default_negotiation_worker.h>

namespace vire {

  namespace cmsclient {

    default_negotiation_worker::default_negotiation_worker(negotiation & nego_)
      : i_worker(nego_)
    {
      return;
    }

    default_negotiation_worker::~default_negotiation_worker()
    {
      return;
    }

    void default_negotiation_worker::set_mode(const mode_type mode_)
    {
      _mode_ = mode_;
      return;
    }

    default_negotiation_worker::mode_type
    default_negotiation_worker::get_mode() const
    {
      return _mode_;
    }

    bool default_negotiation_worker::run(session_connection_parameters & result_)
    {
      if (_mode_ == MODE_FAKE) {
        return _run_fake_(result_);
      }
      if (_mode_ == MODE_BASIC_TUI) {
        return _run_basic_tui_(result_);
      }
      return false;
    }

    bool default_negotiation_worker::_run_fake_(session_connection_parameters & result_)
    {
      result_.set_id("ClientXYZ");
      result_.set_role("universe");
      result_.set_deadline(vire::time::now() + boost::posix_time::hours(2));
      result_.set_login("ia1Mae0");
      result_.set_password("Pha0ohr6jaiVu");
      return result_.is_valid();
    }

    bool default_negotiation_worker::_run_basic_tui_(session_connection_parameters & result_)
    {
      std::string role;
      std::clog << "Role : ";
      std::getline(std::cin, role);

      return result_.is_valid();
    }

  } // namespace cmsclient

} // namespace vire
