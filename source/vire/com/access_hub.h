/// \file  vire/com/access_hub.h
/// \brief Vire communication access hub
//
// Copyright (c) 2016-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2016-2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_ACCESS_HUB_H
#define VIRE_COM_ACCESS_HUB_H

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
    class access_profile;
    class access_hub;

    typedef std::shared_ptr<access_hub> access_hub_ptr_type;
    typedef std::shared_ptr<base_plug>  plug_ptr_type;

    /// \brief Access hub for message transport
    ///
    class access_hub
      : public ::datatools::i_tree_dumpable
      , private boost::noncopyable
    {
    public:

      /// \brief Description record for a plug
      struct plug_entry
      {
        bool          removable = false; ///< Default plug flag
        plug_ptr_type plug_handle; ///< Handle to the plug
      };

      /// \brief Dictionary of plug records
      typedef std::map<std::string, plug_entry> plug_dict_type;

      /// \brief Description record for an accessible domain
      struct domain_entry
      {
        const domain * dom = nullptr; ///< Handle to the domain
        plug_dict_type plugs;         ///< Dictionary of plugs associated to the domain
      };
      
      /// \brief Dictionary of domain records
      typedef std::map<std::string, domain_entry> domain_dict_type;

      /// Constructor
      // XXX private ctor
      access_hub(const access_profile & profile_,
            const std::string & name_,
            const std::set<std::string> & domain_names_,
            const bool lock_ = false);

      /// Destructor
      virtual ~access_hub();
      
      /// Return the access hub name
      const std::string & get_name() const;

      /// Return a handle to the access profile
      const access_profile & get_profile() const;

      /// Build a list of local names for accessible domain
      void build_list_of_domain_local_names(std::set<std::string> & domain_names_) const;

      /// Check if a domain is accessible by the access hub
      bool has_domain(const std::string & local_domain_name_) const;

      /// Return a handle to an accessible domain
      const domain & get_domain(const std::string & local_domain_name_) const;
                   
      /// Check if the access hub is locked, i.e. one cannot create/remove plugs
      bool is_locked() const;

      /// Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_ = empty_options()) const override;

      /// Check if a plug associated to a domain exists
      bool has_plug(const std::string & local_domain_name_,
                    const std::string & plug_name_) const;

      /// Create a new plug associated to a domain
      void create_plug(const std::string & local_domain_name_,
                       const std::string & plug_name_,
                       const plug_category_type plug_category_,
                       const bool removable_ = true);

      /// Check if a plug associated to a domain is removable
      bool is_removable_plug(const std::string & local_domain_name_,
                             const std::string & plug_name_) const;
      
      /// Destroy a plug associated to a domain
      void destroy_plug(const std::string & local_domain_name_,
                        const std::string & plug_name_);
      
      /// Return a plug associated to a domain
      const std::shared_ptr<base_plug> & get_plug(const std::string & local_domain_name_,
                                                  const std::string & plug_name_) const;
      
      /// Return a plug associated to a domain
      std::shared_ptr<base_plug> & grab_plug(const std::string & local_domain_name_,
                                             const std::string & plug_name_);

    private:

      void _set_name_(const std::string & name_);
      
      void _init_domains_(const std::set<std::string> & domain_names_);
      
      void _build_default_plugs_();
      
    private:

      const access_profile *    _profile_ = nullptr;  ///< Profile
      std::string               _name_;               ///< Access hub name
      domain_dict_type          _domains_with_plugs_; ///< Local registry of plugs per domain     
      std::unique_ptr<plug_factory> _plug_factory_;   ///< Plug factory handler
      
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_ACCESS_HUB_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
