// vire/device/instance_to_mapping.cc
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
#include <vire/device/instance_to_mapping.h>

namespace vire {

  namespace device {

    instance_to_mapping::instance_to_mapping()
    {
    }

    instance_to_mapping::~instance_to_mapping()
    {
    }

    bool instance_to_mapping::is_valid() const
    {
      if (_id_mgr_ == nullptr) return false;
      if (_category_.empty()) return false;
      return true;
    }

    void instance_to_mapping::reset()
    {
      _id_mgr_ = nullptr;
      _category_.clear();
      return;
    }

    void instance_to_mapping::set_id_manager(const geomtools::id_mgr & id_mgr_)
    {
      _id_mgr_ = &id_mgr_;
      return;
    }

    void instance_to_mapping::set_category(const std::string & category_)
    {
      _category_ = category_;
      _sync_();
      return;
    }

    void instance_to_mapping::_sync_()
    {
      if (_id_mgr_ == nullptr) return;
      if (_category_.empty()) return;
      if (!_id_mgr_->has_category_info(_category_)) return;
      _catinfo_ = &_id_mgr_->get_category_info(_category_);
      return;
    }

    bool instance_to_mapping::build(const i_instance & instance_,
                                    std::vector<std::string> & mapping_rules_)
    {
      mapping_rules_.clear();
      if (!is_valid()) return false;

      {
        std::string mapping_rule;
        std::ostringstream oss;
        oss << '[' << _category_;
        const std::vector<std::string> & xby = _catinfo_->get_extends_by();
        std::size_t xby_dim = xby.size();
        std::size_t inst_dim = 0;
        if (instance_->get_dimension() == vire::device::space::DIM_1) {
          inst_dim = 1;
        } else if (instance_->get_dimension() == vire::device::space::DIM_2) {
          inst_dim = 2;
        } else if (instance_->get_dimension() == vire::device::space::DIM_3) {
          inst_dim = 3;
        }
        if (inst_dim != xby_dim) {
          return false;
        }
        if (xby_dim > 0) {
          oss << ':';
          for (std::size_t i = 0; i < xby_dim; i++) {
            if (i != 0) {
              oss << ',';
            }
            oss << xby[i];
            oss << "+0";
          }
        }
        oss << ']';
        mapping_rule = oss.str();
      }
      mapping_rules_.push_back(mapping_rule);
      return true;
    }


  } // namespace device

} // namespace vire
