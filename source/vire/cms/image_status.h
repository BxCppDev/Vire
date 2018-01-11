//! \file  vire/cms/image_status.h
//! \brief Image status status of a device or resource
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMS_IMAGE_STATUS_H
#define VIRE_CMS_IMAGE_STATUS_H

// Standard Library:
#include <string>
// Third Party:
// - Boost:
#include <boost/logic/tribool.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// This project:

namespace vire {

  namespace cms {

    //! \brief Dynamic timestamped status of a device or resource
    class image_status
      : public datatools::i_tree_dumpable

    {
    public:

      //! Default constructor
      image_status();

      //! Destructor
      virtual ~image_status();

      //! Check if the status is valid
      bool is_valid() const;

      //! Reset
      void reset();

      //! Set the flags in indeterminate state
      void indeterminate_flags();

      //! Check if timestamp is set
      bool has_timestamp() const;

      //! Set the timestamp
      void set_timestamp(const boost::posix_time::ptime &);

      //! Return the timestamp
      const boost::posix_time::ptime & get_timestamp() const;

      //! Reset the timestamp
      void reset_timestamp();

      //! Check if missing status is known
      bool has_missing() const;

      //! Check if the resource is present
      bool is_present() const;

      //! Check if the resource not present
      bool is_missing() const;

      //! Set the missing flag
      void set_missing(bool);

      //! Reset the missing flag
      void reset_missing();

      //! Check if disabled status is known
      bool has_disabled() const;

      //! Check if the resource is enabled
      bool is_enabled() const;

      //! Check if the resource is disabled
      bool is_disabled() const;

      //! Set the disabled flag
      void set_disabled(bool);

      //! Reset the disabled flag
      void reset_disabled();

      //! Check if pending status is known
      bool has_pending() const;

      //! Check if the resource is idle
      bool is_idle() const;

      //! Check if the resource is pending
      bool is_pending() const;

      //! Set the pending flag
      void set_pending(bool);

      //! Reset the pending flag
      void reset_pending();

      //! Check if failed status is known
      bool has_failed() const;

      //! Check if the resource has no failed
      bool is_success() const;

      //! Check if the resource is in failed status
      bool is_failed() const;

      //! Set the failed flag
      void set_failed(bool);

      //! Reset the failed flag
      void reset_failed();

      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const;

    private:

      // Internal data:
      boost::posix_time::ptime    _timestamp_; //!< Last update timestamp (status or value)
      boost::logic::tribool       _missing_  = boost::logic::indeterminate; //!< Missing flag
      boost::logic::tribool       _disabled_ = boost::logic::indeterminate; //!< Disabled flag
      boost::logic::tribool       _pending_  = boost::logic::indeterminate; //!< Pending flag
      boost::logic::tribool       _failed_   = boost::logic::indeterminate; //!< Failed flag

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace resource

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::cms::image_status);
#endif // Q_MOC_RUN

#endif // VIRE_CMS_IMAGE_STATUS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
