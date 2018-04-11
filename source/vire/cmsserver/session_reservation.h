//! \file  vire/cmsserver/session_reservation.h
//! \brief The CMS server session reservation
//
// Copyright (c) 2017-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_SESSION_RESERVATION_H
#define VIRE_CMSSERVER_SESSION_RESERVATION_H

// Standard Library:
#include <string>

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time.hpp>

// This project:
#include <vire/utility/comparison.h>
#include <vire/cmsserver/use_case_info.h>

namespace vire {

  namespace cmsserver {

    /// \brief Session reservation
    class session_reservation
      : public datatools::i_tree_dumpable
      , public datatools::i_serializable
    {
    public:

      static const int32_t INVALID_ID = -1;
      static const int32_t INVALID_SEQUENCE_ID = -1;

      /// \brief A sequence of session reservations
      struct sequence
        : public datatools::i_tree_dumpable
        , public datatools::i_serializable
      {
      public:

        /// Default constructor
        sequence();

        /// Destructor
        virtual ~sequence();

        /// Check validity
        bool is_valid() const;

        /// Reset
        void reset();

        /// Check if the session sequence ID is set
        bool has_sequence_id() const;

        /// Set the session sequence ID
        void set_sequence_id(const int32_t id_);

        /// Return the session sequence ID
        int32_t get_sequence_id() const;

        //! Smart print
        virtual void tree_dump(std::ostream & out_ = std::clog,
                               const std::string & title_  = "",
                               const std::string & indent_ = "",
                               bool inherit_ = false) const;

      private:

        int32_t _sequence_id_ = INVALID_SEQUENCE_ID; ///< Session reservation sequence ID (mandatory)

        //! Serialization interface
        DATATOOLS_SERIALIZATION_DECLARATION()

      };

      /// Default constructor
      session_reservation();

      /// Destructor
      virtual ~session_reservation();

      /// Check if the reservation is a valid/completed object
      bool is_valid() const;

      /// Reset
      void reset();

      /// Check if the session reservation ID is set
      bool has_id() const;

      /// Set the session reservation ID
      void set_id(const int32_t id_);

      /// Return the session reservation ID
      int32_t get_id() const;

      /// Check if the session reservation sequence ID is set
      bool has_sequence_id() const;

      /// Set the session reservation sequence ID
      void set_sequence_id(const int32_t id_);

      /// Return the session reservation sequence ID
      int32_t get_sequence_id() const;

      bool has_booked_by() const;

      const std::string & get_booked_by() const;

      void set_booked_by(const std::string &);

      bool has_last_update() const;

      const boost::posix_time::ptime & get_last_update() const;

      void set_last_update(const boost::posix_time::ptime &);

      /// Check if the reservation was validated by a validator user
      bool is_validated() const;

      /// Set the validation flag
      void set_validated(bool);

      /// Check if the validator identity is set
      bool has_validated_by() const;

      /// Return the validator identity
      const std::string & get_validated_by() const;

      /// Set the validator identity
      void set_validated_by(const std::string &);

      /// Check if the validator time is set
      bool has_validation_time() const;

      /// Set the validation time
      void set_validation_time(const boost::posix_time::ptime &);

      /// Return the validation time
      const boost::posix_time::ptime & get_validation_time() const;

      bool is_cancelled() const;

      void set_cancelled(bool);

      bool has_cancelled_by() const;

      const std::string & get_cancelled_by() const;

      void set_cancelled_by(const std::string &);

      bool has_cancellation_time() const;

      void set_cancellation_time(const boost::posix_time::ptime &);

      const boost::posix_time::ptime & get_cancellation_time() const;

      bool has_role_description() const;

      const std::string & get_role_description() const;

      void set_role_description(const std::string &);

      /// Check if session time period is set
      bool has_when() const;

      /// Set session time period
      void set_when(const boost::posix_time::time_period & when_);

      /// Return session time period
      const boost::posix_time::time_period & get_when() const;

      bool has_use_case_info() const;

      void set_use_case_info(const use_case_info & uci_);

      const use_case_info & get_use_case_info() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /** \brief Comparison of two session reservations.
       *
       *  One compares the begin (b) times.
       *  Last (l) times are also used to discriminate equal begin times:
       *
       *  \code
       *    -------------------------------------> time
       *          :              :
       *  1 ------b==============l---------------- less
       *  2 ------:---b==============l------------
       *          :              :
       *  1 ------b==============l---------------- less
       *  2 ------:---b======l---:----------------
       *          :              :
       *  1 ------b==============l---------------- less
       *  2 ------b==========l---:----------------
       *          :              :
       *  1 ------b==============l---------------- equal
       *  2 ------b==============l----------------
       *          :              :
       *  1 ------b==============l---------------- greater
       *  2 ------b====================l----------
       *          :              :
       *  1 ------b==============l---------------- greater
       *  2 --b======================l------------
       *          :              :
       *  1 ------b==============l---------------- greater
       *  2 --b==============l---:----------------
       *  \endcode
       *
       */
      static vire::utility::comparison_result compare(const session_reservation & r1_,
                                                      const session_reservation & r2_);

    private:

      int32_t                        _id_ = INVALID_ID;          ///< Session réservation ID (mandatory)
      int32_t                        _sequence_id_ = INVALID_ID; ///< Session sequence ID (mandatory)
      std::string                    _booked_by_;          ///< Identity of the user who booked the session (optional)
      boost::posix_time::ptime       _last_update_;        ///< Last update time (optional)
      bool                           _validated_ = false;  ///< Validation time
      std::string                    _validated_by_;       ///< Identity of the user who booked the session (optional)
      boost::posix_time::ptime       _validation_time_;    ///< Identity of the user who booked the session (optional)
      bool                           _cancelled_ = false;  ///< Cancellation flag
      std::string                    _cancelled_by_;       ///< Identity of the user who booked the session (optional)
      boost::posix_time::ptime       _cancellation_time_;  ///< Identity of the user who booked the session (optional)
      std::string                    _role_description_;   ///< The description of the role associated to the session
      boost::posix_time::time_period _when_;               ///< Session time period (mandatory)
      use_case_info                  _use_case_info_;      ///< Use case information

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION()

    };

    bool operator<(const session_reservation & r1_, const session_reservation & r2_);

  } // namespace cmsserver

} // namespace vire

// #ifndef Q_MOC_RUN
// // Activate reflection layer:
// DR_CLASS_INIT(vire::cmsserver::session_reservation);
// #endif // Q_MOC_RUN

#endif // VIRE_CMSSERVER_SESSION_RESERVATION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
