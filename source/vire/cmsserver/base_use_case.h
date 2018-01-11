//! \file  vire/cmsserver/base_use_case.h
//! \brief Vire CMS base use case
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

#ifndef VIRE_CMSSERVER_BASE_USE_CASE_H
#define VIRE_CMSSERVER_BASE_USE_CASE_H

// Standard Library:
#include <string>
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/factory_macros.h>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace cmsserver {

    class session;

    /// \brief Vire CMS base use case
    class base_use_case
      : public datatools::enriched_base
    {
    public:

      //! Default constructor
      base_use_case();

      //! Destructor
      virtual ~base_use_case();

      void set_mother_session(session & s_);

      //! Check if the maximum duration of the up action is set
      bool has_up_max_duration() const;

      //! Set the maximum duration of the up action
      void set_up_max_duration(const boost::posix_time::time_duration &);

      //! Return the maximum duration of the up action
      const boost::posix_time::time_duration & get_up_max_duration() const;

      //! Check if the minimum duration of the work action is set
      bool has_work_min_duration() const;

      //! Set the minimum duration of the work action
      void set_work_min_duration(const boost::posix_time::time_duration &);

      //! Return the minimum duration of the work action
      const boost::posix_time::time_duration & get_work_min_duration() const;

      //! Check if the maximum duration of the work action is set
      bool has_work_max_duration() const;

      //! Set the maximum duration of the work action
      void set_work_max_duration(const boost::posix_time::time_duration &);

      //! Return the maximum duration of the work action
      const boost::posix_time::time_duration & get_work_max_duration() const;

      //! Check if the maximum duration of the down action is set
      bool has_down_max_duration() const;

      //! Set the maximum duration of the down action
      void set_down_max_duration(const boost::posix_time::time_duration &);

      //! Return the maximum duration of the down action
      const boost::posix_time::time_duration & get_down_max_duration() const;

      bool is_initialized() const;

      void initialize_simple();

      void initialize(const datatools::properties & config_);

      void reset();

      void up();

      void work();

      void down();

    private:

      void _init_(const datatools::properties & config_);

      void _reset_();

      virtual void _at_initialized_(const datatools::properties & config_) = 0;

      virtual void _at_reset_() = 0;

      virtual void _at_up_() = 0;

      virtual void _at_work_() = 0;

      virtual void _at_down_() = 0;

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
      boost::posix_time::time_duration _up_max_duration_;   //!< Maximum duration of the up action
      boost::posix_time::time_duration _work_min_duration_; //!< Minimum duration of the work action
      boost::posix_time::time_duration _work_max_duration_; //!< Maximum duration of the work action
      boost::posix_time::time_duration _down_max_duration_; //!< Maximum duration of the down action

      // Internal data:
      session * _mother_session_ = nullptr; //!< Handle to the mother session

      // Factory declaration :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_use_case)

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_BASE_USE_CASE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
