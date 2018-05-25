//! \file    vire/resource/base_resource_instance.h
//! \brief   Representation of any accessible resource of the environment
//! \details The Vire library is able to describe resource objects
//!          in some environment that can be accessed through sessions
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_RESOURCE_BASE_RESOURCE_INSTANCE_H
#define VIRE_RESOURCE_BASE_RESOURCE_INSTANCE_H

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/service_manager.h>
#include <datatools/enriched_base.h>

// This project:
#include <vire/resource/utils.h>

namespace vire {

  namespace resource {

    //! \brief Base resource instance
    class base_resource_instance
      : public datatools::enriched_base
    {
    public:

      //! Default constructor
      base_resource_instance(resource_category_type = RESOURCE_CATEGORY_INVALID);

      //! Destructor
      virtual ~base_resource_instance();

      //! Check if the resource category is set
      bool has_resource_category() const;

      //! Return the resource category
      resource_category_type get_resource_category() const;

      //! Set the resource category
      void set_resource_category(resource_category_type);

      //! Check if the resource is a method
      bool is_method() const;

      //! Check if the resource is a datapoint
      bool is_datapoint() const;

      //! Check if the resource is user defined
      bool is_user() const;

      //! Check initialization status
      virtual bool is_initialized() const = 0;

      //! Initialize
      void initialize_simple();

      //! Initialize
      void initialize_standalone(const datatools::properties & config_);

      //! Initialize
      virtual void initialize(const datatools::properties & config_,
                              datatools::service_manager &) = 0;

      //! Reset
      virtual void reset() = 0;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected:

      //! Base initialization
      void _base_initialize(const datatools::properties & config_);

      //! Base reset
      void _base_reset();

    private:

      resource_category_type _resource_category_; //!< Resource category

    };

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_BASE_RESOURCE_INSTANCE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
