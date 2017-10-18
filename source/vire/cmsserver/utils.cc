//! \file vire/cmsserver/utils.cc
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/utils.h>

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>

namespace vire {

  namespace cmsserver {

    datatools::logger::priority logging(datatools::logger::priority p_)
    {
      static boost::scoped_ptr<datatools::logger::priority> _prio;
      if (_prio.get() == nullptr) {
        _prio.reset(new datatools::logger::priority);
        *_prio = datatools::logger::PRIO_FATAL;
      }
      if (p_ != datatools::logger::PRIO_UNDEFINED) {
        *_prio = p_;
      }
      return *_prio;
    }

    const std::string & service::default_log_name()
    {
      static const std::string _n("Log");
      return _n;
    }

    const std::string & service::default_users_name()
    {
      static const std::string _n("Users");
      return _n;
    }

    const std::string & service::default_authentication_name()
    {
      static const std::string _n("Auth");
      return _n;
    }

    const std::string & service::default_devices_name()
    {
      static const std::string _n("Devices");
      return _n;
    }

    const std::string & service::default_resources_name()
    {
      static const std::string _n("Resources");
      return _n;
    }

    const std::string & service::default_gate_name()
    {
      static const std::string _n("Gate");
      return _n;
    }

    // const std::string & service::default_rabbitctrl_name()
    // {
    //   static const std::string _n("RabbitCtrl");
    //   return _n;
    // }

    const std::string & service::default_agenda_name()
    {
      static const std::string _n("Agenda");
      return _n;
    }

    const std::string & service::default_pubsubctrl_name()
    {
      static const std::string _n("PubsubCtrl");
      return _n;
    }

    const std::string & service::default_com_name()
    {
      static const std::string _n("Com");
      return _n;
    }

  } // namespace cmsserver

} // namespace vire
