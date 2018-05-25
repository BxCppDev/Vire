//! \file  vire/cmsserver/parametrized_resource_specifications.h
//! \brief The CMS server use case parametrized resource specifications
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

#ifndef VIRE_CMSSERVER_PARAMETRIZED_RESOURCE_SPECIFICATIONS_H
#define VIRE_CMSSERVER_PARAMETRIZED_RESOURCE_SPECIFICATIONS_H

// Standard Library:
#include <string>
#include <set>
#include <map>
#include <iostream>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>

// This project:
#include <vire/utility/model_identifier.h>

namespace vire {

  namespace cmsserver {

    /// \brief Parametrized resource specifications
    class parametrized_resource_specifications
      : public datatools::i_tree_dumpable
    {
    public:

      enum what_type {
        WHAT_INVALID  = 0,
        WHAT_DEVICE   = 1,
        WHAT_RESOURCE = 2
      };
      
      struct entry {
        std::string name;
        std::string type;
        what_type   what;
      };

      typedef std::map<std::string, entry> specs_dict_type;

      bool has(const std::string & name_) const;

      void build_names(std::set<std::string> & names_) const;
      
      bool is_resource(const std::string & name_) const;
      
      bool is_device(const std::string & name_) const;
      
      void add_device(const std::string & name_,
                      const std::string & type_);

      void add_resource(const std::string & name_,
                        const std::string & type_);

      void remove(const std::string & name_);

      const std::string & get_type(const std::string & name_);

      bool is_type(const std::string & name_, const std::string & type_);
      
      void clear();

      bool is_empty() const;

      std::size_t size() const;

      void load(const std::string & path_);
      
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_ = datatools::i_tree_dumpable::empty_options()) const;

    private:
      
      specs_dict_type _specs_; ///!< Specifications
      
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_PARAMETRIZED_RESOURCE_SPECIFICATIONS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
