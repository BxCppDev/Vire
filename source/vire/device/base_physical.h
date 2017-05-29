//! \file  vire/device/base_physical.h
//! \brief Vire base physical
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

#ifndef VIRE_DEVICE_BASE_PHYSICAL_H
#define VIRE_DEVICE_BASE_PHYSICAL_H

// Standard library:
#include <string>
#include <iostream>

// Third party:
// - Boost:
// #include <boost/core/noncopyable.hpp>
// - Bayeux/datatools:
#include <datatools/enriched_base.h>

namespace vire {

  namespace device {

    // Forward class declaration
    class i_instance;
    class logical_device;

    //! \brief The base class of physical objects
    class base_physical
      : public datatools::enriched_base
    // , private boost::noncopyable
    {
    public:

      //! Return the name suffix of physical objects
      static const std::string & physical_name_suffix();

      //! Return the name of a physical objects from a base name
      static std::string make_physical_name(const std::string & basename_);

      //! Default constructor
      base_physical();

      //! Copy constructor
      base_physical(const base_physical &);

      //! Destructor
      virtual ~base_physical();

      //! Assignement
      base_physical & operator=(const base_physical &);

      //! Reset
      void reset();

      //! Check if a mother logical device is defined
      bool has_mother_logical() const;

      //! Set the mother logical device
      void set_mother_logical(const logical_device &);

      //! Return non mutable logical device
      const logical_device & get_mother_logical() const;

      //! Reset the mother logical device
      void reset_mother_logical();

      //! Check if an instance object is set
      bool has_instance() const;

      //! Set the reference to an external instance object
      void set_instance(const i_instance &);

      //! Set the reference to a owned internal instance object
      void set_instance(const i_instance *);

      //! Set the reference to an instance object
      void set_instance(const i_instance *, bool owned_);

      //! Return a const reference to the instance
      const i_instance & get_instance() const;

      //! Reset the reference to the internal instance object
      void reset_instance();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      const logical_device * _mother_logical_ = nullptr; //!< Handle to the mother logical device
      bool                   _owned_instance_ = false;   //!< Flag to own/delete the instance
      const i_instance *     _instance_       = nullptr; //!< Handle to the instance object

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_BASE_PHYSICAL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
