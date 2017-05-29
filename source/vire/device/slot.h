//! \file  vire/device/slot.h
//! \brief A slot is a discrete positioning in some logic discrete reference frame
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

#ifndef VIRE_DEVICE_SLOT_H
#define VIRE_DEVICE_SLOT_H

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/bit_mask.h>
#ifndef Q_MOC_RUN
#include <bayeux/datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// This project:
#include <vire/device/space.h>

namespace vire {

  namespace device {

    //! \brief A slot describes the logic coordinates (multiplets of positive
    //!        integral values) of an object positioned in
    //!        the logic/discrete instantation space associated to a mother container.
    //!        The instantation space is of arbitrary dimension from dimension 0 to 3.
    //!
    //! Example of an instantiation space of dimension 0: a unique instantiation slot is available,
    //! no coordinate is used to identify the slot.
    //!
    //! @code
    //!   +
    //!  (0)
    //! @endcode
    //!
    //!
    //! Example of an instantiation space of dimension 1: each instantiation slot in this space is
    //! identified through an unique coordinate along the logic/dicrete instantiation axis
    //! @code
    //!   + - - + - - + - - + - - + - - + - -//- - + - -//- -> instantiation axis (dimension 1)
    //! (1:0) (1:1) (1:2) (1:3) (1:4) (1:5)      (1:k)
    //! @endcode
    //!
    //!
    //!
    //! Example of an instantiation space of dimension 2: each instantiation slot in this space is
    //! identified through an unique pair of coordinates along both logic/dicrete instantiation axis
    //! @code
    //!   ^ instantiation axis-1 (dimension 2)
    //!   |
    //!   |
    //!   + - - - - + - - - - + - - - - + - - - - +
    //! (2:2.0)   (2:2.1)   (2:2.2)   (2:2.3)   (2:2.4)
    //!   |         |         |         |         |
    //!   + - - - - + - - - - + - - - - + - - - - +
    //! (2:1.0)   (2:1.1)   (2:1.2)   (2:1.3)   (2:1.4)
    //!   |         |         |         |         |
    //!   + - - - - + - - - - + - - - - + - - - - + - - - - > instantiation axis-0 (dimension 2)
    //! (2:0.0)   (2:0.1)   (2:0.2)   (2:0.3)   (2:0.4)
    //! @endcode
    //!
    class slot
      : public datatools::i_serializable
      , public datatools::i_tree_dumpable
    {

    public:

      //! The index of the first coordinate in a instantation space of dimension >= 1
      static const int32_t COORDINATE_INDEX_0 = 0;

      //! The index of the second coordinate in a instantation space of dimension >= 2
      static const int32_t COORDINATE_INDEX_1 = 1;

      //! The index of the second coordinate in a instantation space of dimension 3
      static const int32_t COORDINATE_INDEX_2 = 2;

      //! Chec if dimension is set
      bool has_dimension() const;

      //! Set the dimension
      void set_dimension(space::dimension_type dim_);

      //! Return the dimension of the coordinates
      space::dimension_type get_dimension() const;

      //! Reset the dimension
      void reset_dimension();

      //! Check if slot is dimensionless
      bool is_dimensionless() const;

      //! Check if slot is of dimension 0
      bool is_dimension_0() const;

      //! Check if slot is of dimension 1
      bool is_dimension_1() const;

      //! Check if slot is of dimension 2
      bool is_dimension_2() const;

      //! Check if slot is of dimension 3
      bool is_dimension_3() const;

      //! Return the maximum coordinate index
      int32_t get_max_coordinate_index() const;

      //! Check if coordinate is valid at given dimension
      bool has_valid_coordinate(unsigned int idim_) const;

      //! Return the coordinate value at given dimension
      uint32_t get_coordinate(unsigned int idim_) const;

      //! Set the coordinate value at given dimension
      void set_coordinate(unsigned int idim_, uint32_t value_);

      //! Reset  the coordinates
      void reset_coordinates();

      //! Fetch the coordinates
      void fetch_coordinates(std::vector<uint32_t> & coordinates_, bool clear_ = true) const;

      //! Return coordinates
      std::vector<uint32_t> get_coordinates() const;

      // //! Test
      // const std::vector<uint32_t> & gc() const;

      //! Set the dimension and indexes
      void set(space::dimension_type dimension_,
               uint32_t i_ = space::COORDINATE_INVALID,
               uint32_t j_ = space::COORDINATE_INVALID,
               uint32_t k_ = space::COORDINATE_INVALID);

      //! Default constructor
      slot();

      //! Constructor
      slot(space::dimension_type dimension_,
           uint32_t i_ = space::COORDINATE_INVALID,
           uint32_t j_ = space::COORDINATE_INVALID,
           uint32_t k_ = space::COORDINATE_INVALID);

      //! Check validity
      bool is_valid() const;

      //! Reset
      void reset();

      //! Make a slot of dimension 0
      void make_dimension_0(bool valid_ = true);

      //! Make a slot of dimension 1
      void make_dimension_1(uint32_t i_);

      //! Make a slot of dimension 2
      void make_dimension_2(uint32_t i_, uint32_t j_);

      //! Make a slot of dimension 3
      void make_dimension_3(uint32_t i_, uint32_t j_, uint32_t k_);

      //! Smart print
      virtual void tree_dump(std::ostream & out_         = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_          = false) const;

      //! Parse from a string
      bool from_string(const std::string &);

      //! Encode to string
      void to_string(std::string &) const;

      //! Return string
      std::string str() const;

      //! Parse from a description string
      static bool string_to_slot(const std::string &, slot &);

      //! Convert to a description string
      static void slot_to_string(const slot &, std::string &);

      //! Print
      friend std::ostream & operator<<(std::ostream & out_,
                                       const slot & s_);

      //! Comparison operator
      bool operator==(const slot & slot_) const;

    private:

      space::dimension_type _dimension_; //!< Dimension of the instantiation space
      uint32_t _coordinates_[3];         //!< Coordinates/indexes with respect to a multidimensional discrete reference frame

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION()

      //! Reflection interface
      DR_CLASS_RTTI()

    };

  } // namespace device

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the vire::device::slot class :
DR_CLASS_INIT(::vire::device::slot)
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_SLOT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
