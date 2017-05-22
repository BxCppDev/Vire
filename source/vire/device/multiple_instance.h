//! \file  vire/device/multiple_instance.h
//! \brief Multiple instance object
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

#ifndef VIRE_DEVICE_MULTIPLE_INSTANCE_H
#define VIRE_DEVICE_MULTIPLE_INSTANCE_H

// Standard library:
#include <vector>

// This project:
#include <vire/device/i_instance.h>
#include <vire/device/slot.h>

namespace vire {

  namespace device {

    //! \brief The multiple instance of a collection of daughter objects embedded in a mother object
    //!        with respect to the mother reference layout
    //!
    //! The multiple instance object is associated to an arbitrary set of distinct 'slots'
    //! available in the mother object.
    //!
    //! Multiple slots taken from a logic instantiation space of dimension 1:
    //! @code
    //!        +-----------------------------------+
    //!        |   0   1   2   3   4   5   6   7   |  item:   0     1     2     3
    //!        |  [.] [*] [*] [.] [.] [*] [.] [*]  |  slot: (1:1) (1:2) (1:5) (1:7)
    //!        |                                   |
    //!        +-----------------------------------+
    //! @endcode
    //!
    //!
    //! Multiple slots taken from a logic instantiation space of dimension 2:
    //! @code
    //!       +--------------------+
    //!       |    0   1   2   3   |  item:    0       1      2
    //!       | 2 [.] [*] [.] [.]  |  slot: (2:1,2) (2:2,1) (2:0,0)
    //!       | 1 [.] [.] [*] [.]  |
    //!       | 0 [*] [.] [.] [.]  |
    //!       |                    |
    //!       +--------------------+
    //! @endcode
    //!
    class multiple_instance : public i_instance
    {

    public:

      //! Check if multiple slots exist
      bool has_slots() const;

      //! Check if a slot exists
      bool has_slot(const slot & slot_) const;

      //! Append a slot to the set of slots
      void append_slot(const slot & slot_);

      //! Remove a slot from the set of slots
      void remove_slot(const slot & slot_);

      //! Return the number of slots
      uint32_t get_number_of_slots() const;

      //! Return the array of slots
      const std::vector<slot> & get_slots() const;

      //! Constructor
      multiple_instance();

      //! Destructor
      virtual ~multiple_instance();

      //! Check validity
      virtual bool is_valid() const;

      //! Reset
      virtual void reset();

      //! Return the dimension of the instantiation space
      virtual space::dimension_type get_dimension() const;

      //! Return the number of instance items
      virtual size_t get_number_of_items() const;

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
      static bool from_string(const std::string &, multiple_instance &);

      //! Convert to a description string
      static void to_string(std::string &, const multiple_instance &);

      //! Print
      friend std::ostream & operator<<(std::ostream & out_,
                                       const multiple_instance & ai_);

    private:

      std::vector<slot> _slots_; //!< Ordered set of slots

      //! Registration of a system factory
      VIRE_DEVICE_INSTANCE_REGISTRATION_INTERFACE(multiple_instance);

      // //! Serialization interface
      // DATATOOLS_SERIALIZATION_DECLARATION();

      //! Reflection interface
      DR_CLASS_RTTI();

    };

  } // namespace device

} // namespace vire

// Activate reflection layer for the vire::device::multiple_instance class :
DR_CLASS_INIT(::vire::device::multiple_instance);

#endif // VIRE_DEVICE_MULTIPLE_INSTANCE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
