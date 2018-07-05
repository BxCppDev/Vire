//! \file vire/com/base_plug.cc
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/com/base_plug.h>

// This project:
#include <vire/com/manager.h>
#include <vire/time/utils.h>
#include <vire/message/message.h>

namespace vire {

  namespace com {

    base_plug::base_plug(const std::string & name_,
                         const actor & parent_,
                         const domain & domain_,
                         const datatools::logger::priority logging_)
      : _name_(name_)
      , _parent_(parent_)
      , _domain_(domain_)
      , _logging_(logging_)
    {
      return;
    }

    base_plug::~base_plug()
    {
       return;
    }
    
    datatools::logger::priority base_plug::get_logging() const
    {
      return _logging_;
    }
                        
    void base_plug::set_logging(const datatools::logger::priority logging_)
    {
      _logging_ = logging_;
      return;
    }

    const std::string & base_plug::get_name() const
    {
      return _name_;
    }
 
    const actor & base_plug::get_parent() const
    {
      return _parent_;
    }

    const domain & base_plug::get_domain() const
    {
      return _domain_;
    }

    const datatools::properties & base_plug::get_metadata() const
    {
      return _metadata_;
    }

    datatools::properties & base_plug::grab_metadata()
    {
      return _metadata_;
    }
    
    int base_plug::get_next_message_id() const
    {
      return _next_message_id_;
    }
       
    int base_plug::_pop_next_message_id()
    {
      return _next_message_id_++;
    }

    void base_plug::tree_dump(std::ostream & out_,
                              const std::string & title_,
                              const std::string & indent_,
                              bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << tag
           << "Name:   '" << get_name() << "'" << std::endl;

      out_ << indent_ << tag
           << "Parent: '" << get_parent().get_name() << "'" << std::endl;

      out_ << indent_ << tag
           << "Domain: '" << get_domain().get_name() << "'" << std::endl;

      out_ << indent_ << tag
           << "Category : '" << to_string(get_category()) << "'" << std::endl;

      out_ << indent_ << inherit_tag(inherit_)
           << "Logging: '" << _logging_ << "'" << std::endl;

      return;
    }

  } // namespace com

} // namespace vire
