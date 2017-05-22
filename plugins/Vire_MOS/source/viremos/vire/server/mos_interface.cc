//! \file vire/server/mos_interface.cc
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
#include <vire/server/mos_interface.h>

namespace vire {

  namespace server {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(mos_interface, "vire::server::mos_interface");

    mos_interface::mos_interface()
      : datatools::base_service("MOS", "Vire server MOS interface", "")
    {
      _initialized_ = false;
      _set_defaults();
      return;
    }

    mos_interface::~mos_interface()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void mos_interface::_set_defaults()
    {

      // Do something here

      set_logging_priority(datatools::logger::PRIO_FATAL);
      return;
    }

    bool mos_interface::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int mos_interface::initialize(const datatools::properties & config_,
                            datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Manager is already initialized!");

      this->::datatools::base_service::common_initialize(config_);

      // Do something here

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    int mos_interface::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Manager is not initialized!");
      _initialized_ = false;

      // Do something here

      _set_defaults();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void mos_interface::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);



      return;
    }

  } // namespace server

} // namespace vire
