//! \file vire/device/base_physical.cc
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
#include <vire/device/base_physical.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/i_instance.h>
#include <vire/device/logical_device.h>

namespace vire {

  namespace device {

    // static
    const std::string & base_physical::physical_name_suffix()
    {
      static const std::string _suffix(".phys");
      return _suffix;
    }

    // static
    std::string base_physical::make_physical_name(const std::string & basename_)
    {
      return basename_ + physical_name_suffix();
    }

    base_physical::base_physical()
    {
      _mother_logical_ = 0;
      _owned_instance_ = false;
      _instance_ = 0;
      return;
    }

    base_physical::base_physical(const base_physical & phys_)
    {
      _mother_logical_ = phys_._mother_logical_;
      _owned_instance_ = false;
      _instance_ = phys_._instance_;
      return;
    }

    base_physical::~base_physical()
    {
      reset();
      return;
    }

    base_physical & base_physical::operator=(const base_physical & phys_)
    {
      if (this == &phys_) return *this;
      if (has_instance()) {
        reset_instance();
      }
      _mother_logical_ = phys_._mother_logical_;
      _owned_instance_ = false;
      _instance_ = phys_._instance_;
      return *this;
    }

    void base_physical::reset()
    {
      reset_mother_logical();
      reset_instance();
      return;
    }

    bool base_physical::has_mother_logical() const
    {
      return _mother_logical_ != 0;
    }

    void base_physical::set_mother_logical(const logical_device & ml_)
    {
      _mother_logical_ = &ml_;
      return;
    }

    const logical_device & base_physical::get_mother_logical() const
    {
      DT_THROW_IF(! has_mother_logical(), std::logic_error,
                  "Physical '" << get_name() << "' has no mother logical !");
      return *_mother_logical_;
    }

    void base_physical::reset_mother_logical()
    {
      _mother_logical_ = 0;
      return;
    }

    bool base_physical::has_instance() const
    {
      return _instance_ != 0;
    }

    void base_physical::reset_instance()
    {
      if (has_instance()) {
        if (_owned_instance_) {
          delete _instance_;
        }
        _instance_ = 0;
      }
      _owned_instance_ = false;
      return;
    }

    void base_physical::set_instance(const i_instance & i_)
    {
      reset_instance();
      _instance_ = &i_;
      _owned_instance_ = false;
      return;
    }

    void base_physical::set_instance(const i_instance * i_)
    {
      reset_instance();
      _instance_ = i_;
      _owned_instance_ = true;
      return;
    }

    void base_physical::set_instance(const i_instance * i_, bool owned_)
    {
      reset_instance();
      _instance_ = i_;
      _owned_instance_ = owned_;
      return;
    }

    const i_instance & base_physical::get_instance() const
    {
      DT_THROW_IF(!has_instance(), std::logic_error, "No instance!");
      return *_instance_;
    }

    void base_physical::tree_dump(std::ostream& out_,
                                    const std::string& title_,
                                    const std::string& indent_,
                                    bool inherit_) const
    {
      this->enriched_base::tree_dump(out_, title_, indent_, true);

      {
        out_ << indent_ << i_tree_dumpable::tag
             << "Mother logical device : ";
        if (! has_mother_logical()) {
          out_ << "<none>";
        } else {
          out_ << "'" << _mother_logical_->get_name() << "'";
        }
        out_ << std::endl;
      }

      {
        out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
             << "Instance : ";
        if (! has_instance()) {
          out_ << "<none>";
        } else {
          out_ << "[@" << _instance_ << "]";
        }
        out_ << std::endl;
        if (has_instance()) {
          std::ostringstream indent2;
          indent2 << indent_ << i_tree_dumpable::inherit_skip_tag(inherit_);
          _instance_->tree_dump(out_, "", indent2.str());
        }
      }

      return;
    }

  } // end of namespace device

} // end of namespace vire
