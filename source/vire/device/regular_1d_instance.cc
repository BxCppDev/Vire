// vire/device/regular_1d_instance.cc
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
#include <vire/device/regular_1d_instance.h>

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

    // DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(regular_1d_instance, "vire::device::regular_1d_instance")

    VIRE_DEVICE_INSTANCE_REGISTRATION_IMPLEMENT(regular_1d_instance, "vire::device::regular_1d_instance");

    // static
    const int32_t regular_1d_instance::INVALID_DELTA_COORD;
    const int32_t regular_1d_instance::NO_DELTA_COORD;
    const int32_t regular_1d_instance::DEFAULT_DELTA_COORD;

    uint32_t regular_1d_instance::get_number_of_slots() const
    {
      return _number_of_slots_;
    }

    const slot & regular_1d_instance::get_first_slot() const
    {
      return _first_slot_;
    }

    int32_t regular_1d_instance::get_coord_step(int idim_) const
    {
      DT_THROW_IF(idim_ < 0 || idim_ >= 3,
                  std::domain_error, "Invalid dimension index [" << idim_ << "]!");
      return _coord_step_[idim_];
    }

    void regular_1d_instance::set(const slot & first_slot_,
                                  size_t nslots_,
                                  int32_t dcoord0_,
                                  int32_t dcoord1_,
                                  int32_t dcoord2_
                                  )
    {
      // std::cerr << "DEVEL: regular_1d_instance::set: first_slot = " << first_slot_ << std::endl;
      // std::cerr << "DEVEL: regular_1d_instance::set: nslots     = " << nslots_ << std::endl;
      // std::cerr << "DEVEL: regular_1d_instance::set: dcoord0    = " << dcoord0_ << std::endl;
      // std::cerr << "DEVEL: regular_1d_instance::set: dcoord1    = " << dcoord1_ << std::endl;
      // std::cerr << "DEVEL: regular_1d_instance::set: dcoord2    = " << dcoord2_ << std::endl;
      DT_THROW_IF(!first_slot_.is_valid(), std::domain_error, "Invalid first slot " << first_slot_ << "!");
      // std::cerr << "DEVEL: regular_1d_instance::set: first_slot = " << first_slot_ << std::endl;
      DT_THROW_IF(first_slot_.is_dimension_0(),
                  std::domain_error, "First slot " << first_slot_ << " with dimension 0 is not allowed!");
      // std::cerr << "DEVEL: regular_1d_instance::set: first_slot = " << first_slot_ << std::endl;
      int d = static_cast<int>(_first_slot_.get_dimension());
      // std::cerr << "DEVEL: regular_1d_instance::set: d : " << d << std::endl;
      // std::cerr << "DEVEL: regular_1d_instance::set: first_slot = " << first_slot_ << std::endl;
      if (space::at_least_dimension_1(_first_slot_.get_dimension())) {
        // std::cerr << "DEVEL: regular_1d_instance::set: At least DIM_1 : " << _first_slot_.get_dimension() << std::endl;
        DT_THROW_IF(dcoord0_ == INVALID_DELTA_COORD, std::domain_error, "Invalid coordinate step on dimension index 0!");
        DT_THROW_IF(_first_slot_.get_dimension() == space::DIM_1
                    && dcoord0_ == NO_DELTA_COORD, std::domain_error, "Null coordinate step for dimension 1 slot!");
        int32_t last_coord = _first_slot_.get_coordinate(0) + (nslots_ - 1) * dcoord0_;
        DT_THROW_IF(last_coord < 0, std::domain_error, "Invalid regular 1D instance!");
      }
      if (space::at_least_dimension_2(_first_slot_.get_dimension())) {
        // std::cerr << "DEVEL: regular_1d_instance::set: At least DIM_2 : " << _first_slot_.get_dimension() << std::endl;
        DT_THROW_IF(dcoord1_ == INVALID_DELTA_COORD, std::domain_error, "Invalid coordinate step on dimension index 1!");
        DT_THROW_IF(_first_slot_.get_dimension() == space::DIM_2
                    && dcoord0_ == NO_DELTA_COORD
                    && dcoord1_ == NO_DELTA_COORD, std::domain_error, "Null coordinate steps for dimension 2 slot!");
        int32_t last_coord = _first_slot_.get_coordinate(1) + (nslots_ - 1) * dcoord1_;
        DT_THROW_IF(last_coord < 0, std::domain_error, "Invalid regular 1D instance!");
      }
      if (space::at_least_dimension_3(_first_slot_.get_dimension())) {
        // std::cerr << "DEVEL: regular_1d_instance::set: At least DIM_3 : " << _first_slot_.get_dimension() << std::endl;
        DT_THROW_IF(dcoord2_ == INVALID_DELTA_COORD, std::domain_error, "Invalid coordinate step on dimension index 2!");
        DT_THROW_IF(_first_slot_.get_dimension() == space::DIM_3
                    && dcoord0_ == NO_DELTA_COORD
                    && dcoord1_ == NO_DELTA_COORD
                    && dcoord2_ == NO_DELTA_COORD, std::domain_error, "Null coordinate steps for dimension 3 slot!");
        int32_t last_coord = _first_slot_.get_coordinate(2) + (nslots_ - 1) * dcoord2_;
        DT_THROW_IF(last_coord < 0, std::domain_error, "Invalid regular 1D instance!");
      }
      _first_slot_ = first_slot_;
      _number_of_slots_ = nslots_;
      _coord_step_[slot::COORDINATE_INDEX_0] = dcoord0_;
      if (_first_slot_.get_dimension() >= space::DIM_2) _coord_step_[slot::COORDINATE_INDEX_1] = dcoord1_;
      if (_first_slot_.get_dimension() >= space::DIM_3) _coord_step_[slot::COORDINATE_INDEX_2] = dcoord2_;
      return;
    }

    void regular_1d_instance::_set_defaults()
    {
      _number_of_slots_ = 0;
      _coord_step_[slot::COORDINATE_INDEX_0] = INVALID_DELTA_COORD;
      _coord_step_[slot::COORDINATE_INDEX_1] = INVALID_DELTA_COORD;
      _coord_step_[slot::COORDINATE_INDEX_2] = INVALID_DELTA_COORD;

      return;
    }

    regular_1d_instance::regular_1d_instance()
    {
      _set_defaults();
      return;
    }

    regular_1d_instance::~regular_1d_instance()
    {
      DT_LOG_TRACE_ENTERING(datatools::logger::PRIO_TRACE);
      reset();
      return;
    }

    bool regular_1d_instance::is_valid() const
    {
      if (_number_of_slots_ == 0) {
        return false;
      }
      if (!_first_slot_.is_valid()) {
        return false;
      }
      if (_first_slot_.is_dimension_1()) {
        if (_coord_step_[slot::COORDINATE_INDEX_0] == INVALID_DELTA_COORD) {
          return false;
        }
      } else if (_first_slot_.is_dimension_2()) {
        if (_coord_step_[slot::COORDINATE_INDEX_0] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_[slot::COORDINATE_INDEX_1] == INVALID_DELTA_COORD) {
          return false;
        }
      } else if (_first_slot_.is_dimension_3()) {
        if (_coord_step_[slot::COORDINATE_INDEX_0] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_[slot::COORDINATE_INDEX_1] == INVALID_DELTA_COORD) {
          return false;
        }
        if (_coord_step_[slot::COORDINATE_INDEX_2] == INVALID_DELTA_COORD) {
          return false;
        }
      }
      return true;
    }

    void regular_1d_instance::reset()
    {
      DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "Entering for address = [@" << this << "]");
      _first_slot_.reset();
      _set_defaults();
      return;
    }

    space::dimension_type regular_1d_instance::get_dimension() const
    {
      return _first_slot_.get_dimension();
    }

    size_t regular_1d_instance::get_number_of_items() const
    {
      // DT_LOG_TRACE_ENTERING(datatools::logger::PRIO_TRACE);
      return _number_of_slots_;
    }

    void regular_1d_instance::fetch_slot(int item_, slot & s_) const
    {
      DT_THROW_IF((item_ < 0) || (item_ >= (int) get_number_of_items()), std::domain_error,
                  "Invalid item index '" << item_ << "' !");
      slot s;
      s.set_dimension(_first_slot_.get_dimension());
      int32_t i, j, k;
      i = _first_slot_.get_coordinate(slot::COORDINATE_INDEX_0) + item_ * _coord_step_[slot::COORDINATE_INDEX_0];
      s.set_coordinate(slot::COORDINATE_INDEX_0, i);
      if (_first_slot_.get_dimension() >= space::DIM_2) {
        j = _first_slot_.get_coordinate(slot::COORDINATE_INDEX_1) + item_ * _coord_step_[slot::COORDINATE_INDEX_1];
        s.set_coordinate(slot::COORDINATE_INDEX_1, j);
        if (_first_slot_.get_dimension() >= space::DIM_3) {
          k = _first_slot_.get_coordinate(slot::COORDINATE_INDEX_2) + item_ * _coord_step_[slot::COORDINATE_INDEX_2];
          s.set_coordinate(slot::COORDINATE_INDEX_2, k);
        }
      }
      s_ = s;
      return;
    }

    void regular_1d_instance::tree_dump(std::ostream & out_,
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
           << "Number of slots = ";
      if (_number_of_slots_ > 0) {
        out_ << _number_of_slots_;
      } else {
        out_ << "<none>";
      }
      out_  << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Coordinate step = ";
      if (!is_valid()) {
        out_ << "<none>";
      } else {
        out_ << '(';
        for (int i = 0; i < (int) _first_slot_.get_dimension(); i++) {
          if (i > 0) {
            out_ << ',';
          }
          out_ << _coord_step_[i];
        }
        out_ << ')';
      }

      out_  << std::endl;

      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_,
                              const regular_1d_instance & ai_)
    {
      std::string str;
      regular_1d_instance::to_string(str, ai_);
      out_ << str;
      return out_;
    }

    // static
    bool regular_1d_instance::from_string(const std::string & from_, regular_1d_instance & ri_)
    {
      return ri_.from_string(from_);
    }

    // virtual
    bool regular_1d_instance::from_string(const std::string & from_)
    {
      DT_LOG_TRACE_ENTERING(datatools::logger::PRIO_TRACE);
      namespace qi = boost::spirit::qi;
      this->reset();
      uint32_t nslots = 0;
      std::string first_slot_token;
      std::vector<int32_t> coord_steps;
      std::string::const_iterator str_iter = from_.begin();
      std::string::const_iterator end_iter = from_.end();
      bool res = qi::phrase_parse(str_iter,
                                  end_iter,
                                  '['
                                  >> ('!' |
                                      (qi::lit("nslots") >> '=' >> qi::uint_ >> ';'
                                       >> qi::lit("first_slot") >> '=' >> (+~qi::char_(";]")) >> ';'
                                       >> qi::lit("step") >> '='  >> '(' >> (qi::int_ % ','))
                                      >> ')'
                                      )
                                  >> ']',
                                  qi::space,
                                  nslots, first_slot_token, coord_steps);
      if (!res || str_iter != end_iter) {
        return false;
      }
      try {
        if (nslots == 0 ) {
          if (first_slot_token.empty() && coord_steps.size() == 0) {
            return true;
          }
          return false;
        }
        slot fs;
        if (!fs.from_string(first_slot_token)) {
          return false;
        }
        if (fs.get_dimension() != coord_steps.size()) {
          return false;
        }
        if (fs.get_dimension() == space::DIM_1) {
          this->set(fs, nslots, coord_steps[0]);
        }
        if (fs.get_dimension() == space::DIM_2) {
          this->set(fs, nslots, coord_steps[0], coord_steps[1]);
        }
        if (fs.get_dimension() == space::DIM_3) {
          this->set(fs, nslots, coord_steps[0], coord_steps[1], coord_steps[2]);
        }
      } catch (std::exception & error) {
        DT_LOG_FATAL(datatools::logger::PRIO_FATAL, error.what());
        this->reset();
        return false;
      }
      DT_LOG_TRACE(datatools::logger::PRIO_TRACE, "Parsing was successful.");
      DT_LOG_TRACE_EXITING(datatools::logger::PRIO_TRACE);
      return true;
    }

    // static
    void regular_1d_instance::to_string(std::string & to_, const regular_1d_instance & ri_)
    {
      ri_.to_string(to_);
      return;
    }

    void regular_1d_instance::to_string(std::string & to_) const
    {
      namespace karma = boost::spirit::karma;
      uint32_t nslots = this->get_number_of_slots();
      const slot & first_slot = this->get_first_slot();
      std::vector<int32_t> coord_steps;
      if (space::at_least_dimension_1(this->get_dimension())) {
        coord_steps.push_back(this->get_coord_step(0));
      }
      if (space::at_least_dimension_2(this->get_dimension())) {
        coord_steps.push_back(this->get_coord_step(1));
      }
      if (space::at_least_dimension_3(this->get_dimension())) {
        coord_steps.push_back(this->get_coord_step(2));
      }
      to_.clear();
      // Format:
      //
      // * [!] represent the invalid value
      // * [nslots=3;first_slot=(2:0,9);step=(2,-1)]
      //   This represents the 2D slots: { (2:0,9) ;(2:2,8); (2:4,8) }
      //
      std::string first_slot_token;
      first_slot.to_string(first_slot_token);
      std::back_insert_iterator<std::string> out(to_);
      boost::spirit::karma::generate(out,
                                     '['
                                     << ((karma::eps(!this->is_valid())
                                          << '!' | (
                                                    karma::lit("nslots")     << '=' << karma::uint_   << ';'
                                                    << karma::lit("first_slot") << '=' << karma::string  << ';'
                                                    << karma::lit("step")       << '=' << '(' << (karma::int_ % ',') << ')'
                                                     )
                                          )
                                         )
                                     << ']',
                                     nslots, first_slot_token, coord_steps);

      return;
    }

  } // namespace device

} // namespace vire
