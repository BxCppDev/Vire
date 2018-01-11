//! \file vire/device/instance_tree.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/device/instance_tree.h>

// Third Party:
// - Boost:
#include <boost/algorithm/string.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>

namespace vire {

  namespace device {

    instance_tree::instance_tree()
    {
      _logging_priority_ = datatools::logger::PRIO_FATAL;
      _locked_ = false;
      return;
    }

    // virtual
    instance_tree::~instance_tree()
    {
      reset();
      return;
    }

    void instance_tree::reset()
    {
      if (is_locked()) {
        unlock();
      }
      _instances_.clear();
      _parent_path_.clear();
      return;
    }

    datatools::logger::priority instance_tree::get_logging_priority() const
    {
      return _logging_priority_;
    }

    void instance_tree::set_logging_priority(const datatools::logger::priority p_)
    {
      _logging_priority_ = p_;
      return;
    }

    const std::string & instance_tree::get_parent_path() const
    {
      return _parent_path_;
    }

    void instance_tree::set_parent_path(const std::string & path_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Tree is locked!");
      _parent_path_ = path_;
      return;
    }

    bool instance_tree::has_instance(const std::string & path_) const
    {
      return _instances_.count(path_) == 1;
    }

    void instance_tree::add_instance(instance_info_ptr & pii_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Tree is locked!");
      DT_THROW_IF(pii_.get() == nullptr, std::logic_error,
                  "Attempt to insert an invalid instance handle!");
      DT_THROW_IF(!pii_->is_valid(), std::logic_error,
                  "Attempt to insert an invalid instance!");
      DT_THROW_IF(has_instance(pii_->get_path()), std::logic_error,
                  "Instance '" << pii_->get_path() << "' already exists in the tree!");
     _instances_[pii_->get_path()] = pii_;
      return;
    }

    void instance_tree::add_instance(instance_info & ii_)
    {
      instance_info_ptr ii_ptr(new instance_info);
      *ii_ptr = ii_;
      add_instance(ii_ptr);
      return;
    }

    const instance_info & instance_tree::get_instance(const std::string & path_) const
    {
      instance_info_dict_type::const_iterator found = _instances_.find(path_);
      DT_THROW_IF(found == _instances_.end(), std::logic_error,
                  "No instance with path '" << path_ << "' exists in the tree!");
      return *found->second.get();
    }

    instance_info & instance_tree::grab_instance(const std::string & path_)
    {
      instance_info_dict_type::iterator found = _instances_.find(path_);
      DT_THROW_IF(found == _instances_.end(), std::logic_error,
                  "No instance with path '" << path_ << "' exists in the tree!");
      return *found->second.get();
    }

    const instance_info_ptr & instance_tree::get_instance_ptr(const std::string & path_) const
    {
      instance_info_dict_type::const_iterator found = _instances_.find(path_);
      DT_THROW_IF(found == _instances_.end(), std::logic_error,
                  "No instance with path '" << path_ << "' exists in the tree!");
      return found->second;
    }

    instance_info_ptr & instance_tree::grab_instance_ptr(const std::string & path_)
    {
      instance_info_dict_type::iterator found = _instances_.find(path_);
      DT_THROW_IF(found == _instances_.end(), std::logic_error,
                  "No instance with path '" << path_ << "' exists in the tree!");
      return found->second;
    }

    std::size_t instance_tree::size() const
    {
      return _instances_.size();
    }

    bool instance_tree::is_parent_of(const std::string & candidate_parent_,
                                       const std::string & path_) const
    {
      DT_THROW_IF(!has_instance(candidate_parent_), std::logic_error, "No instance with path '" << candidate_parent_ << "'!");
      DT_THROW_IF(!has_instance(path_), std::logic_error, "No instance with path '" << path_ << "'!");
      if (boost::starts_with(path_, candidate_parent_) && (path_.length() > candidate_parent_.length())) {
        return true;
      }
      return false;
    }

    bool instance_tree::is_child_of(const std::string & candidate_child_,
                                    const std::string & path_) const
    {
      DT_THROW_IF(!has_instance(candidate_child_), std::logic_error, "No instance with path '" << candidate_child_ << "'!");
      DT_THROW_IF(!has_instance(path_), std::logic_error, "No instance with path '" << path_ << "'!");
      bool child = false;
      if (is_parent_of(path_, candidate_child_)) {
        child = true;
      }
      return child;
    }

    void instance_tree::build_parents_of(const std::string & child_path_,
                                         std::set<std::string> & paths_) const
    {
      DT_THROW_IF(!has_instance(child_path_), std::logic_error, "No instance with path '" << child_path_ << "'!");
      paths_.clear();
      for (const auto & p : _instances_) {
        const instance_info & ii = *p.second.get();
        bool add_it = false;
        if (is_parent_of(p.first, child_path_)) {
          add_it = true;
        }
        if (add_it) {
          paths_.insert(p.first);
        }
      }
      return;
    }

    void instance_tree::build_children_of(const std::string & parent_path_,
                                          std::set<std::string> & paths_,
                                          const uint32_t flags_) const
    {
      DT_THROW_IF(!has_instance(parent_path_), std::logic_error, "No instance with path '" << parent_path_ << "'!");
      paths_.clear();
      bool with_devices = true;
      bool with_ports = true;
      if (flags_ & BP_NO_DEVICES) {
        with_devices = false;
      }
      if (flags_ & BP_NO_PORTS) {
        with_ports = false;
      }
      for (const auto & p : _instances_) {
        const instance_info & ii = *p.second.get();
        bool add_it = false;
        if (is_child_of(p.first, parent_path_)) {
          if (with_devices && ii.is_device()) {
            add_it = true;
          }
          if (with_ports && ii.is_port()) {
            add_it = true;
          }
        }
        if (add_it) {
          paths_.insert(p.first);
        }
      }
      return;
    }

    std::size_t instance_tree::build_paths(std::set<std::string> & paths_,
                                           const uint32_t flags_) const
    {
      if (flags_ & BP_CLEAR) {
        paths_.clear();
      }
      std::size_t old = paths_.size();
      bool with_devices = true;
      bool with_ports = true;
      if (flags_ & BP_NO_DEVICES) {
        with_devices = false;
      }
      if (flags_ & BP_NO_PORTS) {
        with_ports = false;
      }
      for (const auto & p : _instances_) {
        const instance_info & ii = *p.second.get();
        bool add_it = false;
        if (with_devices && ii.is_device()) {
          add_it = true;
        }
        if (with_ports && ii.is_port()) {
          add_it = true;
        }
        if (add_it) {
          paths_.insert(p.first);
        }
      }
      return paths_.size() - old;
    }

    const instance_tree::instance_info_dict_type & instance_tree::get_instances() const
    {
      return _instances_;
    }

    bool instance_tree::is_locked() const
    {
      return _locked_;
    }

    void instance_tree::lock()
    {
      _locked_ = true;
      return;
    }

    void instance_tree::unlock()
    {
      _locked_ = false;
      return;
    }

    void instance_tree::print_tree(std::ostream & out_,
                                   const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);
      bool instances_list = options_.get<bool>("list_instances", false);
      // bool instances_full = options_.get<bool>("full_instances", false);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Logging priority  : '"
           << datatools::logger::get_priority_label(_logging_priority_)
           << "'" << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Number of instance entries : "
           << _instances_.size() << std::endl;
      if (instances_list) {
        std::size_t count = 0;
        for (const auto & p : _instances_) {
          count++;
          out_ << popts.indent << datatools::i_tree_dumpable::inherit_skip_tag(popts.inherit);
          bool last = false;
          if (count == _instances_.size()) {
            last = true;
          }
          if (last) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "Path : '" << p.first << "'";
          out_ << std::endl;
          // if (instances_full) {
          //   boost::property_tree::ptree info_options;
          //   info_options.put(datatools::i_tree_dumpable::base_print_options::indent_key(),
          //                    popts.indent + datatools::i_tree_dumpable::tags::item(last, true));
          //   p.second->print_tree(out_, info_options);
          // }
        }
      }

      return;
    }

  } // end of namespace device

} // end of namespace vire
