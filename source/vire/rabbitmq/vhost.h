//! \file  vire/rabbitmq/vhost.h
//! \brief Vire CMS RabbitMQ vhost
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

#ifndef VIRE_RABBITMQ_VHOST_H
#define VIRE_RABBITMQ_VHOST_H

// Standard Library:
#include <string>

// Third party:
// - Bayeux:
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/com/utils.h>

namespace vire {

  namespace rabbitmq {

    class manager_service;
    
    ///! Description of a RabbitMQ vhost for the Vire system
    struct vhost
    {
    public:
 
      /// Default constructor
      vhost();

      /// Constructor
      vhost(const std::string & name_,
            const vire::com::domain_category_type category_);

      /// Check is a name is valid for a vhost
      static bool validate_name(const std::string & name_);

      /// Set the vhost's name
      void set_name(const std::string & name_);

      /// Return the vhost's name
      const std::string & get_name() const;

      /// Set the vhost's category
      void set_category(const vire::com::domain_category_type category_);

      /// Return the vhost's category
      const vire::com::domain_category_type get_category() const;

      /// Check if the vhost is complete (all mandatory fields are set)
      bool is_complete() const;

      /// Initialize (lock)
      void initialize();

      /// Initialize from parameters (lock)
      void initialize(const datatools::properties & config_);

      /// Reset (invalidate)
      void reset();
      
    private:
      
      std::string                     _name_;  //!< Name of the RabbitMQ vhost
      vire::com::domain_category_type _category_ = vire::com::DOMAIN_CATEGORY_INVALID; //!< User category
      
      friend manager_service;
      
    };

  } // namespace rabbitmq

} // namespace vire

#endif // VIRE_RABBITMQ_VHOST_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
