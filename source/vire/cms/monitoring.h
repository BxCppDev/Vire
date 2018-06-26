//! \file  vire/cms/monitoring.h
//! \brief Monitoring
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

#ifndef VIRE_CMS_MONITORING_H
#define VIRE_CMS_MONITORING_H

// Standard Library:
#include <string>

// Third Party:
// - Boost:
#include <boost/variant.hpp>
#include <boost/optional.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>
#include <datatools/exception.h>

// This project:
#include <vire/cms/image_status.h>

namespace vire {

  namespace cms {

    /** \brief Value monitoring mode:
     *  - 'polling' mode:
     *    \code
     *                          +     +                             +
     *              +     +     :     :           +                 :     +
     *        +     :     :     :     :     +     :     +     +     :     :
     *     ---:-----:-----:-----:-----:-----:-----:-----:-----:-----:-----:-----//-----------> t
     *    \endcode
     *  - 'onchange' mode:
     *    \code
     *
     *                          +---------->:   +---->:
     *                    +---->:           +-->:     :              +---->:
     *        +---------->:     :           :   :     +------------->:     + - -  - - -
     *     ---:-----------:-----:-----------:---:-----:--------------:-----:----//----------> t
     *    \endcode
     */
    enum value_monitoring_mode_type {
      VALUE_MONITORING_INVALID  = 0, //!< Invalid value monitoring mode
      VALUE_MONITORING_POLLING  = 1, //!< Instantaneous value monitoring mode (automated polling)
      VALUE_MONITORING_ONCHANGE = 2  //!< On change value monitoring mode (Pub/Sub event driven sampling)
    };

    //! Return the label associated to a value monitoring mode
    std::string value_monitoring_mode_label(const value_monitoring_mode_type);

    //! Return the value monitoring mode associated to a label
    value_monitoring_mode_type value_monitoring_mode(const std::string & label_);

    //! \brief Value monitoring type alias
    typedef boost::variant<
      bool,
      int32_t,
      double,
      std::string> monitored_value_type;
 
  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_MONITORING_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
