//! \file  vire/com/actor.h
//! \brief Vire communication actor
//
// Copyright (c) 2016-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_ACTOR_H
#define VIRE_COM_ACTOR_H

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

namespace vire {

  namespace com {

    class base_plug;
    class plug_factory;
    class manager;
    class domain;
    
    //! \brief Actor description
    //!
    //! Vire uses 3 types of actors for a given control and monitoring system
    //! associated to an experiment:
    //! * a unique Vire server,
    //! * an arbitrary number of Vire clients,
    //! * optional Vire subcontractors (generally at least one) responsible
    //!   of the management of hardware devices.
    //!
    class actor
      : public ::datatools::i_tree_dumpable
      , private boost::noncopyable
    {
    public:

      //! \brief Actor categories
      enum category_type {
        CATEGORY_INVALID                     =  0, ///< Invalid actor category
        CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM = 10, ///< Server subcontractor system category (in CMS server)
        CATEGORY_SERVER_CLIENT_SYSTEM        = 11, ///< Server client system category (in CMS server)
        CATEGORY_SERVER_GATE                 = 12, ///< Server gate system category (in CMS server)
        CATEGORY_SERVER_CMS                  = 13, ///< Server CMS category (in CMS server)
        CATEGORY_CLIENT_SYSTEM               = 20, ///< Client system category (in CMS clients)
        CATEGORY_CLIENT_CMS                  = 21, ///< Client CMS category (in CMS server or clients)
        CATEGORY_CLIENT_GATE                 = 22, ///< Server gate system category (in CMS server)
        CATEGORY_SUBCONTRACTOR               = 30, ///< Subcontractor category (in CMS subcontractors)
      };
 
      //! Return the label associated to a category type
      static std::string to_string(const category_type);

      static bool category_requires_target(const category_type);
 
      static bool category_is_persistant(const category_type);
      
      //! Return the category type associated to a label
      static bool from_string(const std::string &, category_type &);

      typedef std::map<std::string, std::shared_ptr<base_plug>> plug_dict_type;
      typedef std::map<std::string, const domain *> domain_dict_type;

      //! Constructor
      actor(const manager & com_,
            const category_type category_,
            const std::string & target_,
            const std::string & name_,
            const std::string & password_);

      //! Destructor
      virtual ~actor();

      //! Check actor validity
      bool is_valid() const;

      //! Check if name is set
      bool has_name() const;

      //! Set the actor name
      void set_name(const std::string & name_);

      //! Return the actor name
      const std::string & get_name() const;

      //! Check if category is set
      bool has_category() const;

      //! Set the actor category
      void set_category(const category_type & category_);

      //! Return the actor category
      category_type get_category() const;

      //! Check if actor password is set
      bool has_password() const;

      //! Set the actor password
      void set_password(const std::string & password_);

      //! Return the actor password
      const std::string & get_password() const;

      //! Check if a
      bool match_password(const std::string & password_) const;
  
      //! Check if target is set
      bool has_target() const;

      //! Set the actor target
      void set_target(const std::string & target_);

      //! Return the actor target
      const std::string & get_target() const;

      //! Return the metadata
      datatools::properties & grab_metadata();

      //! Return the metadata
      const datatools::properties & get_metadata() const;
       
      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
      
      bool has_plug(const std::string & plug_name_) const;
 
      bool add_plug(const std::string & local_domain_name_,
                    const plug_category_type plug_category_,
                    std::string & plug_name_);
      
      void remove_plug(const std::string & plug_name_);
      
      const std::shared_ptr<base_plug> & get_plug(const std::string & plug_name_) const;
                    
      const plug_dict_type & get_plugs() const;
     
      bool is_locked() const;

      void lock();

      const manager & get_com() const;
 
      plug_factory & grab_plug_factory();

      bool has_domain(const std::string & domain_label_) const;
      
      const domain & get_domain(const std::string & domain_label_) const;

      void build_list_of_domain_local_labels(std::set<std::string> & domain_labels_) const;

      void build_list_of_domain_names(std::set<std::string> & domain_names_) const;

    private:
           
      void _build_default_plugs_();
      
      void _build_domains_indexation_();
      
    private:

      bool                  _locked_ = false;
      const manager *       _com_    = nullptr;
      category_type         _category_; //!< Actor category
      std::string           _name_;     //!< Actor name
      std::string           _password_; //!< Actor password to access the underlying com system
      std::string           _target_;   //!< Client or subcontractor unique identifier
      datatools::properties _metadata_; //!< Metadata
      domain_dict_type      _domains_;  //!< Local domains     
      plug_dict_type        _plugs_;    //!< Plugs
 
      std::unique_ptr<plug_factory> _plug_factory_;
      friend plug_factory;
      
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_ACTOR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
