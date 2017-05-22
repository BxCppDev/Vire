// vire/device/regular_2d_instance.cc
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

// Ourselves:
#include <vire/device/regular_2d_instance.h>

// Standard library:
#include <iostream>
#include <valarray>

// Third party:
// - Boost:
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/single_instance.h>

namespace vire {

  namespace device {

    // DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(regular_2d_instance, "vire::device::regular_2d_instance")

    VIRE_DEVICE_INSTANCE_REGISTRATION_IMPLEMENT(regular_2d_instance, "vire::device::regular_2d_instance");

    // static
    const int32_t regular_2d_instance::INVALID_DELTA_COORD;
    const int32_t regular_2d_instance::NO_DELTA_COORD;
    const int32_t regular_2d_instance::DEFAULT_DELTA_COORD;

    uint32_t regular_2d_instance::get_number_of_slots_0() const
    {
      return _number_of_slots_0_;
    }

    uint32_t regular_2d_instance::get_number_of_slots_1() const
    {
      return _number_of_slots_1_;
    }

    const slot & regular_2d_instance::get_first_slot() const
    {
      return _first_slot_;
    }

    int32_t regular_2d_instance::get_coord_step_0(int idim_) const
    {
      DT_THROW_IF(idim_ < 0 || idim_ >= 3,
                  std::domain_error, "Invalid dimension index [" << idim_ << "]!");
      return _coord_step_0_[idim_];
    }

    int32_t regular_2d_instance::get_coord_step_1(int idim_) const
    {
      DT_THROW_IF(idim_ < 0 || idim_ >= 3,
                  std::domain_error, "Invalid dimension index [" << idim_ << "]!");
      return _coord_step_1_[idim_];
    }

    void regular_2d_instance::set(const slot & first_slot_,
                                  size_t nslots0_,
                                  size_t nslots1_,
                                  int32_t dcoord0_0_,
                                  int32_t dcoord1_0_,
                                  int32_t dcoord0_1_,
                                  int32_t dcoord1_1_,
                                  int32_t dcoord0_2_,
                                  int32_t dcoord1_2_)
    {
      // Check first slot validity:
      DT_THROW_IF(!first_slot_.is_valid(), std::domain_error, "Invalid first slot " << first_slot_ << "!");
      // Check first slot dimension:
      DT_THROW_IF(first_slot_.is_dimension_0() || first_slot_.is_dimension_1(),
                  std::domain_error, "First slot " << first_slot_ << " with dimension < 2 is not allowed!");
      // Replication logic axis 0:
      DT_THROW_IF(dcoord0_0_ == INVALID_DELTA_COORD, std::domain_error,
                  "Invalid axis-0 coordinate step on dimension index 0!");
      DT_THROW_IF(dcoord0_1_ == INVALID_DELTA_COORD, std::domain_error,
                  "Invalid axis-0 coordinate step on dimension index 1!");
      DT_THROW_IF(dcoord1_0_ == INVALID_DELTA_COORD, std::domain_error,
                  "Invalid axis-1 coordinate step on dimension index 0!");
      DT_THROW_IF(dcoord1_1_ == INVALID_DELTA_COORD, std::domain_error,
                  "Invalid axis-1 coordinate step on dimension index 1!");
      if (first_slot_.is_dimension_3()) {
        DT_THROW_IF(dcoord0_2_ == INVALID_DELTA_COORD, std::domain_error,
                    "Invalid axis-0 coordinate step on dimension index 2!");
        DT_THROW_IF(dcoord1_2_ == INVALID_DELTA_COORD, std::domain_error,
                    "Invalid axis-1 coordinate step on dimension index 2!");
      }
      std::valarray<int> s0(3);
      std::valarray<int> daxis0(3);
      std::valarray<int> daxis1(3);
      s0[slot::COORDINATE_INDEX_0] = first_slot_.get_coordinate(slot::COORDINATE_INDEX_0);
      s0[slot::COORDINATE_INDEX_1] = first_slot_.get_coordinate(slot::COORDINATE_INDEX_1);
      daxis0[slot::COORDINATE_INDEX_0] = dcoord0_0_;
      daxis0[slot::COORDINATE_INDEX_1] = dcoord0_1_;
      daxis1[slot::COORDINATE_INDEX_0] = dcoord1_0_;
      daxis1[slot::COORDINATE_INDEX_1] = dcoord1_1_;
      if (first_slot_.get_dimension() == space::DIM_3) {
        s0[slot::COORDINATE_INDEX_2] = first_slot_.get_coordinate(2);
        daxis0[slot::COORDINATE_INDEX_2] = dcoord0_2_;
        daxis1[slot::COORDINATE_INDEX_2] = dcoord1_2_;
      }
      // std::cerr << "DEVEL: s0  = " << '(' << s0[0] << ',' << s0[1] << ',' << s0[2] << ')' << std::endl;
      // std::cerr << "DEVEL: daxis0  = " << '(' << daxis0[0] << ',' << daxis0[1] << ',' << daxis0[2] << ')' << std::endl;
      // std::cerr << "DEVEL: daxis1  = " << '(' << daxis1[0] << ',' << daxis1[1] << ',' << daxis1[2] << ')' << std::endl;
      // std::cerr << "DEVEL: nslots0 = " << nslots0_ << std::endl;
      // std::cerr << "DEVEL: nslots1 = " << nslots1_ << std::endl;
      /*
       *   s0                   s1
       *    *-->*-->*-->*-->*-->*
       *    :                   :
       *    v                   v
       *    *-->*-->*-->*-->*-->*
       *    :                   :
       *    v                   v
       *    *-->*-->*-->*-->*-->*
       *   s2                    s12
       */
      std::valarray<int> d0(3);
      std::valarray<int> d1(3);
      d0 = daxis0 * (int) (nslots0_ - 1);
      // std::cerr << "DEVEL: d0  = " << '(' << d0[0] << ',' << d0[1] << ',' << d0[2] << ')' << std::endl;
      d1 = daxis1 * (int) (nslots1_ - 1);
      // std::cerr << "DEVEL: d1  = " << '(' << d1[0] << ',' << d1[1] << ',' << d1[2] << ')' << std::endl;
      std::valarray<int> s1(3);
      s1 = s0 + d0;
      std::valarray<int> s2(3);
      s2 = s0 + d1;
      std::valarray<int> s12(3);
      s12 = s0 + d0 + d1;
      // std::cerr << "DEVEL: s1  = " << '(' << s1[0] << ',' << s1[1] << ',' << s1[2] << ')' << std::endl;
      // std::cerr << "DEVEL: s2  = " << '(' << s2[0] << ',' << s2[1] << ',' << s2[2] << ')' << std::endl;
      // std::cerr << "DEVEL: s12 = " << '(' << s12[0] << ',' << s12[1] << ',' << s12[2] << ')' << std::endl;
      int32_t last_coord0 = s1[slot::COORDINATE_INDEX_0];
      int32_t last_coord1 = s1[slot::COORDINATE_INDEX_1];
      int32_t last_coord2 = s1[slot::COORDINATE_INDEX_2];
      DT_THROW_IF(last_coord0 < 0, std::domain_error, "Invalid regular 2D instance!");
      DT_THROW_IF(last_coord1 < 0, std::domain_error, "Invalid regular 2D instance!");
      DT_THROW_IF(last_coord2 < 0, std::domain_error, "Invalid regular 2D instance!");
      last_coord0 = s2[slot::COORDINATE_INDEX_0];
      last_coord1 = s2[slot::COORDINATE_INDEX_1];
      last_coord2 = s2[slot::COORDINATE_INDEX_2];
      DT_THROW_IF(last_coord0 < 0, std::domain_error, "Invalid regular 2D instance!");
      DT_THROW_IF(last_coord1 < 0, std::domain_error, "Invalid regular 2D instance!");
      DT_THROW_IF(last_coord2 < 0, std::domain_error, "Invalid regular 2D instance!");
      last_coord0 = s12[slot::COORDINATE_INDEX_0];
      last_coord1 = s12[slot::COORDINATE_INDEX_1];
      last_coord2 = s12[slot::COORDINATE_INDEX_2];
      DT_THROW_IF(last_coord0 < 0, std::domain_error, "Invalid regular 2D instance!");
      DT_THROW_IF(last_coord1 < 0, std::domain_error, "Invalid regular 2D instance!");
      DT_THROW_IF(last_coord2 < 0, std::domain_error, "Invalid regular 2D instance!");

      DT_THROW_IF(first_slot_.get_dimension() == space::DIM_2
                  && dcoord0_0_ == NO_DELTA_COORD
                  && dcoord0_1_ == NO_DELTA_COORD,
                  std::domain_error,
                  "Null axis-0 coordinate steps for dimension 2 slot!");

      DT_THROW_IF(first_slot_.get_dimension() == space::DIM_3
                  && dcoord0_0_ == NO_DELTA_COORD
                  && dcoord0_1_ == NO_DELTA_COORD
                  && dcoord0_2_ == NO_DELTA_COORD,
                  std::domain_error,
                  "Null axis-0 coordinate steps for dimension 3 slot!");

      DT_THROW_IF(first_slot_.get_dimension() == space::DIM_2
                  && dcoord1_0_ == NO_DELTA_COORD
                  && dcoord1_1_ == NO_DELTA_COORD,
                  std::domain_error,
                  "Null axis-1 coordinate steps for dimension 2 slot!");

      DT_THROW_IF(first_slot_.get_dimension() == space::DIM_3
                  && dcoord1_0_ == NO_DELTA_COORD
                  && dcoord1_1_ == NO_DELTA_COORD
                  && dcoord1_2_ == NO_DELTA_COORD,
                  std::domain_error,
                  "Null axis-1 coordinate steps for dimension 3 slot!");

      // Set attributes:
      _first_slot_ = first_slot_;
      _number_of_slots_0_ = nslots0_;
      _number_of_slots_1_ = nslots1_;
      _coord_step_0_[slot::COORDINATE_INDEX_0] = dcoord0_0_;
      _coord_step_0_[slot::COORDINATE_INDEX_1] = dcoord0_1_;
      _coord_step_1_[slot::COORDINATE_INDEX_0] = dcoord1_0_;
      _coord_step_1_[slot::COORDINATE_INDEX_1] = dcoord1_1_;
      if (_first_slot_.get_dimension() == space::DIM_3) {
        _coord_step_0_[slot::COORDINATE_INDEX_2] = dcoord0_2_;
        _coord_step_1_[slot::COORDINATE_INDEX_2] = dcoord1_2_;
      }
      _s0_ = s0;
      _daxis0_ = daxis0;
      _daxis1_ = daxis1;
      return;
    }

    void regular_2d_instance::_set_defaults()
    {
      _number_of_slots_0_ = 0;
      _number_of_slots_1_ = 0;
      _coord_step_0_[slot::COORDINATE_INDEX_0] = INVALID_DELTA_COORD;
      _coord_step_0_[slot::COORDINATE_INDEX_1] = INVALID_DELTA_COORD;
      _coord_step_0_[slot::COORDINATE_INDEX_2] = INVALID_DELTA_COORD;
      _coord_step_1_[slot::COORDINATE_INDEX_0] = INVALID_DELTA_COORD;
      _coord_step_1_[slot::COORDINATE_INDEX_1] = INVALID_DELTA_COORD;
      _coord_step_1_[slot::COORDINATE_INDEX_2] = INVALID_DELTA_COORD;
      return;
    }

    regular_2d_instance::regular_2d_instance()
    {
      _set_defaults();
      return;
    }

    regular_2d_instance::~regular_2d_instance()
    {
      return;
    }

    bool regular_2d_instance::is_valid() const
    {
      if (_number_of_slots_0_ == 0) {
        return false;
      }
      if (_number_of_slots_1_ == 0) {
        return false;
      }
      if (!_first_slot_.is_valid()) {
        return false;
      }
      if (_first_slot_.is_dimension_0()) {
        return false;
      }
      if (_first_slot_.is_dimension_1()) {
        return false;
      }
      if (_coord_step_0_[slot::COORDINATE_INDEX_0] == INVALID_DELTA_COORD) {
        return false;
      }
      if (_coord_step_1_[slot::COORDINATE_INDEX_0] == INVALID_DELTA_COORD) {
        return false;
      }
      if (_first_slot_.is_dimension_2()) {
        if (_coord_step_0_[slot::COORDINATE_INDEX_0] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_0_[slot::COORDINATE_INDEX_1] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_1_[slot::COORDINATE_INDEX_0] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_1_[slot::COORDINATE_INDEX_1] == INVALID_DELTA_COORD) {
          return false;
        }
      } else if (_first_slot_.is_dimension_3()) {
        if (_coord_step_0_[slot::COORDINATE_INDEX_0] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_0_[slot::COORDINATE_INDEX_1] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_0_[slot::COORDINATE_INDEX_2] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_1_[slot::COORDINATE_INDEX_0] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_1_[slot::COORDINATE_INDEX_1] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_1_[slot::COORDINATE_INDEX_2] == INVALID_DELTA_COORD) {
          return false;
        }
      }
      return true;
    }

    void regular_2d_instance::reset()
    {
      _s0_.resize(0);
      _daxis0_.resize(0);
      _daxis1_.resize(0);
      _first_slot_.reset();
      _set_defaults();
      return;
    }

    space::dimension_type regular_2d_instance::get_dimension() const
    {
      return _first_slot_.get_dimension();
    }

    size_t regular_2d_instance::get_number_of_items() const
    {
      return _number_of_slots_0_ * _number_of_slots_1_;
    }

    void regular_2d_instance::fetch_slot(int item_, slot & s_) const
    {
      DT_THROW_IF((item_ < 0) || (item_ >= (int) get_number_of_items()), std::domain_error,
                  "Invalid item index '" << item_ << "' !");
      slot s;
      s.set_dimension(_first_slot_.get_dimension());
      int32_t ii, jj;
      ii = item_ % _number_of_slots_0_;
      jj = item_ / _number_of_slots_0_;
      std::valarray<int> pos = _s0_ + ii * _daxis0_ + jj * _daxis1_;
      s.set_coordinate(slot::COORDINATE_INDEX_0, pos[slot::COORDINATE_INDEX_0]);
      s.set_coordinate(slot::COORDINATE_INDEX_1, pos[slot::COORDINATE_INDEX_1]);
      if (_first_slot_.get_dimension() == space::DIM_3) {
        s.set_coordinate(slot::COORDINATE_INDEX_2, pos[slot::COORDINATE_INDEX_2]);
      }
      s_ = s;
      return;
    }

    void regular_2d_instance::tree_dump(std::ostream & out_,
                                        const std::string & title_,
                                        const std::string & indent_,
                                        bool inherit_) const
    {
      this->i_instance::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "First slot = ";
      if (_first_slot_.is_valid()) {
        out_ << _first_slot_;
      } else {
        out_ << "<none>";
      }
      out_  << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Number of slots (axis 0) = ";
      if (_number_of_slots_0_ > 0) {
        out_ << _number_of_slots_0_;
      } else {
        out_ << "<none>";
      }
      out_  << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Number of slots (axis 1) = ";
      if (_number_of_slots_1_ > 0) {
        out_ << _number_of_slots_1_;
      } else {
        out_ << "<none>";
      }
      out_  << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Coordinate step (axis 0) = ";
      if (!is_valid()) {
        out_ << "<none>";
      } else {
        out_ << '(';
        for (int i = 0; i < (int) _first_slot_.get_dimension(); i++) {
          if (i > 0) {
            out_ << ',';
          }
          out_ << _coord_step_0_[i];
        }
        out_ << ')';
      }
      out_  << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Coordinate step (axis 1) = ";
      if (!is_valid()) {
        out_ << "<none>";
      } else {
        out_ << '(';
        for (int i = 0; i < (int) _first_slot_.get_dimension(); i++) {
          if (i > 0) {
            out_ << ',';
          }
          out_ << _coord_step_1_[i];
        }
        out_ << ')';
      }
      out_  << std::endl;

      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_,
                              const regular_2d_instance & ai_)
    {
      std::string str;
      regular_2d_instance::to_string(str, ai_);
      out_ << str;
      return out_;
    }

    // static
    bool regular_2d_instance::from_string(const std::string & from_, regular_2d_instance & ri_)
    {
      return ri_.from_string(from_);
    }

    // virtual
    bool regular_2d_instance::from_string(const std::string & from_)
    {
      namespace qi = boost::spirit::qi;
      reset();
      uint32_t nslots0 = 0;
      uint32_t nslots1 = 0;
      std::string first_slot_token;
      std::vector<int32_t> coord_steps0;
      std::vector<int32_t> coord_steps1;
      std::string::const_iterator str_iter = from_.begin();
      std::string::const_iterator end_iter = from_.end();
      bool res = qi::phrase_parse(str_iter,
                                  end_iter,
                                  '['
                                  >> ('!' |
                                      (qi::lit("nslots0") >> '=' >> qi::uint_ >> ';'
                                       >> qi::lit("nslots1") >> '=' >> qi::uint_ >> ';'
                                       >> qi::lit("first_slot") >> '=' >> (+~qi::char_(";]")) >> ';'
                                       >> qi::lit("step0") >> '='  >> '(' >> (qi::int_ % ',') >> ')' >> ';'
                                       >> qi::lit("step1") >> '='  >> '(' >> (qi::int_ % ',') >> ')')
                                      )
                                  >> ']',
                                  qi::space,
                                  nslots0, nslots1, first_slot_token, coord_steps0, coord_steps1);
      if (!res || str_iter != end_iter) {
        return false;
      }
      try {
        if (nslots0 == 0 || nslots1 == 0 ) {
          if (nslots0 == 0 && nslots1 == 0
              && first_slot_token.empty()
              && coord_steps0.size() == 0
              && coord_steps1.size() == 0) {
            return true;
          }
          return false;
        }
        slot fs;
        if (!fs.from_string(first_slot_token)) {
          DT_LOG_ERROR(get_logging(), "First slot parse error in '" << first_slot_token << "'!");
          return false;
        }
        if (fs.get_dimension() == space::DIM_1) {
          DT_LOG_ERROR(get_logging(), "Invalid dimension 1!");
          return false;
        }
        if (fs.get_dimension() != coord_steps0.size()) {
          DT_LOG_ERROR(get_logging(), "First slot dimension does not match slot step 0!");
          return false;
        }
        if (fs.get_dimension() != coord_steps1.size()) {
          DT_LOG_ERROR(get_logging(), "First slot dimension does not match slot step 1!");
          return false;
        }
        if (fs.get_dimension() == space::DIM_2) {
          this->set(fs, nslots0, nslots1,
                  coord_steps0[0], coord_steps1[0],
                  coord_steps0[1], coord_steps1[1]);
        }
        if (fs.get_dimension() == space::DIM_3) {
          this->set(fs, nslots0, nslots1,
                  coord_steps0[slot::COORDINATE_INDEX_0], coord_steps1[slot::COORDINATE_INDEX_0],
                  coord_steps0[slot::COORDINATE_INDEX_1], coord_steps1[slot::COORDINATE_INDEX_1],
                  coord_steps0[slot::COORDINATE_INDEX_2], coord_steps1[slot::COORDINATE_INDEX_2]);
        }
      } catch (std::exception & error) {
        DT_LOG_ERROR(get_logging(), error.what());
        this->reset();
        return false;
      }
      return true;
    }

    // static
    void regular_2d_instance::to_string(std::string & to_, const regular_2d_instance & ri_)
    {
      ri_.to_string(to_);
      return;
    }

    void regular_2d_instance::to_string(std::string & to_) const
    {
      namespace karma = boost::spirit::karma;
      uint32_t nslots0 = this->get_number_of_slots_0();
      uint32_t nslots1 = this->get_number_of_slots_1();
      const slot & first_slot = this->get_first_slot();
      std::vector<int32_t> coord_steps0;
      std::vector<int32_t> coord_steps1;
      if (space::at_least_dimension_1(this->get_dimension())) {
        coord_steps0.push_back(this->get_coord_step_0(0));
        coord_steps1.push_back(this->get_coord_step_1(0));
      }
      if (space::at_least_dimension_2(this->get_dimension())) {
        coord_steps0.push_back(this->get_coord_step_0(1));
        coord_steps1.push_back(this->get_coord_step_1(1));
      }
      if (space::at_least_dimension_3(this->get_dimension())) {
        coord_steps0.push_back(this->get_coord_step_0(2));
        coord_steps1.push_back(this->get_coord_step_1(2));
      }
      to_.clear();
      // Format:
      //
      // * [!] represent the invalid value
      // * [nslots0=4;nslots1=3;first_slot=(2:0,4);step0=(2,0),step1=(0,1)]
      //   This represents the following 2D mesh of 2D slots:
      //   {
      //    (2:0,4) ; (2:2,4); (2:4,4); (2:6,4)
      //    (2:0,5) ; (2:2,5); (2:4,5); (2:6,5)
      //    (2:0,6) ; (2:2,6); (2:4,6); (2:6,6)
      //   }
      //
      std::string first_slot_token;
      first_slot.to_string(first_slot_token);
      std::back_insert_iterator<std::string> out(to_);
      boost::spirit::karma::generate(out,
                                     '['
                                     << ((karma::eps(!this->is_valid())
                                          << '!' | (
                                                    karma::lit("nslots0")     << '=' << karma::uint_   << ';'
                                                    << karma::lit("nslots1")     << '=' << karma::uint_   << ';'
                                                    << karma::lit("first_slot") << '=' << karma::string  << ';'
                                                    << karma::lit("step0")       << '=' << '(' << (karma::int_ % ',') << ')' << ';'
                                                    << karma::lit("step1")       << '=' << '(' << (karma::int_ % ',') << ')'
                                                    )
                                          )
                                         )
                                     << ']',
                                     nslots0, nslots1, first_slot_token, coord_steps0, coord_steps1);

      return;
    }

  } // namespace device

} // namespace vire
