//! \file  vire/cmsserver/uc_model_db.h
//! \brief A database of use case model descriptions for the CMS server
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

#ifndef VIRE_CMSSERVER_UC_MODEL_DB_H
#define VIRE_CMSSERVER_UC_MODEL_DB_H

// Standard Library:
#include <string>
#include <map>
#include <set>

// Third party:
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/multi_properties.h>
#include <bayeux/datatools/dependency_graph.h>

// This project:
#include <vire/cmsserver/uc_model_description.h>

namespace vire {

  namespace cmsserver {

    /// \brief DB of use case model descriptions
    class uc_model_db
      : public datatools::i_tree_dumpable
      , public datatools::i_serializable
    {
    public:

      typedef std::map<std::string, uc_model_description> dict_type;

      /// Default constructor
      uc_model_db();

      /// Destructor
      virtual ~uc_model_db();

      bool is_check_uc_factory() const;

      void set_check_uc_factory(const bool check_);

      void add_supported_type_id(const std::string & type_id_);

      bool has_supported_type_id(const std::string & type_id_) const;
      
      bool has(const std::string & name_) const;

      void add(const uc_model_description & umd_);
      
      void add(const std::string & name_,
               const std::string & type_id_,
               const std::string & description_,
               const uc_composition_description & composition_,
               const datatools::properties & config_);
      
      void add(const std::string & name_,
               const std::string & type_id_,
               const std::string & description_,
               const uc_composition_description & composition_,
               const std::string & config_path_);

      void remove(const std::string & name_);

      void remove_all();

      void build_list_of_model_names(std::vector<std::string> & names_) const;

      const dict_type & get_models() const;

      const uc_model_description & get(const std::string & name_) const;
      
      void load(const datatools::multi_properties & model_defs_);

      bool match_type_id(const std::string & type_id_) const;

      bool is_locked() const;

      void lock();

      void unlock();

      void configure(const datatools::properties & config_);
 
      void reset();
      
      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const;

      bool has_cyclic_dependencies() const;

      const std::set<std::string> & get_cyclic_dependencies() const;
 
      bool has_unsatisfied_dependencies() const;
   
      const std::set<std::string> & get_unsatisfied_dependencies() const;
   
    private:

      void _add_check_(const uc_model_description & umd_);
      
    private:

      // Management:
      bool                  _locked_ = false;

      // Configuration:
      bool                  _check_uc_factory_ = false; //!< Flag to check use case type ID from the system factory
      std::set<std::string> _supported_types_; //!< List of supported types 
      dict_type             _models_;          //!< Dictionary of UC model descriptions

      // Working data:
      datatools::dependency_graph _dependencies_; //!< Dependency graph
      std::set<std::string>       _cyclic_dependencies_;     
      std::set<std::string>       _unsatisfied_dependencies_;     
      
      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION()

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_MODEL_DESCRIPTION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
