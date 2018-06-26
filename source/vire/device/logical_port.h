//! \file  vire/device/logical_port.h
//! \brief Vire logical port
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

#ifndef VIRE_DEVICE_LOGICAL_PORT_H
#define VIRE_DEVICE_LOGICAL_PORT_H

// Standard library:
#include <string>
#include <vector>
#include <map>

// Third Party:
// - Bayeux/datatools:
#include <datatools/enriched_base.h>

// This project:
#include <vire/device/device_utils.h>

namespace vire {

  namespace device {

    // Forward declarations:
    class base_port_model;

    //! \brief The logical port
    class logical_port
      : public datatools::enriched_base
    {
    public:

      //! Default constructor
      logical_port();

      //! Destructor
      virtual ~logical_port();

      //! Check if the logical has a port model
      bool has_model() const;

      //! Set a non mutable reference to the port model
      void set_model(const base_port_model &);

      //! Return a non mutable reference to the port model
      const base_port_model & get_model() const;

      //! Check initialization flag
      bool is_initialized() const;

      //! Initialize
      void initialize();

      //! Reset
      void reset();

      //! Smart print
      // virtual
      void tree_dump(std::ostream & out_ = std::clog,
                     const std::string & title_  = "",
                     const std::string & indent_ = "",
                     bool inherit_ = false) const;

    private:

      bool                    _initialized_; //!< Initialization flag
      const base_port_model * _model_;       //!< Pointer to the port model from which the logical is constructed

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_LOGICAL_PORT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
