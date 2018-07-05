//! \file  vire/simulation/base_object.h
//! \brief Simulated base object
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

#ifndef VIRE_SIMULATION_BASE_OBJECT_H
#define VIRE_SIMULATION_BASE_OBJECT_H

// Standard library:
#include <string>
#include <map>
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>

namespace vire {

  namespace simulation {

    /// Simulated base object
    class base_object
      : public datatools::enriched_base
    {
    public:

      typedef base_object *                       plain_pointer_type;
      typedef const base_object *                 const_plain_pointer_type;
      typedef std::shared_ptr<base_object>        pointer_type;
      typedef std::map<std::string, pointer_type> object_dict_type;
      typedef std::map<std::string, plain_pointer_type> plain_object_dict_type;
      typedef std::list<pointer_type>             object_list_type;
      typedef std::list<plain_pointer_type>       plain_object_list_type;

      static bool validate_name(const std::string & candidate_name_);

      bool is_name_valid(const std::string & name_) const override;

      base_object();

      virtual ~base_object();

      virtual bool is_built() const;

      bool has_category() const;

      void set_category(const std::string & category_);

      const std::string & get_category() const;

      bool has_parent() const;

      bool has_child(const std::string &) const;

      void add_child(const std::string & name_, const pointer_type & ptr_);

      void remove_child(const std::string & name_);

      bool has_subscription(const std::string & name_) const;
 
      void subscribe_to(const std::string & name_, plain_pointer_type & ptr_);
      
      void unsubscribe_from(const std::string & name_);

      bool has_subscribers() const;
      
      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;
     
    private:

      std::string            _category_;      ///< The object category
      plain_object_dict_type _subscriptions_; ///!< Dictionary of subscriptions
      plain_object_list_type _subscribers_;   ///!< List of subscribers
      object_dict_type       _children_;      ///< Hierarchical composition
      const_plain_pointer_type _parent_ = nullptr; ///< The hierarchical parent object

    };
    
  } // namespace simulation

} // namespace vire

#endif // VIRE_SIMULATION_BASE_OBJECT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
