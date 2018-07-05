//! \file  vire/cmsserver/uc_scope_resources.h
//! \brief Vire CMS use case's relative scope resources
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_UC_SCOPE_RESOURCES_H
#define VIRE_CMSSERVER_UC_SCOPE_RESOURCES_H

// Standard Library:
#include <set>

// This project:
#include <vire/utility/path.h>
      
// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
  
namespace vire {

  namespace cmsserver {

    /// \brief Container that stores the lists of relative and absolute resource paths required by a use case
    class uc_scope_resources
      : public datatools::i_tree_dumpable
    {
    public:
      
      uc_scope_resources();

      virtual ~uc_scope_resources();

      void add_relative_resource(const vire::utility::relative_path & rp_);

      bool has_relative_resource(const vire::utility::relative_path & rp_) const;
      
      const std::set<vire::utility::relative_path> & get_relative_resources() const;

      bool has_absolute_resource(const vire::utility::path & p_) const;

      void add_absolute_resource(const vire::utility::path & p_);
 
      const std::set<vire::utility::path> & get_absolute_resources() const;
  
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options() ) const override;

      void lock();

      bool is_locked() const;
      
    private:

      std::set<vire::utility::relative_path> _relative_resources_;
      std::set<vire::utility::path>          _absolute_resources_;
      bool                                   _locked_ = false;

      BOOST_SERIALIZATION_BASIC_DECLARATION();
       
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_SCOPE_RESOURCES_H
// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
