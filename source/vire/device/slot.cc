// vire/device/slot.cc
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
#include <vire/device/slot.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace device {

    // Serialization implementation
    DATATOOLS_SERIALIZATION_IMPLEMENTATION(slot, "vire::device::slot")

    // static
    const int32_t slot::COORDINATE_INDEX_0;
    const int32_t slot::COORDINATE_INDEX_1;
    const int32_t slot::COORDINATE_INDEX_2;

    bool slot::has_dimension() const
    {
      return _dimension_ != space::DIM_INVALID;
    }

    void slot::set_dimension(space::dimension_type dim_)
    {
      reset_coordinates();
      _dimension_ = dim_;
      return;
    }

    space::dimension_type slot::get_dimension() const
    {
      return _dimension_;
    }

    void slot::reset_dimension()
    {
      _dimension_ = space::DIM_INVALID;
      return;
    }

    bool slot::is_dimensionless() const
    {
      return _dimension_ == space::DIM_0;
    }

    bool slot::is_dimension_0() const
    {
      return _dimension_ == space::DIM_0;
    }

    bool slot::is_dimension_1() const
    {
      return _dimension_ == space::DIM_1;
    }

    bool slot::is_dimension_2() const
    {
      return _dimension_ == space::DIM_2;
    }

    bool slot::is_dimension_3() const
    {
      return _dimension_ == space::DIM_3;
    }

    int32_t slot::get_max_coordinate_index() const
    {
      switch (_dimension_) {
      case space::DIM_1: return 0;
      case space::DIM_2: return 1;
      case space::DIM_3: return 2;
      }
      return -1;
    }

    bool slot::has_valid_coordinate(unsigned int idim_) const
    {
      DT_THROW_IF(idim_ < COORDINATE_INDEX_0 || idim_ > get_max_coordinate_index(),
                  std::range_error,
                  "Invalid coordinate index [" << idim_ << "]!");
      return _coordinates_[idim_] != space::COORDINATE_INVALID;
    }

    uint32_t slot::get_coordinate(unsigned int idim_) const
    {
      DT_THROW_IF(idim_ < COORDINATE_INDEX_0 || idim_ > get_max_coordinate_index(),
                  std::range_error,
                  "Invalid coordinate index [" << idim_ << "]!");
      // std::cerr << "DEVEL: slot::get_coordinate: idim=[" << idim_ << "] = " <<  _coordinates_[idim_] << std::endl;
      return _coordinates_[idim_];
    }

    void slot::set_coordinate(unsigned int idim_, uint32_t value_)
    {
      // std::cerr << "DEVEL: slot::set_coordinate: idim_=" << idim_ << " value_=" << value_ << std::endl;
      DT_THROW_IF(!has_dimension(), std::logic_error,
                  "Slot dimension is not defined!");
      if (_dimension_ == space::DIM_0) {
        // std::cerr << "DEVEL: slot::set_coordinate: _dimension_=" << _dimension_ << std::endl;
        if (idim_ != 0) {
          DT_THROW(std::range_error,
                   "Invalid dimension [" << idim_ << "] for dimensionless slot!");
        }
        if (value_ == space::COORDINATE_INVALID) {
          _coordinates_[0] = value_;
        } else {
          _coordinates_[0] = space::COORDINATE_NA;
        }
      } else {
        // std::cerr << "DEVEL: slot::set_coordinate: _dimension_=" << _dimension_ << std::endl;
        DT_THROW_IF(idim_ > get_max_coordinate_index(),
                    std::range_error,
                    "Invalid coordinate index [" << idim_ << "] with respect to the slot dimension [" << _dimension_ << "]!");
        DT_THROW_IF(!space::coordinate_is_special(value_) &&  value_ > space::COORDINATE_MAX, std::range_error,
                    "Invalid coordinate value [" << value_ << "] at coordinate index [" << idim_ << "]!");
        _coordinates_[idim_] = value_;
      }
      return;
    }

    void slot::reset_coordinates()
    {
      for (size_t i = 0; i < space::DIM_MAX; i++) {
        _coordinates_[i] = space::COORDINATE_INVALID;
      }
      return;
    }

    void slot::fetch_coordinates(std::vector<uint32_t> & coordinates_,
                                 bool clear_) const
    {
      if (clear_) {
        coordinates_.clear();
      }
      if (get_max_coordinate_index() >= 0) {
        for (int i = 0; i <= get_max_coordinate_index(); i++) {
          coordinates_.push_back(_coordinates_[i]);
        }
      }
      return;
    }

    std::vector<uint32_t> slot::get_coordinates() const
    {
      std::vector<uint32_t> c;
      this->fetch_coordinates(c);
      return c;
    }

    // slot::coordinates_array slot::gc() const
    // {
    //   coordinates_array ca;
    //   slot::fetch_coordinates(ca.coordinates);
    //   return ca;
    // }

    // const std::vector<uint32_t> & slot::gc() const
    // {
    //   return ccc;
    // }

    void slot::set(space::dimension_type dimension_, uint32_t i_, uint32_t j_, uint32_t k_)
    {
      // std::cerr << "DEVEL: slot::set: dimension_=" << dimension_ << std::endl;
      set_dimension(dimension_);
      if (dimension_ != space::DIM_INVALID) {
        if (dimension_ == 0) {
          // Use bin 0 to store a validity value for a slot of dimension 0
          set_coordinate(0, (i_ != space::COORDINATE_INVALID)? space::COORDINATE_NA : i_);
        } else {
          if (dimension_ >= space::DIM_1) {
            set_coordinate(0, i_);
          }
          if (dimension_ >= space::DIM_2) {
            set_coordinate(1, j_);
          }
          if (dimension_ == space::DIM_3) {
            set_coordinate(2, k_);
          }
        }
      }
      return;
    }

    slot::slot()
    {
      _dimension_ = space::DIM_INVALID;
      for (size_t i = 0; i < space::DIM_MAX; i++) {
        _coordinates_[i] = space::COORDINATE_INVALID;
      }
      return;
    }

    slot::slot(space::dimension_type dimension_, uint32_t i_, uint32_t j_, uint32_t k_)
    {
      _dimension_ = space::DIM_INVALID;
      set(dimension_, i_, j_, k_);
      return;
    }

    bool slot::is_valid() const
    {
      if (!has_dimension()) {
        return false;
      }
      if (get_dimension() == 0) {
        return _coordinates_[COORDINATE_INDEX_0] != space::COORDINATE_INVALID;
      } else {
        for (int i = 0; i <= (int) get_max_coordinate_index(); i++) {
          if (_coordinates_[i] == space::COORDINATE_INVALID) {
            return false;
          }
        }
      }
      return true;
    }

    void slot::reset()
    {
      reset_dimension();
      reset_coordinates();
      return;
    }

    void slot::make_dimension_0(bool valid_)
    {
      // std::cerr << "DEVEL: slot::make_dimension_0: valid_=" << valid_ << std::endl;
      set(space::DIM_0, valid_ ? space::COORDINATE_NA: space::COORDINATE_INVALID);
      return;
    }

    void slot::make_dimension_1(uint32_t i_)
    {
      set(space::DIM_1, i_);
      return;
    }

    void slot::make_dimension_2(uint32_t i_, uint32_t j_)
    {
      set(space::DIM_2, i_, j_);
      return;
    }

    void slot::make_dimension_3(uint32_t i_, uint32_t j_, uint32_t k_)
    {
      set(space::DIM_3, i_, j_, k_);
      return;
    }

    bool slot::operator==(const slot & slot_) const
    {
      if (_dimension_ == space::DIM_INVALID) {
        return false;
      }
      if (_dimension_ != slot_._dimension_) {
        return false;
      }
      if (_dimension_ == space::DIM_0 || space::at_least_dimension_1(_dimension_)) {
        if (_coordinates_[0] != slot_._coordinates_[0]) {
          return false;
        }
      }
      if (space::at_least_dimension_2(_dimension_)) {
        if (_coordinates_[1] != slot_._coordinates_[1]) {
          return false;
        }
      }
      if (space::at_least_dimension_3(_dimension_)) {
        if (_coordinates_[2] != slot_._coordinates_[2]) {
          return false;
        }
      }
      return true;
    }

    // virtual
    void slot::tree_dump(std::ostream & out_,
                         const std::string & title_,
                         const std::string & indent_,
                         bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Dimension: ";
      if (!has_dimension()) {
        out_ << "<none>";
      } else {
        out_ << '[' << get_dimension()  << ']';
      }
      out_ << std::endl;

      if (has_dimension()) {
        out_ << indent_ << datatools::i_tree_dumpable::tag
             << "Coordinates: ";
        if (_dimension_ == space::DIM_0) {
          out_ << "<none>";
        } else {
          out_ << '(';
          if (space::at_least_dimension_1(_dimension_)) {
            space::coordinate_symbol(out_, _coordinates_[0]);
            if (space::at_least_dimension_2(_dimension_)) {
              out_ << ',';
              space::coordinate_symbol(out_, _coordinates_[1]);
              if (space::at_least_dimension_3(_dimension_)) {
                out_ << ',';
                space::coordinate_symbol(out_, _coordinates_[2]);
              }
            }
          }
          out_ << ')';
        }
        out_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Validity: " << is_valid() << std::endl;

      return;
    }

    bool slot::from_string(const std::string & from_)
    {
      return slot::string_to_slot(from_, *this);
    }

    // friend
    std::ostream & operator<<(std::ostream & out_,
                              const slot & s_)
    {
      std::string str;
      slot::slot_to_string(s_, str);
      out_ << str;
      return out_;
    }

    // static
    bool slot::string_to_slot(const std::string & from_, slot & s_)
    {
      // Ho my ! Please Spirit/Qi-ize me !!!!
      s_.reset();
      std::istringstream iss(from_);
      iss >> std::ws;

      char open = 0;
      open = (char) iss.peek();
      if (open != '(') {
        return false;
      }
      open = iss.get();
      // std::cerr << "DEVEL: open ok\n";

      space::dimension_type dim = space::DIM_INVALID;
      uint32_t c0, c1, c2;
      iss >> std::ws;
      // Parse dimension:
      char nodim = iss.peek();
      // std::cerr << "DEVEL: nodim='" << nodim << "'\n";
      if (nodim == '!') {
        // std::cerr << "DEVEL: undefined dimension\n";
        nodim = iss.get();
        // Undefined dimension:
        dim = space::DIM_INVALID;
        iss >> std::ws;
        char close = iss.peek();
        if (close != ')') {
          return false;
        }
        // std::cerr << "DEVEL: close ok\n";
        close = iss.get();
        return true;
      }

      // Dimension is valid:
      iss >> std::ws;
      nodim = iss.get();
      if (nodim == '0') {
        // std::cerr << "DEVEL: dimension 0\n";
        dim = space::DIM_0;
      } else if (nodim == '1') {
        // std::cerr << "DEVEL: dimension 1\n";
        dim = space::DIM_1;
      } else if (nodim == '2') {
        // std::cerr << "DEVEL: dimension 2\n";
        dim = space::DIM_2;
      } else if (nodim == '3') {
        // std::cerr << "DEVEL: dimension 3\n";
        dim = space::DIM_3;
      } else {
        // std::cerr << "DEVEL: invalid dimension '" << nodim << "'\n";
        return false;
      }

      char next = 0;
      char sep = 0;
      if (dim == space::DIM_0) {
        // std::cerr << "DEVEL: dimensionless slot case...\n";
        // Dimensionless slot:
        // Formats: (0) or (0!)
        bool valid0 = true;
        iss >> std::ws;
        next = iss.peek();
        // std::cerr << "DEVEL: next='" << next << "'\n";
        if (next == '!') {
          valid0 = false;
          next = iss.get();
        }
        next = 0;
        iss >> std::ws;
        next = iss.peek();
        if (next != ')') {
          return false;
        }
        // std::cerr << "DEVEL: close ok\n";
        next = iss.get();
        s_.make_dimension_0(valid0);
        return true;
      }

      if (space::at_least_dimension_1(dim)) {
        // std::cerr << "DEVEL: dim >= DIM_1\n";
        // 1D slot:
        // Formats:
        //  (1:I) with I > 0 or I == '!', '*' , '-'
        //  (1:!)
        //  (1:*)
        //  (1:-)
        sep = 0;
        iss >> std::ws;
        sep = iss.get();
        if (sep != ':') {
          return false;
        }
        next = 0;
        iss >> std::ws;
        next = iss.get();
        // std::cerr << "DEVEL: next='" << next << "'\n";
        if (next == '!') {
          c0 = space::COORDINATE_INVALID;
        } else if (next == '*') {
          c0 = space::COORDINATE_ANY;
        } else if (next == '-') {
          c0 = space::COORDINATE_NA;
        } else {
          iss.putback(next);
          iss >> c0 >> std::ws;
          if (!iss) {
            return false;
          }
        }
        // std::cerr << "DEVEL: c0=[" << c0 << "]\n";
        if (space::at_least_dimension_2(dim)) {
          // std::cerr << "DEVEL: dim >= DIM_2\n";
          // 2D slot:
          // Formats:
          //  (1:I, J) with I, J > 0 or I,J == '!', '*' , '-'
          //  (1:!, J) (1:!, !) (1:I, !)
          //  (1:*, J) ...
          //  (1:-, J) ...
          sep = 0;
          iss >> std::ws;
          sep = iss.get();
          if (sep != ',') {
            return false;
          }
          next = 0;
          iss >> std::ws;
          next = iss.get();
          // std::cerr << "DEVEL: next='" << next << "'\n";
          if (next == '!') {
            c1 = space::COORDINATE_INVALID;
          } else if (next == '*') {
            c1 = space::COORDINATE_ANY;
          } else if (next == '-') {
            c1 = space::COORDINATE_NA;
          } else {
            iss.putback(next);
            iss >> c1 >> std::ws;
            if (!iss) {
              return false;
            }
          }
          // std::cerr << "DEVEL: c1=[" << c1 << "]\n";
          if (space::at_least_dimension_3(dim)) {
            // std::cerr << "DEVEL: dim >= DIM_3\n";
            // 3D slot:
            // Formats:
            //  (1:I, J, K) with I, J, K > 0 or I,J,K == '!', '*' , '-'
            sep = 0;
            iss >> std::ws;
            sep = iss.get();
            if (sep != ',') {
              return false;
            }
            next = 0;
            iss >> std::ws;
            next = iss.get();
            // std::cerr << "DEVEL: next='" << next << "'\n";
            if (next == '!') {
              c2 = space::COORDINATE_INVALID;
            } else if (next == '*') {
              c2 = space::COORDINATE_ANY;
            } else if (next == '-') {
              c2 = space::COORDINATE_NA;
            } else {
              iss.putback(next);
              iss >> c2 >> std::ws;
              if (!iss) {
                return false;
              }
            }
            // std::cerr << "DEVEL: c2=[" << c2 << "]\n";
          }
        }
        char close = iss.get();
        if (close != ')') {
          return false;
        }

        iss >> std::ws;
        std::string tail;
        iss >> tail;
        if (!tail.empty()) {
          return false;
        }

        // std::cerr << "DEVEL: close ok\n";
        if (dim == space::DIM_1) {
          s_.make_dimension_1(c0);
        } else if (dim == space::DIM_2) {
          s_.make_dimension_2(c0, c1);
        } else if (dim == space::DIM_3) {
          s_.make_dimension_3(c0, c1, c2);
        }
        return true;
      }

      return false;
    }

    void slot::to_string(std::string & to_) const
    {
      slot::slot_to_string(*this, to_);
      return;
    }

    std::string slot::str() const
    {
      std::string s;
      to_string(s);
      return s;
    }

    // static
    void slot::slot_to_string(const slot & s_, std::string & to_)
    {
      to_.clear();
      std::ostringstream out;
      out << '(' ;
      space::dimension_type dimension = s_.get_dimension();
      if (dimension == space::DIM_INVALID) {
        out << "!";
      } else if (dimension == space::DIM_0) {
        bool valid = s_._coordinates_[0] != space::COORDINATE_INVALID;
        out << dimension;
        if (!valid) out << '!';
      } else {
        out << dimension << ':';
        if (space::at_least_dimension_1(dimension)) {
          space::coordinate_symbol(out, s_._coordinates_[0]);
          if (space::at_least_dimension_2(dimension)) {
            out << ',';
            space::coordinate_symbol(out, s_._coordinates_[1]);
            if (space::at_least_dimension_3(dimension)) {
              out << ',';
              space::coordinate_symbol(out, s_._coordinates_[2]);
            }
          }
        }
      }
      out << ')';
      to_ = out.str();
      return;
    }

  } // namespace device

} // namespace vire
