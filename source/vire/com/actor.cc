//! \file vire/com/actor.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/com/actor.h>

// Standard library:
#include <sstream>

namespace vire {

  namespace com {

    // static
    std::string actor::category_label(const category_type pt_)
    {
      switch(pt_) {
      case CATEGORY_SERVER: return "server";
      case CATEGORY_CLIENT: return "client";
      case CATEGORY_SUBCONTRACTOR: return "subcontractor";
      case CATEGORY_SYSTEM: return "system";
      default:
        break;
      }
      return "";
    }

    // static
    actor::category_type actor::category(const std::string & label_)
    {
      if (label_ == category_label(CATEGORY_SERVER)) return CATEGORY_SERVER;
      if (label_ == category_label(CATEGORY_CLIENT)) return CATEGORY_CLIENT;
      if (label_ == category_label(CATEGORY_SUBCONTRACTOR)) return CATEGORY_SUBCONTRACTOR;
      if (label_ == category_label(CATEGORY_SYSTEM)) return CATEGORY_SYSTEM;
      return CATEGORY_INVALID;
    }

    // static
    std::string actor::build_name(const category_type category_,
                                  const std::string & setup_name_,
                                  const std::string & id_)
    {
      std::ostringstream out;
      if (!setup_name_.empty()) {
        out << setup_name_ << '.';
      }
      out << "vire";
      if (category_ == CATEGORY_SERVER) {
        out << ".server";
      } else if (category_ == CATEGORY_CLIENT) {
        out << ".client";
      } else if (category_ == CATEGORY_SUBCONTRACTOR) {
        out << ".subcontractor";
      } else if (category_ == CATEGORY_SYSTEM) {
        out << ".system";
      }
      if (!id_.empty()) {
        out << '.' << id_;
      }
      return out.str();
    }

    actor::actor()
    {
      return;
    }

    actor::actor(const std::string & name_, const category_type category_)
    {
      set_name(name_);
      set_category(category_);
      return;
    }

    actor::~actor()
    {
      return;
    }

    bool actor::is_valid() const
    {
      if (!has_category()) return false;
      return true;
    }

    void actor::reset()
    {
      _name_.clear();
      _category_ = CATEGORY_INVALID;
      _metadata_.clear();
      return;
    }

    bool actor::has_category() const
    {
      return _category_ != CATEGORY_INVALID;
    }

    void actor::set_category(const category_type & category_)
    {
      _category_ = category_;
      return;
    }

    actor::category_type actor::get_category() const
    {
      return _category_;
    }

    bool actor::has_name() const
    {
      return !_name_.empty();
    }

    void actor::set_name(const std::string & name_)
    {
      _name_ = name_;
      return;
    }

    const std::string & actor::get_name() const
    {
      return _name_;
    }

    bool actor::is_server() const
    {
      return _category_ == CATEGORY_SERVER;
    }

    bool actor::is_client() const
    {
      return _category_ == CATEGORY_CLIENT;
    }

    bool actor::is_subcontractor() const
    {
      return _category_ == CATEGORY_SUBCONTRACTOR;
    }

    bool actor::is_system() const
    {
      return _category_ == CATEGORY_SYSTEM;
    }

    datatools::properties & actor::grab_metadata()
    {
      return _metadata_;
    }

    const datatools::properties & actor::get_metadata() const
    {
      return _metadata_;
    }

    void actor::tree_dump(std::ostream & out_,
                         const std::string & title_,
                         const std::string & indent_,
                         bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Validity : " << std::boolalpha << is_valid() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Category : '" << category_label(_category_) << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name     : '" << _name_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Metadata : [" << _metadata_.size() << "]" << std::endl;

      return;
    }

  } // namespace com

} // namespace vire
