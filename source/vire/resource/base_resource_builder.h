//! \file  vire/resource/base_resource_builder.h
//! \brief Vire resource builder for the resource manager
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

#ifndef VIRE_RESOURCE_BASE_RESOURCE_BUILDER_H
#define VIRE_RESOURCE_BASE_RESOURCE_BUILDER_H

// Third party:
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/properties.h>

namespace vire {

  namespace resource {

    class manager;

    //! \brief Base builder of resources from an external source
    class base_resource_builder
    {
    public:

      //! Default constructor
      base_resource_builder();

      //! Destructor
      virtual ~base_resource_builder();

      //! Check if the resource manager is set
      bool has_resource_manager() const;

      //! Set the resource manager
      void set_resource_manager(::vire::resource::manager & dev_mgr_);

      //! Return the resource manager
      const ::vire::resource::manager & get_resource_manager() const;

      //! Set the logging priority threshold
      void set_logging_priority(datatools::logger::priority);

      //! Return the logging priority threshold
      datatools::logger::priority get_logging_priority() const;

      //! Check initialization status
      virtual bool is_initialized() const = 0;

      //! Initialize
      void initialize_simple();

      //! Initialize
      virtual void initialize(const datatools::properties & config_) = 0;

      //! Reset
      virtual void reset() = 0;

      //! Build the resources from some external source and add them in the resource manager
      virtual void build_resources() = 0;

    protected:

      //!< Return a reference to the target resource manager
      ::vire::resource::manager & _grab_resource_manager();

    protected:

      datatools::logger::priority _logging_priority; //!< Logging priority threshold

    private:

      ::vire::resource::manager * _resource_manager_; //!< Handle to the resource manager

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace resource

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::resource::base_resource_builder);
#endif // Q_MOC_RUN

#endif // VIRE_RESOURCE_BASE_RESOURCE_BUILDER_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
