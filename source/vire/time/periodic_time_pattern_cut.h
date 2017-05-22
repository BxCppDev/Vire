//! \file  vire/time/periodic_time_pattern_cut.h
//! \brief A periodic time pattern cut
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_TIME_PERIODIC_TIME_PATTERN_CUT_H
#define VIRE_TIME_PERIODIC_TIME_PATTERN_CUT_H

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/cuts:
#include <cuts/i_cut.h>

namespace vire {

  namespace time {

    //! \brief A periodic time pattern cut
    class periodic_time_pattern_cut : public cuts::i_cut
    {
    public:

      //! Default constructor
      //! @arg logging_priority_ the logging priority threshold
      periodic_time_pattern_cut(datatools::logger::priority logging_priority_ =
                                datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~periodic_time_pattern_cut();


      //! Initialization
      //! @arg config_ the container of configuration parameters
      //! @arg srvc_mgr_ the service manager optionally used to initialize the cut (not used)
      //! @arg cuts_ the dictionary of cuts optionally used to initialize the cut (not used)
      virtual void initialize(const datatools::properties & config_,
                              datatools::service_manager & srvc_mgr_,
                              cuts::cut_handle_dict_type & cuts_);

      //! Reset
      virtual void reset();

      //! Check the validity of the time pattern
      //! @return true if the embedded time pattern is valid
      bool has_valid_time_pattern() const;

      //! Return the time pattern
      //! @return the embedded valid time pattern
      const cuts::i_cut & get_time_pattern() const;

      //! Set the time interval
      //! @arg time_pattern_ the handle to the time pattern
      void set_time_pattern(cut_handle_type & time_pattern_);

     //! Check the validity of the period
      //! @return true if the period is valid
      bool has_valid_period() const;

      //! Return the period
      //! @return the period
      const boost::posix_time::time_duration & get_period() const;

      //! Set the period
      //! @arg period_ the period
      void set_period(const boost::posix_time::time_duration & period_);

      // Smart print:
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_ = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected :

      //! Selection
      virtual int _accept();

      virtual void _at_set_user_data();

      virtual void _at_reset_user_data();


    private:

      cuts::cut_handle_type _time_pattern_; //!< The handle to the time pattern
      boost::posix_time::time_duration _period_; //!< The period

      // Macro to automate the registration of the cut :
      CUT_REGISTRATION_INTERFACE(periodic_time_pattern_cut);

    };

  } // namespace time

} // namespace vire

#endif // VIRE_TIME_PERIODIC_TIME_PATTERN_CUT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
