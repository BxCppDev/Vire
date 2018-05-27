//! \file  vire/cmsserver/uc_resource_constraints.h
//! \brief Vire CMS use case resource constraints
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

#ifndef VIRE_CMSSERVER_UC_RESOURCE_CONSTRAINTS_H
#define VIRE_CMSSERVER_UC_RESOURCE_CONSTRAINTS_H

// Standard Library:
#include <string>
  
namespace vire {

  namespace cmsserver {

    /// \brief Use case resource constraints 
    class uc_resource_constraints
    {

    public:

      uc_resource_constraints();

      ~uc_resource_constraints();
     
      // //! Check if the role expression is set
      // bool has_role_expression() const;

      // //! Set the role expression
      // void set_role_expression(const std::string & role_expression_);

      // //! Return the role expression
      // const std::string & get_role_expression() const;

      // const ::vire::resource::role & get_minimal_role() const;

      // virtual const ::vire::resource::role * _create_minimal_role_() = 0;
     
      // std::unique_ptr<const vire::resource::role> _minimal_role_;         //!< Cached minimal role

    };
    
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_RESOURCE_CONSTRAINTS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
