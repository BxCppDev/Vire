//! \file    vire/time/time_availability.h
//! \brief   Representation of the rule to access some resource in time
//! \details The Vire library is able to describe resource objects
//!          in some environment that can be accessed through sessions.
//!          Some resources may have special time slots when they are (or
//!          not) accessible. The time_availability is implemented through
//!          the cuts library.
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

#ifndef VIRE_TIME_TIME_AVAILABILITY_H
#define VIRE_TIME_TIME_AVAILABILITY_H

// Standard library:
#include <iostream>

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>

namespace cuts {
  // Forward declaration:
  class i_cut;
}

namespace vire {

  namespace time {

    //! \brief Resource time availability
    class time_availability : public datatools::i_tree_dumpable
    {
    public:

      //! Default constructor
      time_availability();

      //! Constructor
      time_availability(const cuts::i_cut &);

      //! Destructor
      virtual ~time_availability();

      //! Check validity
      bool is_valid() const;

      //! Set the time resolution
      void set_time_resolution(const boost::posix_time::time_duration &);

      //! Return the time resolution
      const boost::posix_time::time_duration & get_time_resolution() const;

      //! Set cut
      void set_time_cut(const cuts::i_cut &);

      //! Return the name
      std::string get_name() const;

      //! Return the handled cut
      const cuts::i_cut & get_time_cut() const;

      //! Check if a given time falls inside the availability time slot
      bool accept_time(const boost::posix_time::ptime &) const;

      //! Check if a given time period is entirely contained inside the availability time slot
      bool accept_period(const boost::posix_time::time_period &) const;

      //! Reset
      void reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Simple draw of the time availability
      void draw(std::ostream & out_,
                const boost::posix_time::time_period & time_slice_,
                const boost::posix_time::time_duration & delta_time_,
                int shift_ = 0, int mode_ = 0) const;

    protected:

      //! Set default attribute values
      void _set_defaults();

    private:

      boost::posix_time::time_duration _time_resolution_; //!< Time resolution (default: 1 second)
      const cuts::i_cut * _time_cut_; //!< Handled time cut

    };

  } // namespace time

} // namespace vire

#endif // VIRE_TIME_TIME_AVAILABILITY_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
