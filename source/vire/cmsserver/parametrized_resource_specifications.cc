//! \file vire/cmsserver/parametrized_resource_specifications.cc
//
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/parametrized_resource_specifications.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/multi_properties.h>

namespace vire {

  namespace cmsserver {

    bool parametrized_resource_specifications::is_empty() const
    {
      return _specs_.size() == 0;
    }

    std::size_t parametrized_resource_specifications::size() const
    {
      return _specs_.size();
    }

    bool parametrized_resource_specifications::has(const std::string & name_) const
    {
      return _specs_.count(name_);
    }

    void parametrized_resource_specifications::build_names(std::set<std::string> & names_) const
    {
      names_.clear();
      for (auto p : _specs_) {
        names_.insert(p.first);
      }
      return;
    }
    
    bool parametrized_resource_specifications::is_resource(const std::string & name_) const
    {
      specs_dict_type::const_iterator found = _specs_.find(name_);
      DT_THROW_IF(found == _specs_.end(), std::logic_error, "No entry with name '" << name_ << "'!");
      return found->second.what == WHAT_RESOURCE;
    }
      
    bool parametrized_resource_specifications::is_device(const std::string & name_) const
    {
      specs_dict_type::const_iterator found = _specs_.find(name_);
      DT_THROW_IF(found == _specs_.end(), std::logic_error, "No entry with name '" << name_ << "'!");
      return found->second.what == WHAT_DEVICE;
    }
     
    void parametrized_resource_specifications::add_device(const std::string & name_,
                                                          const std::string & type_)
    {
      DT_THROW_IF(has(name_), std::logic_error, "Entry with name '" << name_ << "' already exists!");
      entry e;
      e.name = name_;
      e.type = type_;
      e.what = WHAT_DEVICE;
      _specs_[name_] = e;
      return;
    }

    void parametrized_resource_specifications::add_resource(const std::string & name_,
                                                            const std::string & type_)
    {
      DT_THROW_IF(has(name_), std::logic_error, "Entry with name '" << name_ << "' already exists!");
      entry e;
      e.name = name_;
      e.type = type_;
      e.what = WHAT_RESOURCE;
      _specs_[name_] = e;
      return;
    }

    void parametrized_resource_specifications::remove(const std::string & name_)
    {
      DT_THROW_IF(!has(name_), std::logic_error, "Entry with name '" << name_ << "' does not exist!");
      _specs_.erase(name_);
      return;
    }

    const std::string & parametrized_resource_specifications::get_type(const std::string & name_)
    {
      specs_dict_type::const_iterator found = _specs_.find(name_);
      DT_THROW_IF(found == _specs_.end(), std::logic_error, "No entry with name '" << name_ << "'!");
      return found->second.type;
    }
      
    bool parametrized_resource_specifications::is_type(const std::string & name_, const std::string & type_)
    {
      return get_type(name_) == type_;
    }

    void parametrized_resource_specifications::clear()
    {
      _specs_.clear();
      return;
    }

    void parametrized_resource_specifications::load(const std::string & path_)
    {
      std::string spec_path = path_;
      datatools::fetch_path_with_env(spec_path);
      datatools::multi_properties spec_mprops;
      spec_mprops.read(spec_path);
      const datatools::multi_properties::entries_ordered_col_type & entries
        = spec_mprops.ordered_entries();
      for (datatools::multi_properties::entries_ordered_col_type::const_iterator i
             = entries.begin();
           i != entries.end();
           i++) {
        const datatools::multi_properties::entry & e = **i;
        const std::string & name = e.get_key();
        const std::string & meta = e.get_meta();
        const datatools::properties & props = e.get_properties();
        std::string type;
        if (props.has_key("type")) {
          type = props.fetch_string("type");
        }
        if (meta == "device") {
          add_device(name, type);
        } if (meta == "resource") {
          add_resource(name, type);
        } else {
          DT_THROW(std::logic_error, "Invalid specification type '" << meta << "'!");
        }
      }
      return;
    }
    
    void parametrized_resource_specifications::print_tree(std::ostream & out_,
                                                          const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);

      out_ << popts.indent << i_tree_dumpable::tag
           << "Number of resource specification entries : "
           << _specs_.size()
           << std::endl;

      if (!is_empty()) {
        out_ << popts.indent << i_tree_dumpable::tag
             << "Resource specifications : " << std::endl;
        size_t count = 0;
        for(const auto e : _specs_) {
          out_ << popts.indent << i_tree_dumpable::skip_tag;
          if (++count == _specs_.size()) {
            out_ << i_tree_dumpable::last_tag;
          } else {
            out_ << i_tree_dumpable::tag;
          }
          what_type what = e.second.what;
          out_ << "Name='" << e.first << "'" << " : " << "type='" << e.second.type << "' (";
          if (what == WHAT_DEVICE) out_ << "device";
          else if (what == WHAT_RESOURCE) out_ << "resource";
          out_ << ")"  << std::endl;
        }
      }  
           
      out_ << popts.indent << i_tree_dumpable::inherit_tag(popts.inherit)
           << "Empty : "
           << std::boolalpha
           << this->is_empty()
           << std::endl;
    
      return;
    }

  } // namespace cmsserver

} // namespace vire
