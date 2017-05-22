//! \file  vire/device/single_instance.h
//! \brief Single instance object
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

#ifndef VIRE_DEVICE_SINGLE_INSTANCE_H
#define VIRE_DEVICE_SINGLE_INSTANCE_H

// This project:
#include <vire/device/i_instance.h>
#include <vire/device/slot.h>

namespace vire {

  namespace device {

    //! \brief A single instance of a daughter object
    //!        embedded in a mother object with respect to the
    //!        mother reference instantiation space.
    //!
    //! The single instance object is associated to an unique 'slot'
    //! available in the mother object, whatever the dimension of the
    //! logic instantiation space is.
    //!
    //! Examples:
    //!  - In a space without dimension:
    //! @code
    //!              +----------------+
    //!              |                |
    //!  slot(0)     |            [*] | this is the unique instantiation possibility
    //!              |                | in such a dimensionless space
    //!              +----------------+
    //! @endcode
    //!  - In a space of dimension one:
    //! @code
    //!              +------------------+
    //!              |                  |
    //!  slot(1:2)   | -[.]--[.]--[*]-> | this is one of three instantiation possibilities
    //!              |   0    1    2    | in such a 1D space
    //!              +- ----------------+
    //! @endcode
    //!  - In a space of dimension two:
    //! @code
    //!              +-------------------+
    //!              |    ^              |
    //!              |    :              |
    //!              | 3 [.]--[.]--[.]   |
    //!              | 2 [.]--[.]--[.]   |
    //!              | 1 [.]--[.]--[*]   | this is one of 12 instantiation possibilities
    //!  slot(2:2,1) | 0 [.]--[.]--[.]-> | in such a 2D space
    //!              |    0    1    2    |
    //!              +-------------------+
    //! @endcode
    //!
    class single_instance : public i_instance
    {

    public:

      //! Default constructor
      single_instance();

      //! Constructor
      single_instance(const slot & slot_);

      //! Destructor
      virtual ~single_instance();

      //! Check slot validity
      bool has_slot() const;

      //! Set the slot
      void set_slot(const slot & slot_);

      //! Reset the slot
      void reset_slot();

      //! Return the slot
      const slot & get_slot() const;

      //! Return the slot
      slot & grab_slot();

      //! Check validity
      virtual bool is_valid() const;

      //! Reset
      virtual void reset();

      //! Return the dimension of the instantiation space
      virtual space::dimension_type get_dimension() const;

      //! Return the number of instanciated items
      virtual size_t get_number_of_items() const;

      // //! Compute an array of indexes in a multidimensional frame from the item rank/index
      // virtual size_t compute_index_map(std::vector<uint32_t> & map_, int item_) const;

      //! Compute the slot at given item rank/index
      virtual void fetch_slot(int item_, slot & s_) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_         = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_          = false) const;

      //! Parse from a string
      virtual bool from_string(const std::string &);

      //! Convert to a description string
      void to_string(std::string &) const;

      //! Parse from a description string
      static bool from_string(const std::string &, single_instance &);

      //! Convert to a description string
      static void to_string(std::string &, const single_instance &);

      //! Print
      friend std::ostream & operator<<(std::ostream & out_,
                                       const single_instance & si_);

    private:

      slot _slot_; //!< Single unique slot where an object is instantiated

      //! Registration of a system factory
      VIRE_DEVICE_INSTANCE_REGISTRATION_INTERFACE(single_instance)

      // //! Serialization interface
      // DATATOOLS_SERIALIZATION_DECLARATION()

      //! Reflection interface
      DR_CLASS_RTTI()

    };

  } // namespace device

} // namespace vire

// Activate reflection layer for the vire::device::single_instance class :
DR_CLASS_INIT(::vire::device::single_instance)

#endif // VIRE_DEVICE_SINGLE_INSTANCE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
