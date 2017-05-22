//! \file  vire/device/regular_1d_instance.h
//! \brief Regular 1D instance object
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

#ifndef VIRE_DEVICE_REGULAR_1D_INSTANCE_H
#define VIRE_DEVICE_REGULAR_1D_INSTANCE_H

// This project:
#include <vire/device/i_instance.h>
#include <vire/device/slot.h>

namespace vire {

  namespace device {

    //! \brief The regular 1D instance of a collection of daughter objects embedded in a mother object
    //!        with respect to the mother reference layout
    //!
    //! The regular 1D instance object is associated to a set of regularly spaced distinct 'slots'
    //! available in the mother object associated to some instantiation space of dimension 1, 2 or 3.
    //!
    //! Regular 1D instance in a 1 dimension instantiation space:
    //! @code
    //! +--------------------------------------+
    //! |  0  1  2  3  4  5  6  7  8  9  10 11 | first_slot      == (1:3)
    //! | [.][.][.][*][.][*][.][*][.][*][.][.] | number_of_slots == 4
    //! |                                      | coord step      == 2
    //! +--------------------------------------+
    //! @endcode
    //!
    //! Regular 1D instance in a 2 dimension instantiation space:
    //! @code
    //! +-------------------------------------+
    //! |    0  1  2  3  4  5  6  7  8  9  10 | first_slot      == (2:3,2)
    //! | 3 [.][.][.][.][.][.][.][.][.][.][.] | number_of_slots == 3
    //! | 2 [.][.][.][*][.][.][.][.][.][.][.] | coord step      == (1,-1)
    //! | 1 [.][.][.][.][*][.][.][.][.][.][.] |
    //! | 0 [.][.][.][.][.][*][.][.][.][.][.] |
    //! |                                     |
    //! +-------------------------------------+
    //! @endcode
    //!
    class regular_1d_instance : public i_instance
    {

    public:

      static const int32_t INVALID_DELTA_COORD = 0x7FFFFFFF;
      static const int32_t NO_DELTA_COORD      = 0;
      static const int32_t DEFAULT_DELTA_COORD = +1;

      // Return the number of slots
      uint32_t get_number_of_slots() const;

      // Return the first slot
      const slot & get_first_slot() const;

      // Return the coord step
      int32_t get_coord_step(int idim_) const;

      //! Set
      void set(const slot & first_slot_,
               size_t nslots_,
               int32_t dslot0_ = DEFAULT_DELTA_COORD,
               int32_t dslot1_ = INVALID_DELTA_COORD,
               int32_t dslot2_ = INVALID_DELTA_COORD);

      //! Constructor
      regular_1d_instance();

      //! Destructor
      virtual ~regular_1d_instance();

      //! Check validity
      virtual bool is_valid() const;

      //! Reset
      virtual void reset();

      //! Return the dimension of the instance object
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
      static bool from_string(const std::string &, regular_1d_instance &);

      //! Convert to a description string
      static void to_string(std::string &, const regular_1d_instance &);

      //! Print
      friend std::ostream & operator<<(std::ostream & out_,
                                       const regular_1d_instance & ai_);

    protected:

      //! Set default values to attributes
      void _set_defaults();

    private:

      slot     _first_slot_;      //!< First slot index of the object
      uint32_t _number_of_slots_; //!< Number of slots
      int32_t  _coord_step_[3];   //!< Coordinate step between instantiation (default: (+1[,+1[,+1]]))

      //! Registration of a system factory
      VIRE_DEVICE_INSTANCE_REGISTRATION_INTERFACE(regular_1d_instance)

      // //! Serialization interface
      // DATATOOLS_SERIALIZATION_DECLARATION()

      //! Reflection interface
      DR_CLASS_RTTI()

    };

  } // namespace device

} // namespace vire

// Activate reflection layer for the vire::device:regular_1d_instance class :
DR_CLASS_INIT(::vire::device::regular_1d_instance)

#endif // VIRE_DEVICE_REGULAR_1D_INSTANCE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
