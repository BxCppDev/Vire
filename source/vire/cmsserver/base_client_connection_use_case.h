//! \file  vire/cmsserver/base_client_connection_use_case.h
//! \brief The CMS server client connection use case
//
// Copyright (c) 2016-2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                         François Mauger <mauger@lpccaen.in2p3.fr>
//                         Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_BASE_CLIENT_CONNECTION_USE_CASE_H
#define VIRE_CMSSERVER_BASE_CLIENT_CONNECTION_USE_CASE_H

// Standard Library:
#include <memory>

// This project:
#include <vire/cmsserver/base_use_case.h>
#include <vire/cmsserver/client_connection.h>

namespace vire {

  namespace cmsserver {

    class base_client_connection_use_case
      : public base_use_case
    {
    public:

      //! Default constructor
      base_client_connection_use_case();

      //! Destructor
      virtual ~base_client_connection_use_case();

      void set_check_stop_period(const std::size_t);

      std::size_t get_check_stop_period() const;

    private:

       void _at_initialize_(const datatools::properties & config_) override;

       void _at_terminate_() override;

      // Running:

      virtual void _at_run_prepare_() final;

      // virtual void _at_run_distributable_up_();

      // virtual void _at_run_functional_up_();

      virtual void _at_run_functional_work_loop_iteration_() final;

      // virtual void _at_run_functional_down_();

      // virtual void _at_run_distributable_down_();

      virtual void _at_run_terminate_() final;

    private:

      std::unique_ptr<client_connection> _conn_; ///< Connection handle (unique_ptr)
      int _last_connection_id_ = -1;
      std::size_t _check_stop_period_ = 10; ///! Period for checking a stop request second

       // boost::optional<macro> _start_macro_,
       // boost::optional<macro> _stop_macro_,

      // Class registration:
      VIRE_CMSSERVER_USE_CASE_REGISTRATION_INTERFACE(base_client_connection_use_case);

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_BASE_CLIENT_CONNECTION_USE_CASE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
