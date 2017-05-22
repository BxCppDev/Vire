// opcua_utils.cc
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
#include <vire/mos/export_utils.h>

namespace vire {

  namespace mos {

    device_entry_type::device_entry_type()
    {
      return;
    }

    void device_entry_type::reset()
    {
      name.clear();
      model_type.clear();
      class_id.clear();
      config.reset();
      return;
    }

    void device_entry_type::tree_dump(std::ostream & out_,
                                      const std::string & title_,
                                      const std::string & indent_,
                                      bool inherit_) const
    {
      if (! title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name  : '" << name << "'" << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Model type = '" << model_type << "'" << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Class Id   = '" << class_id << "'" << std::endl;
      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Config     = [" << config.size() << "]" << std::endl;
      if (config.size()) {
        std::ostringstream indent2;
        indent2 << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
        config.tree_dump(out_, "", indent2.str());
      }
      return;
    }

  } // namespace mos

} // namespace vire
