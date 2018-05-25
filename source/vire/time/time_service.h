//! \file  vire/time/time_service.h
//! \brief Time service
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

#ifndef VIRE_TIME_TIME_SERVICE_H
#define VIRE_TIME_TIME_SERVICE_H

// Standard library:
#include <string>
#include <memory>
#include <tuple>

// Third Party:
// - Boost:
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/logic/tribool.hpp>
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/properties.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

namespace vire {

  namespace time {

    //! \brief Time service refers to a specific time zone and provides tools for local <-> UTC conversion
    class time_service
      : public ::datatools::base_service
    {
      
    public:
      
      //! Default constructor
      time_service();

      //! Destructor
      ~time_service();

      //! Check if the time zone database filename is set
      bool has_time_zone_database_path() const;

      //! Set the time zone database filename
      void set_time_zone_database_path(const std::string & time_zone_db_filename_);
      
      //! Return the time zone database filename
      const std::string & get_time_zone_database_path() const;

      //! Check if the time zone identifier associated the time service is set
      bool has_time_zone_id() const;

      //! Set the time zone identifier associated the time service
      void set_time_zone_id(const std::string & time_zone_id_);
      
      //! Return the time zone identifier associated the time service
      const std::string & get_time_zone_id() const;

      //! Return the time zone database
      const boost::local_time::tz_database & get_time_zone_database() const;
      
      //! Return the time zone associated to the time service
      boost::local_time::time_zone_ptr get_time_zone() const;

      //! Invalidate a local time
      void invalidate_local_time(boost::local_time::local_date_time & local_time_) const;

      //! Check if date/time local time informations are valid with respect to the time zone
      bool check_local_time(const unsigned int year_,
                            const unsigned int month_,
                            const unsigned int day_,
                            const unsigned int hours_,
                            const unsigned int minutes_,
                            const unsigned int seconds_,
                            const unsigned int microseconds_,
                            const bool dst_ = false) const;

      //! Characterize a local time point
      void local_time_infos(const boost::posix_time::ptime & local_,
                            bool & valid_,
                            boost::logic::tribool & dst_) const;
      
      //! Build a local time with respect to the current time zone
      std::tuple<bool, std::string> make_local_time(boost::local_time::local_date_time & local_time_,
                                                    const unsigned int year_,
                                                    const unsigned int month_,
                                                    const unsigned int day_,
                                                    const unsigned int hours_ = 0,
                                                    const unsigned int minutes_ = 0,
                                                    const unsigned int seconds_ = 0,
                                                    const unsigned int microseconds_ = 0,
                                                    const bool dst_ = false) const;
      
      //! Convert a local time to UTC time
      boost::posix_time::ptime local_to_utc(const boost::posix_time::ptime & local_,
                                            const bool dst_ = false) const;

      //! Convert UTC time to a local time
      boost::posix_time::ptime utc_to_local(const boost::posix_time::ptime & utc_) const; 
      
      //! Check if the service is initialized
      virtual bool is_initialized() const;
    
      //! Initialize the service using a list of properties with access to a dictionary of services
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the service
      virtual int reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_         = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    protected:

      void _set_defaults();

      void _init();

      void _reset();

    private:

      // Management:
      bool _initialized_; //!< Initialization flag
 
      // Configuration:
      std::string _time_zone_db_path_; //!< Time zone database path
      std::string _time_zone_id_; //!< Time zone identifier
     
      // Private implementation (PIMPL):
      struct work_type;
      std::unique_ptr<work_type> _work_; //!< Private logging
      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(time_service);

    };
    
  } // namespace session

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::time::time_service)
#endif // Q_MOC_RUN

#endif // VIRE_TIME_TIME_SERVICE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
