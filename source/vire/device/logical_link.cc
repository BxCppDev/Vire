//! \file vire/device/logical_link.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/logical_link.h>

// - Bayeux/datatools
#include <datatools/exception.h>

// This project
#include <vire/device/link_types.h>
#include <vire/device/base_link_model.h>

namespace vire {

  namespace device {

    logical_link::logical_link()
    {
      _initialized_ = false;
      _model_ = 0;
      return;
    }

    logical_link::~logical_link()
    {
      if (_initialized_) {
        reset();
      }
      return;
    }

    bool logical_link::has_model() const
    {
      return _model_ != 0;
    }

    void logical_link::set_model(const base_link_model & m_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Logical link '" << get_name() << "' is locked !");
      _model_ = &m_;
      return;
    }

    const base_link_model & logical_link::get_model() const
    {
      DT_THROW_IF(! has_model(),
                  std::logic_error,
                  "Logical link '" << get_name() << "' does not have an associated model !");
      return *_model_;
    }

    bool logical_link::is_initialized() const
    {
      return _initialized_;
    }

    void logical_link::initialize()
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Logical link '"
                  << get_name() << "' is already initialized !");
      _initialized_ = true;
      return;
    }

    void logical_link::reset()
    {
      DT_THROW_IF(! is_initialized(), std::logic_error, "Logical link '"
                  << get_name() << "' is not initialized !");
      _initialized_ = false;
      _model_ = 0;
      return;
    }

    void logical_link::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      this->enriched_base::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Initialized : " << _initialized_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Model : ";
      if (_model_) {
        out_ << "'" << _model_->get_name() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // end of namespace device

} // end of namespace vire
