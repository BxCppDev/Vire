//! \file  vire/cmsserver/session_reservation.h
//! \brief The CMS server session reservation
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_SESSION_RESERVATION_H
#define VIRE_CMSSERVER_SESSION_RESERVATION_H

// Standard Library:
#include <string>

// This project:
#include <vire/utility/comparison.h>
#include <vire/cmsserver/session_info.h>

namespace vire {

  namespace cmsserver {

    /// \brief Session reservation
    class session_reservation
      : public datatools::i_tree_dumpable
      , public datatools::i_serializable
    {
    public:

      static const int32_t INVALID_ID = -1;

      /// Default constructor
      session_reservation();

      /// Destructor
      virtual ~session_reservation();

      /// Check validity
      bool is_valid() const;

      /// Reset
      void reset();

      bool has_id() const;

      void set_id(const int32_t id_);

      int32_t get_id() const;

      bool has_sinfo() const;

      void set_sinfo(const session_info &);

      const session_info & get_sinfo() const;

      void set_start_macro(const std::string &);

      bool has_start_macro() const;

      const std::string & get_start_macro() const;

      bool has_stop_macro() const;

      void set_stop_macro(const std::string &);

      const std::string & get_stop_macro() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /** \brief Comparison of two session reservations.
       *
       *  One compares the begin (b) times of embedded session infos.
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

      int32_t      _id_ = INVALID_ID; ///< Session réservation ID
      session_info _sinfo_;       ///< Session information
      std::string  _start_macro_; ///< Start macro executed at session start
      std::string  _stop_macro_;  ///< Stop macro executed at session stop

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
