//! \file vire/simulation/base_object.cc
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
#include <vire/simulation/base_object.h>

// Third party:
// - Boost:
#include <boost/algorithm/string/trim.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace simulation {

    // static
    bool base_object::validate_name(const std::string & candidate_name_)
    {
      return ::datatools::name_validation(candidate_name_, ::datatools::NV_INSTANCE);
    }
    
    // virtual override
    bool base_object::is_name_valid(const std::string & name_) const
    {
      return base_object::validate_name(name_);
    }

    base_object::base_object()
    {
      return;
    }

    // virtual
    base_object::~base_object()
    {
      return;
    }

    // virtual
    bool base_object::is_built() const
    {
      return true;
    }

    bool base_object::has_category() const
    {
      return !_category_.empty();
    }

    void base_object::set_category(const std::string & category_)
    {
      DT_THROW_IF(boost::algorithm::trim_copy(category_).empty(),
                  std::logic_error,
                  "Invalid empty category!");
      _category_ = category_;
      return;
    }

    const std::string & base_object::get_category() const
    {
      return _category_;
    }

    bool base_object::has_parent() const
    {
      return _parent_ != nullptr;
    }

    bool base_object::has_child(const std::string & name_) const
    {
      return _children_.count(name_) > 0;
    }
 
    void base_object::add_child(const std::string & name_, const pointer_type & ptr_)
    {
      DT_THROW_IF(!validate_name(name_), std::logic_error,
                  "Object '" << get_name() << "' cannot accept a child with name '"
                  << name_ << "'!");
      DT_THROW_IF(ptr_->has_parent(), std::logic_error,
                  "Object '" << get_name() << "' cannot accept the child '"
                  << name_ << "' with its parent already set!");
      DT_THROW_IF(has_child(name_), std::logic_error,
                  "Object '" << get_name() << "' already has child with name '"
                  << name_ << "'!");
      _children_[name_] = ptr_;
      {
        object_dict_type::iterator found = _children_.find(name_);
        found->second->_parent_ = this;
      }
      return;
    }

    void base_object::remove_child(const std::string & name_)
    {
      object_dict_type::iterator found = _children_.find(name_);
      DT_THROW_IF(found == _children_.end(), std::logic_error,
                  "Object '" << get_name() << "'  has no child with name '"
                  << name_ << "'!");
      found->second->_parent_ = nullptr;
      _children_.erase(found);
      return;
    }
 
    bool base_object::has_subscribers() const
    {
      return _subscribers_.size();
    }
 
    bool base_object::has_subscription(const std::string & name_) const
    {
      return _subscriptions_.count(name_) > 0;
    }
  
    void base_object::subscribe_to(const std::string & name_, plain_pointer_type & ptr_)
    {
      DT_THROW_IF(!validate_name(name_), std::logic_error,
                  "Object '" << get_name()
                  << "' cannot create a subscription with name '" << name_ << "'!");
      _subscriptions_[name_] = ptr_;
      ptr_->_subscribers_.push_back(this);
      return;
    }
      
    void base_object::unsubscribe_from(const std::string & name_)
    {
      plain_object_dict_type::iterator found = _subscriptions_.find(name_);
      DT_THROW_IF(found == _subscriptions_.end(),
                  std::logic_error,
                  "Object '" << get_name()
                  << "'  has no subscription with name '" << name_ << "'!");
      plain_object_list_type::iterator found2
        = std::find(found->second->_subscribers_.begin(),
                    found->second->_subscribers_.end(),
                    this);
      if (found2 != found->second->_subscribers_.end()) {
        found->second->_subscribers_.erase(found2);
      }
      _subscriptions_.erase(found);
      return;
    }
       
    // virtual
    void base_object::print_tree(std::ostream & out_,
                                 const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      this->datatools::enriched_base::print_tree(out_,
                                                 base_print_options::force_inheritance(options_));

      out_ <<  popts.indent << tag
           << "Built : " << std::boolalpha << is_built() << std::endl;

      out_ <<  popts.indent << tag
           << "Category : ";
      if (has_category()) {
        out_ << "'" << get_category() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Subscriptions : ";
      out_ << _subscriptions_.size();
      out_ << std::endl;
      if (_subscriptions_.size()) {
        std::size_t counter = 0;
        for (plain_object_dict_type::const_iterator i = _subscriptions_.begin();
             i != _subscriptions_.end();
             i++) {
          out_ << popts.indent << tag
               << skip_tag;
          if (++counter == _subscriptions_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Subscription '" << i->first << "'" << std::endl;
        }
      }

      out_ <<  popts.indent << tag
           << "Subscribers : ";
      out_ << _subscribers_.size();
      out_ << std::endl;
      if (_subscribers_.size()) {
        std::size_t counter = 0;
        for (plain_object_list_type::const_iterator i = _subscribers_.begin();
             i != _subscribers_.end();
             i++) {
          out_ << popts.indent << tag
               << skip_tag;
          if (++counter == _subscribers_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Subscriber '" << (*i)->get_name() << "'" << std::endl;
        }
      }

      out_ <<  popts.indent << tag
           << "Children : ";
      out_ << _children_.size();
      out_ << std::endl;
      if (_children_.size()) {
        std::size_t counter = 0;
        for (object_dict_type::const_iterator i = _children_.begin();
             i != _children_.end();
             i++) {
          out_ << popts.indent << skip_tag;
          if (++counter == _children_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Child '" << i->first << "'" << std::endl;
        }
      }

      out_ <<  popts.indent << inherit_tag(popts.inherit)
           << "Parent : ";
      if (has_parent()) {
        out_ << _parent_->get_name();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace simulation

} // namespace vire
