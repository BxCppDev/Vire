//! \file  vire/device/regular_2d_instance.h
//! \brief Regular 2D instance object
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

#ifndef VIRE_DEVICE_REGULAR_2D_INSTANCE_H
#define VIRE_DEVICE_REGULAR_2D_INSTANCE_H

// Standard library:
#include <valarray>

// This project:
#include <vire/device/i_instance.h>
#include <vire/device/slot.h>

namespace vire {

  namespace device {

    //! \brief The regular 2D instance of a collection of daughter objects embedded in a mother object
    //!        with respect to the mother reference layout
    //!
    //! The regular 2D instance object is associated to a set of regularly spaced distinct 'slots'
    //! available in the mother object associated to some instantiation space of dimension 2 or 3.
    //!
    //! Regular 2D instance in a 2 dimension instantiation space:
    //! @code
    //! +-------------------------------------+
    //! |    0  1  2  3  4  5  6  7  8  9  10 | first_slot        == (2:3,2)
    //! | 3 [.][.][.][.][.][.][.][.][.][.][.] | number_of_slots_0 == 4
    //! | 2 [.][.][.][*][*][*][*][.][.][.][.] | number_of_slots_1 == 3
    //! | 1 [.][.][.][*][*][*][*][.][.][.][.] | coord_step_0      == (1,0)
    //! | 0 [.][.][.][*][*][*][*][.][.][.][.] | coord_step_1      == (0,1)
    //! |                                     |
    //! +-------------------------------------+
    //! @endcode
    //!
    class regular_2d_instance : public i_instance
    {

    public:

      static const int32_t INVALID_DELTA_COORD = 0x7FFFFFFF;
      static const int32_t NO_DELTA_COORD      = 0;
      static const int32_t DEFAULT_DELTA_COORD = +1;

      // Return the number of slots on replication logic axis 0
      uint32_t get_number_of_slots_0() const;

      // Return the number of slots on replication logic axis 1
      uint32_t get_number_of_slots_1() const;

      // Return the first slot
      const slot & get_first_slot() const;

      // Return the coord step on replication logic axis 0
      int32_t get_coord_step_0(int idim_) const;

      // Return the coord step on replication logic axis 1
      int32_t get_coord_step_1(int idim_) const;

      //! Set
      void set(const slot & first_slot_,
               size_t nslots0_,
               size_t nslots1_,
               int32_t dslot0_0_ = DEFAULT_DELTA_COORD,
               int32_t dslot1_0_ = DEFAULT_DELTA_COORD,
               int32_t dslot0_1_ = INVALID_DELTA_COORD,
               int32_t dslot1_1_ = INVALID_DELTA_COORD,
               int32_t dslot0_2_ = INVALID_DELTA_COORD,
               int32_t dslot1_2_ = INVALID_DELTA_COORD);

      //! Constructor
      regular_2d_instance();

      //! Destructor
      virtual ~regular_2d_instance();

      //! Check validity
      virtual bool is_valid() const;

      //! Reset
      virtual void reset();

      //! Return the dimension of the instance object
      virtual space::dimension_type get_dimension() const;

      //! Return the number of instance items
      virtual size_t get_number_of_items() const;

      //! Compute the slot at given item rank/index
      //!
      //!
      //! @code
      //!  +-----+-----+-----+-----+
      //!  |  [8]|  [9]| [10]| [11]|
      //!  |(0,2)|(1,2)|(2,2)|(3,2)|
      //!  +-----+-----+-----+-----+
      //!  |  [4]|  [5]|  [6]|  [7]|
      //!  |(0,1)|(1,1)|(2,1)|(3,1)|
      //!  +-----+-----+-----+-----+
      //!  |  [0]|  [1]|  [2]|  [3]|
      //!  |(0,0)|(1,0)|(2,0)|(3,0)|
      //!  +-----+-----+-----+-----+
      //! @endcode
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
      static bool from_string(const std::string &, regular_2d_instance &);

      //! Convert to a description string
      static void to_string(std::string &, const regular_2d_instance &);

      //! Print
      friend std::ostream & operator<<(std::ostream & out_,
                                       const regular_2d_instance & ai_);

    protected:

      //! Set default values to attributes
      void _set_defaults();

    private:

      slot     _first_slot_;        //!< First slot index of the object
      uint32_t _number_of_slots_0_; //!< Number of slots for replication logic axis 0
      uint32_t _number_of_slots_1_; //!< Number of slots for replication logic axis 1
      int32_t  _coord_step_0_[3];   //!< Coordinate step between instantiation (default: (+1 ,0[,?]))
      int32_t  _coord_step_1_[3];   //!< Coordinate step between instantiation (default: (0 ,+1[,?]))


      // Working data:
      //
      //
      //   +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +
      //
      //   +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +
      //
      //   +  +  +  +  +  +  + (+) + (+) + (+) + (+) + (+) + (+)
      //
      //   +  +  +  +  +  + (+) + (+) + (+) + (+) + (+) + (+) +
      //
      //   +  +  +  +  + (+) + (+) + (+) + (+) + (+) + (+) +  +
      //           (1,1) /
      //   +  +  +  + [+]-+-(+) + (+) + (+) + (+) + (+) +  +  +
      //                (2,0)
      //   +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +
      //
      //   +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +  +
      //
      //
      //
      std::valarray<int> _s0_;     //!< Vector of coordinates of the first slot
      std::valarray<int> _daxis0_; //!< Vector of coordinates of the axis-0 step
      std::valarray<int> _daxis1_; //!< Vector of coordinates of the axis-1 step

      //! Registration of a system factory
      VIRE_DEVICE_INSTANCE_REGISTRATION_INTERFACE(regular_2d_instance)

      // //! Serialization interface
      // DATATOOLS_SERIALIZATION_DECLARATION()

      //! Reflection interface
      DR_CLASS_RTTI()

    };

  } // namespace device

} // namespace vire

// Activate reflection layer for the vire::device:regular_2d_instance class :
DR_CLASS_INIT(::vire::device::regular_2d_instance)

#endif // VIRE_DEVICE_REGULAR_2D_INSTANCE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
