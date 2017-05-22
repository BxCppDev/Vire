//! \file  vire/time/time_interval_cut.h
//! \brief A time interval cut
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

#ifndef VIRE_TIME_TIME_INTERVAL_CUT_H
#define VIRE_TIME_TIME_INTERVAL_CUT_H

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/cuts:
#include <cuts/i_cut.h>

namespace vire {

  namespace time {

    //! \brief A time interval cut
    class time_interval_cut : public cuts::i_cut
    {
    public:

      //! Default constructor
      //! @arg logging_priority_ the logging priority threshold
      time_interval_cut(datatools::logger::priority logging_priority_ =
                        datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~time_interval_cut();

      //! Initialization
      //! @arg config_ the container of configuration parameters
      //! @arg srvc_mgr_ the service manager optionally used to initialize the cut (not used)
      //! @arg cuts_ the dictionary of cuts optionally used to initialize the cut (not used)
      virtual void initialize(const datatools::properties & config_,
                              datatools::service_manager & srvc_mgr_,
                              cuts::cut_handle_dict_type & cuts_);

      //! Reset
      virtual void reset();

      //! Check the validity of the time interval
      //! @return true if the embedded time interval is valid
      bool has_valid_interval() const;

      //! Return the time interval
      //! @return the embedded valid time interval
      const boost::posix_time::time_period & get_interval() const;

      //! Set the time interval
      //! @arg period_ the time period used as the valid interval
      void set_interval(const boost::posix_time::time_period & period_);

      //! Set the time interval
      //! @arg start_ the start time of the valid interval
      //! @arg duration_ the duration of the valid interval
      void set_interval(const boost::posix_time::ptime & start_,
                        const boost::posix_time::time_duration & duration_);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_ = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected :

      //! Selection
      virtual int _accept();

    private:

      boost::posix_time::time_period _interval_; //!< The allowed time interval

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(time_interval_cut);

    };

  } // namespace time

} // namespace vire

#endif // VIRE_TIME_TIME_INTERVAL_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
