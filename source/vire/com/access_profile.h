//! \file  vire/com/access_profile.h
//! \brief Vire communication access profile
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_ACCESS_PROFILE_H
#define VIRE_COM_ACCESS_PROFILE_H

// Standard library:
#include <string>
#include <map>
#include <set>
#include <memory>

// Third party:
// - Boost:
#include <boost/noncopyable.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/com/utils.h>
#include <vire/com/access_hub.h>

namespace vire {

  namespace com {

    class base_plug;
    class plug_factory;
    class manager;
    class domain;
    
    //! \brief Access profile
    class access_profile
      : public ::datatools::i_tree_dumpable
      , private boost::noncopyable
    {
    public:
 
      // typedef std::map<std::string, std::shared_ptr<base_plug>> plug_dict_type;
      typedef std::map<std::string, const domain *> domain_dict_type;
      
      //! Constructor
      access_profile(const manager & com_,
                     const access_category_type category_,
                     const std::string & target_,
                     const std::string & name_,
                     const std::string & password_);

      //! Destructor
      virtual ~access_profile();

      //! Check validity
      bool is_valid() const;

      //! Check if name is set
      bool has_name() const;

      //! Set the name
      void set_name(const std::string & name_);

      //! Return the name
      const std::string & get_name() const;

      //! Check if category is set
      bool has_category() const;

      //! Set the category
      void set_category(const access_category_type & category_);

      //! Return the category
      access_category_type get_category() const;

      //! Check if password is set
      bool has_password() const;

      //! Set the password
      void set_password(const std::string & password_);

      //! Return the password
      const std::string & get_password() const;

      //! Check if a
      bool match_password(const std::string & password_) const;
  
      //! Check if target is set
      bool has_target() const;

      //! Set the target
      void set_target(const std::string & target_);

      //! Return the target
      const std::string & get_target() const;

      //! Return the metadata
      datatools::properties & grab_metadata();

      //! Return the metadata
      const datatools::properties & get_metadata() const;

      /// Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_ = empty_options()) const override;

      // void lock();
      bool is_locked() const;

      const manager & get_com() const;
 
      plug_factory & grab_plug_factory();

      bool has_domain(const std::string & domain_label_) const;
 
      void build_list_of_domain_local_labels(std::set<std::string> & domain_labels_) const;

      const domain & get_domain(const std::string & domain_label_) const;

      // XXX useful ???
      void build_list_of_domain_names(std::set<std::string> & domain_names_) const;

      access_hub_ptr_type create_access_hub(const std::string & name_,
                                            const std::string & select_domain_ = "all",
                                            const bool lock_ = true) const;

    private:
      
      void _build_domains_indexation_();
      
    private:

      const manager *       _com_    = nullptr;
      access_category_type   _category_; //!< Access category
      std::string           _target_;   //!< Client or subcontractor unique identifier
      std::string           _name_;     //!< Access name/login to access the underlying com system
      std::string           _password_; //!< Access password to access the underlying com system
      datatools::properties _metadata_; //!< Metadata
      domain_dict_type      _domains_;  //!< Local indexation of accessible domains
      bool                  _locked_ = false; //!< Lock flag
     
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_ACCESS_PROFILE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
