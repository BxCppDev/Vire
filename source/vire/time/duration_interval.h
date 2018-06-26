//! \file  vire/time/duration_interval.h
//! \brief Vire CMS duration interval
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

#ifndef VIRE_TIME_DURATION_INTERVAL_H
#define VIRE_TIME_DURATION_INTERVAL_H

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>

// This project:
#include <vire/time/utils.h>
  
namespace vire {

  namespace time {
    
    class duration_interval
      : public datatools::i_tree_dumpable
    {

    public:

      //! Check the validity of a pair of min/max durations
      //!
      //! Valid intervals are:
      //! \code
      //! -----(;)---------
      //! -----(;max]------
      //! -----[min;max]---
      //! -----[min;)------
      //! \endcode
      static bool validate(const boost::posix_time::time_duration & min_,
                           const boost::posix_time::time_duration & max_);

      static duration_interval make_min(const boost::posix_time::time_duration & min_);
      
      static duration_interval make_max(const boost::posix_time::time_duration & max_);
      
      static duration_interval make_min_max(const boost::posix_time::time_duration & min_,
                                            const boost::posix_time::time_duration & max_);
      
      /// Default constructor
      duration_interval();
      
      /// Constructor
      duration_interval(const boost::posix_time::time_duration & min_);
      
      /// Constructor
      duration_interval(const boost::posix_time::time_duration & min_,
                        const boost::posix_time::time_duration & max_);

      /// Destructor
      ~duration_interval();

      //! Reset
      void reset();

      //! Check if the minimum duration is set
      bool has_min() const;

      //! Set the minimum duration
      void set_min(const boost::posix_time::time_duration &);

      //! Return the minimum duration
      const boost::posix_time::time_duration & get_min() const;

      //! Check if the maximum duration is set
      bool has_max() const;

      //! Set the maximum duration
      void set_max(const boost::posix_time::time_duration &);

      //! Return the maximum duration
      const boost::posix_time::time_duration & get_max() const;

      //! Set the minimum and maximum durations
      void set_min_max(const boost::posix_time::time_duration & min_,
                       const boost::posix_time::time_duration & max_);

      //! Set the minimum and maximum durations from real time duration with explicit unit
      void set_min_max(const double time_min_, const double time_max_ = std::numeric_limits<double>::quiet_NaN());
      
      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const;
     
    private:

      // Pre-session time constraints:
      boost::posix_time::time_duration _min_; //!< Minimum duration of the use case's action
      boost::posix_time::time_duration _max_; //!< Maximum duration of the use case's action
      
    };
   
  } // namespace time

} // namespace vire

#endif // VIRE_TIME_DURATION_INTERVAL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
